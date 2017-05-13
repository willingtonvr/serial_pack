// progra de prueba
#include "Arduino.h"
#include "serial_pack.hpp"
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

/// estos pines dependen del modelo del Arduino
// pines del Arduino UNO
#define LED1 13
#define LED2 10
#define LED3 10
#define LED4 10
// 3 canales de Voltaje
#define PIN_BAT_V1 A0
#define PIN_BAT_V2 A1
#define PIN_BAT_V3 A2
//#define PIN_BAT_V4 A3
// 3 canales de Corriente
#define PIN_BAT_I1 A3
#define PIN_BAT_I2 A4
#define PIN_BAT_I3 A5
//#define PIN_BAT_I4 A7
// 4 canales de salida digitales para activar el relé del cargador
// 4 canales de Corriente
#define PIN_ACTU1 LED1
#define PIN_ACTU2 LED2
#define PIN_ACTU3 LED3
#define PIN_ACTU4 LED4
#define N_MUESTRAS 100 // numero de muestras por variable
// los sensores de temperatura van todos por oneWire
#define PIN_WIRE_TEMP 2 //Se declara el PIN_WIRE_TEMP donde se conectará la DATA
// DIRECCION DEL MODULO
/////////////////////////////
#define DEV_ADDRESS 0x4142
/////////////////////////////

OneWire ourWire(PIN_WIRE_TEMP); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature
const float Sensibilidad=0.185; //sensibilidad en Voltios/Amperio para sensor de 5A

Serial_Pack sp_in(DEV_ADDRESS);
Serial_Pack sp_out(DEV_ADDRESS);

void setup(){
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  /* manuela afirma que hay un led en el pin 13 de todo Arduino
  */
  digitalWrite(LED1,HIGH);
  delay(200);
  digitalWrite(LED1,LOW);
  delay(200);
  digitalWrite(LED1,HIGH);
  delay(500);
  digitalWrite(LED1,LOW);
  delay(200);
  digitalWrite(LED1,LOW);
  /* es cierto :|
  */
  delay(200);
  Serial.begin(9600);
  sensors.begin(); //Se inician los sensores
  Serial.println("Listos para la accion");
}

float get_corriente(int n_muestras,byte PIN_READ)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(PIN_READ) * (5.0 / 1023.0);////lectura del sensor
    corriente=corriente+(voltajeSensor-2.5)/Sensibilidad; //Ecuación  para obtener la corriente
  }
  corriente=corriente/n_muestras;
  return(corriente);
}
float get_voltaje(int n_muestras,byte PIN_READ)
{
  float voltajeSensor;

  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(PIN_READ) * (5.0 / 1023.0);////lectura del sensor
  }
  return(voltajeSensor);
}

void loop(){
// leer el Serial
// verificar que sea un paquete
// verificar el Checksum
// si Checksum esta ok actuar
// muesreo de variables
float I1=get_corriente(N_MUESTRAS,PIN_BAT_I1);//obtenemos la corriente promedio
float I2=get_corriente(N_MUESTRAS,PIN_BAT_I2);
float I3=get_corriente(N_MUESTRAS,PIN_BAT_I3);

float V1=get_voltaje(N_MUESTRAS,PIN_BAT_V1);//obtenemos el Voltaje promedio
float V2=get_voltaje(N_MUESTRAS,PIN_BAT_V2);
float V3=get_voltaje(N_MUESTRAS,PIN_BAT_V3);

sensors.requestTemperatures(); //Prepara el sensor para la lectura
float T1 = sensors.getTempCByIndex(0);
float T2 = sensors.getTempCByIndex(1);
float T3 = sensors.getTempCByIndex(2);

sp_in.leer(&Serial);

if (sp_in.ChecksumOK()) {
/* TODO
  Despues de leer el serial y verificar el checksum se deberia
  verificar la direccion de destino, de esta forma se puede conectar
  varios modulos en el mismo canal serial
*/
  // los pines de led son los mismos de led actuador
  switch (sp_in.get_function()) {
    case N_FUNC_LED1:
      if (sp_in.get_cur_value_LSB_L()==255){
          digitalWrite(LED1, HIGH);
      } else {
          digitalWrite(LED1, LOW);
      };
    break;
    case N_FUNC_LED2:
        if (sp_in.get_cur_value_LSB_L()==255){
            digitalWrite(LED2, HIGH);
        } else {
            digitalWrite(LED2, LOW);
        };
    break;
    case N_FUNC_LED3:
        if (sp_in.get_cur_value_LSB_L()==255){
            digitalWrite(LED3, HIGH);
        } else {
            digitalWrite(LED3, LOW);
        };
    break;
    case N_FUNC_LED4:
        if (sp_in.get_cur_value_LSB_L()==255){
            digitalWrite(LED4, HIGH);
        } else {
            digitalWrite(LED4, LOW);
        };
    break;
    case N_FUNC_CORR1: // Corriente de bateria
        sp_out.set_function(N_FUNC_CORR1);
        sp_out.set_cur_value(I1);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_CORR2: // Corriente de bateria
        sp_out.set_function(N_FUNC_CORR2);
        sp_out.set_cur_value(I2);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_CORR3: // Corriente de bateria
        sp_out.set_function(N_FUNC_CORR3);
        sp_out.set_cur_value(I3);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_VOLT1: // Voltaje de bateria 1
        sp_out.set_function(N_FUNC_VOLT1);
        sp_out.set_cur_value(V1);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_VOLT2: // Voltaje de bateria 2
        sp_out.set_function(N_FUNC_VOLT2);
        sp_out.set_cur_value(V2);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_VOLT3: // Voltaje de bateria 3
        sp_out.set_function(N_FUNC_VOLT3);
        sp_out.set_cur_value(V3);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_TEMP1: // temperatura 1
        sp_out.set_function(N_FUNC_TEMP1);
        sp_out.set_cur_value(T1);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_TEMP2:
        sp_out.set_function(N_FUNC_TEMP2);
        sp_out.set_cur_value(T2);
        sp_out.enviar(&Serial);
    break;
    case N_FUNC_TEMP3:
        sp_out.set_function(N_FUNC_TEMP3);
        sp_out.set_cur_value(T3);
        sp_out.enviar(&Serial);
    break;


  }
}
sp_in.set_function(N_FUNC_NULL);

}
