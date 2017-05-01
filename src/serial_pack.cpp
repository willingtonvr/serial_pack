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
uint8_t Serial_Pack::get_cur_value_LSB(){
  return (uint8_t) (cur_value & 0x00FF);
};
uint8_t Serial_Pack::get_cur_value_MSB(){
  return (uint8_t) (cur_value >> 8) ;
};
uint8_t Serial_Pack::getCheckSum() {// calcula el Checksum
  uint8_t tchk=0;
  uint8_t MSB_id;
  uint8_t LSB_id;

  uint8_t MSB_curv;
  uint8_t LSB_curv;
  MSB_id = (uint8_t) id_modulo >>8;

  LSB_id = (uint8_t) id_modulo & 0x00FF;

  MSB_curv = (uint8_t) cur_value >>8;
  LSB_curv = (uint8_t) cur_value & 0x00FF;

  tchk = MSB_id + LSB_id + cur_func + MSB_curv + LSB_curv;

  return (0xFF - tchk);
};
//constructor
Serial_Pack::Serial_Pack(uint16_t id_modulo){
this->id_modulo=id_modulo;

};

void Serial_Pack::setCheckSum(){
  checksum = getCheckSum();
};

bool Serial_Pack::ChecksumOK(){
  byte tmpchek;
  tmpchek=getCheckSum();

  return (checksum == tmpchek);
};
void Serial_Pack::leer(Stream *port){

  char dato;
  byte tmpmsb;
  byte tmplsb;
  if (port->available()>8)

    dato=port->read();
    port->write(dato);
  if (dato==START_PACK) {  // verificar si es {
     dato=port->read();
     port->write(dato);
     tmpmsb=dato;

     dato=port->read();
     port->write(dato);

     tmplsb=dato;
     id_modulo= (int) tmpmsb <<8 | (int) tmplsb;

     dato=port->read();
     port->write(dato);

     if (dato==T_FUNC_SET){
       set_or_req=255;
     };
     if (dato==T_FUNC_REQ){
       set_or_req=0;
     };

     dato=port->read();
     port->write(dato);

     cur_func=dato;

     dato=port->read();
     port->write(dato);

     tmpmsb=dato;

     dato=port->read();
     port->write(dato);

     tmplsb=dato;
     cur_value = (int) tmpmsb <<8 | (int) tmplsb;

     dato=port->read();
     port->write(dato);

     checksum=dato;

     dato=port->read(); // este deberia ser el fin '}'
     port->write(dato);
     dato=getCheckSum();
     port->print("checksum = ");
     port->println(dato);
     port->println(dato,DEC);
     port->print("id_modulo =");
     port->println(id_modulo);
  }

};

void Serial_Pack::set_set(){
  set_or_req=255;
};
void Serial_Pack::set_request(){
  set_or_req=0;
};
bool Serial_Pack::getSet_Request(){
  return (bool) set_or_req;
};
