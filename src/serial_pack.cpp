#include <Arduino.h>
#include "serial_pack.hpp"

void Serial_Pack::set_id_modulo(uint16_t newid){
  id_modulo=newid;
};
uint16_t Serial_Pack::get_id_modulo(){
  return id_modulo;
};
void Serial_Pack::set_id_usuario(uint16_t newid){
  id_usuario=newid;
};
uint16_t Serial_Pack::get_id_usuario(){
    return id_usuario;
};
void Serial_Pack::set_function(uint8_t newfunc){
  cur_func=newfunc;
};
uint8_t Serial_Pack::get_function(){
  return cur_func;
};
void Serial_Pack::set_cur_value(uint16_t newvalue){
  cur_value=newvalue;
};
uint16_t Serial_Pack::get_cur_value(){
  return cur_value;
};
