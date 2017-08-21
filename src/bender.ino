// progra de prueba
#include "Arduino.h"
#include "serial_pack.hpp"
#include <TimerOne.h>
#include <Servo.h>
#define DEBUG
#define LED1 13   // led de destello
#define LED2 2    // led verde
#define LED3 3    // del rojo
#define LED4 4    // PIN SERVO ON

// 4 canales de Voltaje las 2 ultimas lecturas seran compart7B 41 42 53 11 00 00 00 01 9C 7Didas
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

// DIRECCION DEL MODULO
/////////////////////////////
#define DEV_ADDRESS 0x4142
/////////////////////////////
// intervalo de tiempo para enviar el estado del sistema
#define BCAST_INTERVAL 30
#define N_MUESTRAS 200
// definicion de la velocidad serial
#define BAUD_RATE 9600
// DEFINICIONES DE CADA MODULO
// AQUI SELECCIONA EL NO DE EJE QUE VA ESCUCHAR
// pines para leeer el eje seleccionado
#define EJE1 10
#define EJE2 11
#define EJE3 12


// numeros de la funcion del eje
#define MI_EJE_F01 0x01
#define MI_EJE_F02 0x02
#define MI_EJE_F03 0x03
#define MI_EJE_F04 0x04

#define DIR A0  // PIN A0 para el Pulso al driver
#define PULSO A1  // PIN A1 para la direccion al driver

#define DOBLA_PIN 3
#define ALIMENTA_PIN 5
Servo servo_alim;
Servo servo_dobla;
int mi_eje=0xFF;
bool mi_eje_enabled=false;
float cal_factor = 1.0; // factor de calibracion mecanica del eje

// FIN DEFINICION DE EJE
// variales de estado
float I1,I2,I3,I4; // cooriente
float V1,V2,V3,V4; // voltaje
float T1,T2,T3,T4; // temperatura


Serial_Pack sp_in(DEV_ADDRESS);
Serial_Pack sp_out(DEV_ADDRESS);
long ticks=0;
long pticks=0;
bool bcast_enabled=true;
bool broad_cast=false;

void setup(){


  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(DIR, OUTPUT);
  pinMode(PULSO, OUTPUT);

  pinMode(EJE1,INPUT);
  pinMode(EJE2,INPUT);
  pinMode(EJE3,INPUT);

  Timer1.initialize(1000000); // timer de 1 segundo para contar el tiempo
  Timer1.attachInterrupt(inc_segundos);
  broad_cast=false;
  servo_alim.attach(ALIMENTA_PIN);
  servo_dobla.attach(DOBLA_PIN);
  // 10 destellos para informar que arranco

  blink_led1();
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,HIGH);   // led verde off y rojo ON todavia no esta listo
  Serial.begin(BAUD_RATE);
  // Serial.println("{WIRE BEND CONTROLLER V0.1}");
  init_servo_pos();
  leer_mi_eje();
}
void leer_mi_eje(){
  byte Bit0 = 0;
  byte Bit1 = 0;
  byte Bit2 = 0;
  mi_eje =0;
  if (digitalRead(EJE1)==HIGH ) Bit0 =1; else Bit0=0;
  if (digitalRead(EJE2)==HIGH ) Bit1 =2; else Bit1=0;
  if (digitalRead(EJE3)==HIGH ) Bit2 =4; else Bit2=0;
  // esto es equivalente a B2+B1+B0
  mi_eje = (Bit2 | Bit1 | Bit0);

};
void blink_led1(){
  for (int  i = 0;  i < 10;  i++) {
    digitalWrite(LED1,HIGH);
    delay(200);
    digitalWrite(LED1,LOW);
    delay(200);
  }
}
void inc_segundos(){
  ticks++;
  if (((ticks-pticks) >BCAST_INTERVAL) && bcast_enabled) broad_cast=true;
}
float get_corriente(int n_muestras,byte PIN_READ)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(PIN_READ) * (5.0 / 1023.0);////lectura del sensor
    corriente = corriente +  (voltajeSensor-2.5)/1.1; //Ecuación  para obtener la corriente
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
void send_eje_listo(){
  sp_out.set_t_func(T_FUNC_ACK);
  sp_out.set_function(N_FUNC_NULL);
  sp_out.set_cur_value(float(mi_eje));

};
void send_estatus(uint8_t funcion,float valor ){
  sp_out.set_function(funcion);
  sp_out.set_t_func(T_FUNC_REQ);
  sp_out.set_cur_value(valor);
  sp_out.enviar(&Serial);
  Serial.flush();
  //delay(1);
}
void send_ack(int8_t funcion,float valor){
  sp_out.set_function(funcion);
  sp_out.set_t_func(T_FUNC_ACK);
  sp_out.set_cur_value(valor);
  sp_out.enviar(&Serial);
  Serial.flush();
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
          // FUNCION NEOPIXEL
      break;

      // ejecuta las funciones de cada eje
      case CNC_AX0_F01: if (mi_eje==0) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX0_F02: if (mi_eje==0) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX0_F03: if (mi_eje==0) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX0_F04: if (mi_eje==0) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX1_F01: if (mi_eje==1) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX1_F02: if (mi_eje==1) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX1_F03: if (mi_eje==1) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX1_F04: if (mi_eje==1) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX2_F01: if (mi_eje==2) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX2_F02: if (mi_eje==2) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX2_F03: if (mi_eje==2) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX2_F04: if (mi_eje==2) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX3_F01: if (mi_eje==3) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX3_F02: if (mi_eje==3) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX3_F03: if (mi_eje==3) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX3_F04: if (mi_eje==3) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX4_F01: if (mi_eje==4) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX4_F02: if (mi_eje==4) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX4_F03: if (mi_eje==4) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX4_F04: if (mi_eje==4) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX5_F01: if (mi_eje==5) ejecutar_funcion(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX5_F02: if (mi_eje==5) ejecutar_funcion(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX5_F03: if (mi_eje==5) ejecutar_funcion(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX5_F04: if (mi_eje==5) ejecutar_funcion(MI_EJE_F04,sp_in.get_cur_valuef()); break;

      case CNC_AX6_F01: if (mi_eje==6) ejecutar_funcion2(MI_EJE_F01,sp_in.get_cur_valuef()); break;
      case CNC_AX6_F02: if (mi_eje==6) ejecutar_funcion2(MI_EJE_F02,sp_in.get_cur_valuef()); break;
      case CNC_AX6_F03: if (mi_eje==6) ejecutar_funcion2(MI_EJE_F03,sp_in.get_cur_valuef()); break;
      case CNC_AX6_F04: if (mi_eje==6) ejecutar_funcion2(MI_EJE_F04,sp_in.get_cur_valuef()); break;

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
// funcion CNC a ejecutar
// esta funcion recibe 2 parametros el tipo de funcion y el valor
// en este ejemplo es un servo el valor puede estar entre -100 a 100 grados
void ejecutar_funcion(int nfunc,float valor){

  switch (nfunc) {
    case MI_EJE_F01:  // funcion 1 envio avance / rertoceso por unidades absolutas
        if (mi_eje_enabled) enviar_distancia(valor);
    break;
    case MI_EJE_F02:  // funcion 2 avance / retroceso por pulsos
        if (mi_eje_enabled) enviar_pulsos(valor);
    break;
    case MI_EJE_F03: // funcion 3 acepar valor de calibracion distancia / pulsos
        cal_factor = valor;
    break;
    case MI_EJE_F04:  // funcion 4 // habilitar/ deshabilitar el eje
      if (valor>99) mi_eje_enabled=true; else mi_eje_enabled=false;
      if (mi_eje_enabled) {
        digitalWrite(LED2,HIGH);  // habilitado luz Verde
        digitalWrite(LED4,HIGH);  // habilitado SERVO ON
        digitalWrite(LED3,LOW);
      } else{
        digitalWrite(LED2,LOW);  // no va a procesar nada Luz roja
        digitalWrite(LED4,LOW);  // Servo off
        digitalWrite(LED3,HIGH);
      }
    break;
  }
  // enviamos ACK para decir que terminamos


  send_ack(nfunc, valor);

}
// funcioes diferentes para el eje 6
void ejecutar_funcion2(int nfunc,float valor){

  switch (nfunc) {
    case MI_EJE_F01:  // funcion 1 envio avance / rertoceso por unidades absolutas
        if (mi_eje_enabled)avanzar(valor);
    break;
    case MI_EJE_F02:  // funcion 2 acance / retroceso por pulsos
        if (mi_eje_enabled) dobla(int(valor));

    break;
    case MI_EJE_F03:

    break;
    case MI_EJE_F04:  // funcion 4 // habilitar/ deshabilitar el eje
      if (valor>99) mi_eje_enabled=true; else mi_eje_enabled=false;
      if (mi_eje_enabled) {
        digitalWrite(LED2,HIGH);  // habilitado luz Verde
        digitalWrite(LED3,LOW);
      } else{
        digitalWrite(LED2,LOW);  // no va a procesar nada Luz roja
        digitalWrite(LED3,HIGH);
      }

    break;


  }
  // enviamos ACK para decir que terminamos

  send_ack(nfunc, valor);

}

void enviar_distancia(float valor){
  float n_pulsos=0;
  n_pulsos = valor * cal_factor;
  enviar_pulsos(n_pulsos);
};
void enviar_pulsos(float pulsos){
  int int_pulsos = int(pulsos);
  int direccion =0;
  if (pulsos>0) direccion=HIGH; else direccion=LOW;

  // esta funcion es critica, basicamente la funcion digitalWrite es muy lenta
  // asi que se debe hacer acceso directo a alos puertos para mayor velocidad
  // antes de optimizar usamos digitalWrite si es muy lento pasamos
  // a un acceso masrapido tipo PORTC = PORTC | BITMASK

  digitalWrite(DIR,direccion);

  for (int i = 0;i < int_pulsos; i++) {
    digitalWrite(PULSO,HIGH);
    delayMicroseconds(5);  // la frecuencia maxima del driver es de 200 Kpss
    digitalWrite(PULSO,LOW);
  };



};
void avanzar(float cantidad){
  int n_paso=int(abs(cantidad));

  for (int i = 0; i < n_paso; i++) {
    if (cantidad >0) avance(); else retroceso();
  }

};

void avance()  {
  servo_alim.write(105);
  delay(50);
  servo_alim.write(95);

}
void retroceso() {
  servo_alim.write(75);
  delay(50);
  servo_alim.write(95);
}
void dobla(int angulo){
  //blink_led1();
  servo_dobla.write(angulo);
  delay(1000);
  servo_dobla.write(5);
  delay(50);

}
void init_servo_pos(){
  servo_dobla.write(5);

}
void stop_servos() {
  //servo_base.detach();
  //servo_brazo.detach();
  analogWrite(DOBLA_PIN, 0);
  analogWrite(ALIMENTA_PIN, 0);
}
