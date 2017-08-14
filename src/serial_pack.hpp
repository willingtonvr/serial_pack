/* Especificacion en README.md*/
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
#define N_FUNC_VOLT1  0xC1 // Voltaje 1
#define N_FUNC_VOLT2  0xC2 // Voltaje 2
#define N_FUNC_VOLT3  0xC3 // Voltaje 3
#define N_FUNC_VOLT4  0xC4 // Voltaje 4
#define N_FUNC_CORR1  0xD1 // Corriente 1
#define N_FUNC_CORR2  0xD2 // Corriente 2
#define N_FUNC_CORR3  0xD3 // Corriente 3
#define N_FUNC_CORR4  0xD4 // Corriente 4
#define N_FUNC_TIME1  0xE1 // Tiempo 1
#define N_FUNC_TIME2  0xE2 // Tiempo 1
#define N_FUNC_TIME3  0xE3 // Tiempo 1
#define N_FUNC_TIME4  0xE4 // Tiempo 1
#define N_FUNC_TEMP1 0xA1 // Sensor 1
#define N_FUNC_TEMP2 0xA2 // Sensor 2
#define N_FUNC_TEMP3 0xA3 // Sensor 3
#define N_FUNC_TEMP4 0xA4 // Sensor 4
#define N_FUNC_LED1  0xA8 // Led 1
#define N_FUNC_LED2  0xA9 // led 2
#define N_FUNC_LED3  0xAA // Led 3
#define N_FUNC_LED4  0xAB // led 4
#define N_FUNC_NEO   0xAC // led NEOPIXEL
#define N_FUNC_NEO_BLINK5 0xAD
#define N_FUNC_NEO_BLINK10 0xAE
#define N_FUNC_MOD   0xB0 // Id modulo
#define N_FUNC_USR   0xA1 // Id usuario
#define N_FUNC_BAT   0xD0 // tipo bateria
#define N_FUNC_BCAST_ENABLE   0xF1 // tipo bateria
// AGREAMOS FUNCIONES TIPO CNC
// EN GENERAL CADA EJE PUEDE EJECUTAR 4 FUNCIONES DISTINTAS

#define CNC_AX0_F01 0x10
#define CNC_AX0_F02 0x11
#define CNC_AX0_F03 0x12
#define CNC_AX0_F04 0x13

#define CNC_AX1_F01 0x14
#define CNC_AX1_F02 0x15
#define CNC_AX1_F03 0x16
#define CNC_AX1_F04 0x17

#define CNC_AX2_F01 0x18
#define CNC_AX2_F02 0x19
#define CNC_AX2_F03 0x1A
#define CNC_AX2_F04 0x1B

#define CNC_AX3_F01 0x1C
#define CNC_AX3_F02 0x1D
#define CNC_AX3_F03 0x1E
#define CNC_AX3_F04 0x1F

#define CNC_AX4_F01 0x20
#define CNC_AX4_F02 0x21
#define CNC_AX4_F03 0x22
#define CNC_AX4_F04 0x23

#define CNC_AX5_F01 0x24
#define CNC_AX5_F02 0x25
#define CNC_AX5_F03 0x26
#define CNC_AX5_F04 0x27

#define CNC_AX6_F01 0x28
#define CNC_AX6_F02 0x29
#define CNC_AX6_F03 0x2A
#define CNC_AX6_F04 0x2B

typedef union {
 float float_valor;
 uint32_t uint32val;
 uint8_t binary[4];
} binary4;


class Serial_Pack{
public:
  Serial_Pack(uint16_t id_modulo);
  void set_id_modulo(uint16_t newid);
  uint16_t get_id_modulo();
  void set_id_usuario(uint16_t newid);
  uint16_t get_id_usuario();
  void set_function(uint8_t newfunc);
  uint8_t get_function();
  void set_cur_value(uint32_t newvalue);
  void set_cur_value(binary4 newvalue);
  void set_cur_value(float newvalue);
  uint32_t get_cur_value32();
  float get_cur_valuef();
  uint8_t get_cur_value_LSB_L();
  uint8_t get_cur_value_LSB_H();
  uint8_t get_cur_value_MSB_L();
  uint8_t get_cur_value_MSB_H();
  uint8_t getCheckSum(); // calcula el Checksum
  void setCheckSum();
  bool ChecksumOK();
  void leer(Stream *port);
  void enviar(Stream *port);
  void set_set();
  void set_request();
  bool getSet_Request();
  void set_t_func(uint8_t new_t_func);
  uint8_t get_t_func();

private:
  uint16_t id_modulo; // el identificador de este modulo
  uint16_t id_usuario; // el usuario actualmente conectado a este modulo
  uint8_t cur_func; // nombre de la funcion actualmente
  binary4 cur_value; // valor actual del dato
  uint8_t checksum;
  uint8_t set_or_req;  // 0 = Request;  255 = Set
  uint8_t t_func;

};




#endif
