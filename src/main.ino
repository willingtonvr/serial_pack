// progra de prueba
#include "Arduino.h"
#include "serial_pack.hpp"
Serial_Pack sp_in(0x4142);
Serial_Pack sp_out(0x4142);

/// estos pines dependen del modelo del Arduino
// pines del Arduino UNO
#define LED1 13
#define LED2 10
#define PIN_BATERIA A0
//

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
  Serial.println("Listos para la accion");
}

void loop(){
// leer el Serial
// verificar que sea un paquete
// verificar el Checksum
// si Checksum esta ok actuar

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


    case N_FUNC_BAT: // Voltaje de bateria
        sp_out.set_function(N_FUNC_BAT);
        sp_out.set_cur_value(analogRead(PIN_BATERIA));

        sp_out.enviar(&Serial);

    break;

  }
}
sp_in.set_function(N_FUNC_NULL);

}
