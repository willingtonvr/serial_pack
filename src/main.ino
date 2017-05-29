// progra de prueba
#include "Arduino.h"
#include "serial_pack.hpp"
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    12    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  4

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/// estos pines dependen del modelo del Arduino
// pines del Arduino UNO
#define LED1 13
#define LED2 6
#define LED3 5
#define LED4 4 /// esta el neopixel
// 4 canales de Voltaje las 2 ulyimas lecturas seran compartidas
#define PIN_BAT_V1 A0
#define PIN_BAT_V2 A1
#define PIN_BAT_V3 A2
#define PIN_BAT_V4 A2

// 3 canales de Corriente  idem a voltaje
#define PIN_BAT_I1 A3
#define PIN_BAT_I2 A4
#define PIN_BAT_I3 A5
#define PIN_BAT_I4 A5
//#define PIN_BAT_I4 A7
// 4 canales de salida digitales para activar el relé del cargador
// 4 canales de Corriente
#define PIN_ACTU1 LED1
#define PIN_ACTU2 LED2
#define PIN_ACTU3 LED3
#define PIN_ACTU4 LED4
#define N_MUESTRAS 200 // numero de muestras por variable
// los sensores de temperatura van todos por oneWire
#define PIN_WIRE_TEMP 2 //Se declara el PIN_WIRE_TEMP donde se conectará la DATA
// DIRECCION DEL MODULO
/////////////////////////////
#define DEV_ADDRESS 0x4142
/////////////////////////////
// intervalo de tiempo para enviar el estado del sistema
#define BCAST_INTERVAL 30

// variales de estado
float I1,I2,I3,I4; // cooriente
float V1,V2,V3,V4; // voltaje
float T1,T2,T3,T4; // temperatura

OneWire ourWire(PIN_WIRE_TEMP); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature
float Sensibilidad=0.185; //sensibilidad en Voltios/Amperio para sensor de 5A

Serial_Pack sp_in(DEV_ADDRESS);
Serial_Pack sp_out(DEV_ADDRESS);
long ticks=0;
long pticks=0;
bool broad_cast=false;
void setup(){
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(PIXEL_PIN,OUTPUT);

  pinMode(PIN_BAT_I1,INPUT);
  pinMode(PIN_BAT_I2,INPUT);
  pinMode(PIN_BAT_I3,INPUT);
  pinMode(PIN_BAT_V1,INPUT);
  pinMode(PIN_BAT_V2,INPUT);
  pinMode(PIN_BAT_V3,INPUT);

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
  // incializacion del neopixe
  //
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  delay(200);
  Serial.begin(9600);
  Serial.println("{CARGADOR BICICLEAS V0.1}");
  sensors.begin(); //Se inician los sensores
  Timer1.initialize(1000000); // timer de 1 segundo para contar el tiempo
  Timer1.attachInterrupt(inc_segundos);
  broad_cast=false;
  for (byte j = 0; j < 4; j++) {

    for (byte i = 0; i < 5; i++) {
      /* code */
      strip.setPixelColor(j, 255,  0, 255);
      strip.show();
      delay(100);
      strip.setPixelColor(j, 255,  0, 0);
      strip.show();
      delay(100);
    }
}
  for (byte j = 0; j < 4; j++) {
    strip.setPixelColor(j, 0,  0, 0);
  }
  strip.show();
}
void inc_segundos(){
  ticks++;
  if ((ticks-pticks) >BCAST_INTERVAL) broad_cast=true;
}
float get_corriente(int n_muestras,byte PIN_READ)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(PIN_READ) * (5.0 / 1023.0);////lectura del sensor
    corriente = corriente +  (voltajeSensor-2.5)/Sensibilidad; //Ecuación  para obtener la corriente
  }

  return(corriente/n_muestras);
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
void broadcast(){  // emitimos el estado
      //Serial.print("Tick: ");
      //Serial.println(ticks);
      send_estatus(N_FUNC_CORR1, I1);
      delay(10);
      send_estatus(N_FUNC_CORR2, I2);
      delay(10);
      send_estatus(N_FUNC_CORR3, I3);
      delay(10);
      send_estatus(N_FUNC_CORR4, I4);
      delay(10);

      send_estatus(N_FUNC_VOLT1, V1);
      delay(10);
      send_estatus(N_FUNC_VOLT2, V2);
      delay(10);
      send_estatus(N_FUNC_VOLT3, V3);
      delay(10);
      send_estatus(N_FUNC_VOLT4, V4);
      delay(10);

      send_estatus(N_FUNC_TEMP1, T1);
      delay(10);
      send_estatus(N_FUNC_TEMP2, T2);
      delay(10);
      send_estatus(N_FUNC_TEMP3, T3);
      delay(10);
      send_estatus(N_FUNC_TEMP4, T4);
      delay(10);
}
void send_estatus(uint8_t funcion,float valor ){
  sp_out.set_function(funcion);
  sp_out.set_cur_value(valor);
  sp_out.enviar(&Serial);
  Serial.flush();
  //delay(1);
}


void loop(){
// leer el Serial
// verificar que sea un paquete
// verificar el Checksum
// si Checksum esta ok actuar
// muesreo de variables
I1=get_corriente(N_MUESTRAS,PIN_BAT_I1);//obtenemos la corriente promedio
I2=get_corriente(N_MUESTRAS,PIN_BAT_I2);
I3=get_corriente(N_MUESTRAS,PIN_BAT_I3);
I4=get_corriente(N_MUESTRAS,PIN_BAT_I4);

V1=get_voltaje(N_MUESTRAS,PIN_BAT_V1);//obtenemos el Voltaje promedio
V2=get_voltaje(N_MUESTRAS,PIN_BAT_V2);
V3=get_voltaje(N_MUESTRAS,PIN_BAT_V3);
V4=get_voltaje(N_MUESTRAS,PIN_BAT_V4);

sensors.requestTemperatures(); //Prepara el sensor para la lectura
T1 = sensors.getTempCByIndex(0);
T2 = sensors.getTempCByIndex(1);
T3 = sensors.getTempCByIndex(2);
T4 = sensors.getTempCByIndex(3);

if (broad_cast) {
  broadcast();
  pticks=ticks;
  broad_cast=false;
}


sp_in.leer(&Serial);

if (sp_in.ChecksumOK()) {
/* TODO
  Despues de leer el serial y verificar el checksum se deberia
  verificar la direccion de destino, de esta forma se puede conectar
  varios modulos en el mismo canal serial
*/
  // los pines de led son los mismos de led actuador
if (sp_in.getSet_Request()){  // activar hardware

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
      case N_FUNC_NEO:
          strip.setPixelColor(sp_in.get_cur_value_MSB_H(),  // numero de pixels
                              sp_in.get_cur_value_MSB_L(),  // R
                              sp_in.get_cur_value_LSB_H(),  // G
                              sp_in.get_cur_value_LSB_L()  // B
                            );
          strip.show();
      break;
    }

} else { // enviar lecturas hardware
    switch (sp_in.get_function()) {

      case N_FUNC_CORR1: // Corriente de bateria
          send_estatus(N_FUNC_CORR1, I1);
      break;
      case N_FUNC_CORR2: // Corriente de bateria
          send_estatus(N_FUNC_CORR2, I2);
      break;
      case N_FUNC_CORR3: // Corriente de bateria
          send_estatus(N_FUNC_CORR3, I3);
      break;
      case N_FUNC_CORR4: // Corriente de bateria
          send_estatus(N_FUNC_CORR4, I4);
      break;
      case N_FUNC_VOLT1: // Voltaje de bateria 1
          send_estatus(N_FUNC_VOLT1, V1);
      break;
      case N_FUNC_VOLT2: // Voltaje de bateria 2
        send_estatus(N_FUNC_VOLT2, V2);
      break;
      case N_FUNC_VOLT3: // Voltaje de bateria 3
          send_estatus(N_FUNC_VOLT3, V3);
      break;
      case N_FUNC_VOLT4: // Voltaje de bateria 3
          send_estatus(N_FUNC_VOLT4, V4);
      break;
      case N_FUNC_TEMP1: // temperatura 1
          send_estatus(N_FUNC_TEMP1, T1);
      break;
      case N_FUNC_TEMP2:
          send_estatus(N_FUNC_TEMP2, T2);
      break;
      case N_FUNC_TEMP3:
          send_estatus(N_FUNC_TEMP3, T3);
      break;
      case N_FUNC_TEMP4:
          send_estatus(N_FUNC_TEMP4, T4);
      break;
    }
}

} // checksumok

sp_in.set_function(N_FUNC_NULL);

}
