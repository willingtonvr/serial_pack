/*

el paquete de datos estara conformado de la siguiente forma:

**INCIO del frame**: 0X7B = '{'
+ **id modulo (2 bytes)**:
* 	MSB id modulo
* 	LSB id modulo
+ **Tipo de Funcion**:
*	Request/set	: 0x53  = 'S'
*	Response	: 0x52  = 'R'
*	Ack			: 0x4B  = 'K'
+ **Nombre de Funcion**:
*	Voltaje 	: 0xA1
*	Corriente : 0xA2
*	Tiempo		: 0xA3
* Sensor1		: 0xA4
*	Sensor2		: 0xA5
*	Sensor3		: 0xA5
*	Sensor4		: 0xA7
*	LED1		: 0xA8
*	LED2		: 0xA9
* LED3		: 0xAA
*	LED3		: 0xAB
* 	id modulo   : 0xB0
* 	id usuario	: 0xB1
* 	tipo bateria: 0xD0
+ **Valor de la funcion (2 bytes)**:
*	MSB byte
*	LSB byte
+ **Checksum: 255 menos la suma de los bytes todos los valores a partir del encabezado.**
+ **FIN PAQUETE** :0x7D = '}'
FIN PAQUETE 0x7D = '}'
*/
#include <Arduino.h>
//#include <Stream.h>
#ifndef  SERIAL_PACK_H
#define SERIAL_PACK_H
/* definiciones generales */
#define START_PACK   0x7B
#define END_PACK     0x7D
#define T_FUNC_SET   0x53
#define T_FUNC_REQ   0x52
#define T_FUNC_ACK   0x4B
#define N_FUNC_NULL  0xFE // funcion vacia para hacer ACK
#define N_FUNC_VOLT  0xA1 // Voltaje
#define N_FUNC_CORR  0xA2 // Corriente
#define N_FUNC_TIME  0xA3 // Tiempo
#define N_FUNC_SENS1 0xA4 // Sensor 1
#define N_FUNC_SENS2 0xA5 // Sensor 2
#define N_FUNC_SENS3 0xA6 // Sensor 3
#define N_FUNC_SENS4 0xA7 // Sensor 4
#define N_FUNC_LED1  0xA8 // Led 1
#define N_FUNC_LED2  0xA9 // led 2
#define N_FUNC_LED3  0xAA // Led 3
#define N_FUNC_LED4  0xAB // led 4
#define N_FUNC_MOD   0xB0 // Id modulo
#define N_FUNC_USR   0xA1 // Id usuario
#define N_FUNC_BAT   0xD0 // tipo bateria

class Serial_Pack{
public:
  Serial_Pack(uint16_t id_modulo);
  void set_id_modulo(uint16_t newid);
  uint16_t get_id_modulo();
  void set_id_usuario(uint16_t newid);
  uint16_t get_id_usuario();
  void set_function(uint8_t newfunc);
  uint8_t get_function();
  void set_cur_value(uint16_t newvalue);
  uint16_t get_cur_value();
  uint8_t get_cur_value_LSB();
  uint8_t get_cur_value_MSB();
  uint8_t getCheckSum(); // calcula el Checksum
  void setCheckSum();
  bool ChecksumOK();
  void leer(Stream *port);
  void enviar(Stream *port);
  void set_set();
  void set_request();
  bool getSet_Request();
private:
  uint16_t id_modulo; // el identificador de este modulo
  uint16_t id_usuario; // el usuario actualmente conectado a este modulo
  uint8_t cur_func; // nombre de la funcion actualmente
  uint16_t cur_value; // valor actual del dato
  uint8_t checksum;
  uint8_t set_or_req;  // 0 = Request;  255 = Set
  uint8_t t_func;

};




#endif
