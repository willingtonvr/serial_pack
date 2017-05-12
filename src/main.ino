// progra de prueba
#include "Arduino.h"
#include "serial_pack.hpp"
Serial_Pack sp_in(0x4142);
Serial_Pack sp_out(0x4142);
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>

/// estos pines dependen del modelo del Arduino
// pines del Arduino UNO
#define LED1 13
#define LED2 10
#define PIN_BATERIA A0

#define Pin 2 //Se declara el pin donde se conectará la DATA

OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire

DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

float Sensibilidad=0.185; //sensibilidad en Voltios/Amperio para sensor de 5A


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

float get_corriente(int n_muestras)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);////lectura del sensor
    corriente=corriente+(voltajeSensor-2.5)/Sensibilidad; //Ecuación  para obtener la corriente
  }
  corriente=corriente/n_muestras;
  return(corriente);
}

void loop(){
// leer el Serial
// verificar que sea un paquete
// verificar el Checksum
// si Checksum esta ok actuar

float I=get_corriente(200);//obtenemos la corriente promedio de 200 muestras

// Serial.print("Corriente: ");
// Serial.println(I,5);

sensors.requestTemperatures(); //Prepara el sensor para la lectura

//Serial.print(sensors.getTempCByIndex(0)); //Se lee e imprime la temperatura en grados Celsius


sp_in.leer(&Serial);

if (sp_in.ChecksumOK()) {
  switch (sp_in.get_function()) {
    case N_FUNC_LED1:
      if (sp_in.get_cur_value_LSB()==255){
          digitalWrite(LED1, HIGH);
      } else {
          digitalWrite(LED1, LOW);
      };
    break;
    case N_FUNC_LED2:
        if (sp_in.get_cur_value_LSB()==255){
            digitalWrite(LED2, HIGH);
        } else {
            digitalWrite(LED2, LOW);
        };
    break;
    case N_FUNC_CORR: // leer corriente

    break;
    case N_FUNC_BAT: // Voltaje de bateria
        sp_out.set_function(N_FUNC_BAT);
        sp_out.set_cur_value(analogRead(PIN_BATERIA));

        sp_out.enviar(&Serial);

    break;

  }
}
sp_in.set_function(N_FUNC_NULL);

}
