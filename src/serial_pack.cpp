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
void Serial_Pack::set_cur_value(uint32_t newvalue){
  cur_value.uint32val =newvalue;
};
void Serial_Pack::set_cur_value(binary4 newvalue){
  cur_value =newvalue;
};
void Serial_Pack::set_cur_value(float newvalue){
  cur_value.float_valor =newvalue;
};
uint32_t Serial_Pack::get_cur_value32(){
  return cur_value.uint32val;
};
float Serial_Pack::get_cur_valuef(){
  return cur_value.float_valor;
};
uint8_t Serial_Pack::get_cur_value_LSB_L(){
  return (uint8_t) (cur_value.binary[0]);
};
uint8_t Serial_Pack::get_cur_value_LSB_H(){
  return (uint8_t) (cur_value.binary[1]);
};
uint8_t Serial_Pack::get_cur_value_MSB_L(){
  return (uint8_t) (cur_value.binary[2]) ;
};
uint8_t Serial_Pack::get_cur_value_MSB_H(){
  return (uint8_t) (cur_value.binary[3]) ;
};
uint8_t Serial_Pack::getCheckSum() {// calcula el Checksum
  uint16_t tchk=0;
  uint8_t MSB_id=0;
  uint8_t LSB_id=0;
  // Curv es de 32 bits
  uint8_t MSB_curv_H=0;
  uint8_t MSB_curv_L=0;
  uint8_t LSB_curv_H=0;
  uint8_t LSB_curv_L=0;
  MSB_id =  id_modulo >>8;

  LSB_id = id_modulo & 0x00FF;

  MSB_curv_H =  cur_value.binary[3];
  MSB_curv_L =  cur_value.binary[2];
  LSB_curv_H =  cur_value.binary[1];
  LSB_curv_L =  cur_value.binary[0];

  tchk =MSB_id +
        LSB_id +
        t_func +
        cur_func +
        MSB_curv_H +
        MSB_curv_L +
        LSB_curv_H +
        LSB_curv_L;

  return byte(0xFF - ( tchk & 0x00FF ));
  //return byte( tchk & 0x00FF );
};
//constructor
Serial_Pack::Serial_Pack(uint16_t id_modulo){
this->id_modulo=id_modulo;
t_func=T_FUNC_SET;

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

  if (port->available()>10){

    dato=port->read();
    //port->print(dato,HEX);
    if (dato==START_PACK) {  // verificar si es {
       dato=port->read();
       //port->print(dato,HEX);
       tmpmsb=dato;

       dato=port->read();
       //port->print(dato,HEX);

       tmplsb=dato;
       id_modulo= (int) tmpmsb <<8 | (int) tmplsb;

       dato=port->read();
       //port->print(dato,HEX);
       t_func=dato;

       if (t_func==T_FUNC_SET){
         set_or_req=255;
       };
       if (t_func==T_FUNC_REQ){
         set_or_req=0;
       };

       dato=port->read();
       //port->print(dato,HEX);
       cur_func=dato;
       // 4 bytes con el valor

       dato=port->read();
       cur_value.binary[3]=dato;
       dato=port->read();
       cur_value.binary[2]=dato;
       dato=port->read();
       cur_value.binary[1]=dato;
       dato=port->read();
       cur_value.binary[0]=dato;


       dato=port->read();
       //port->print(dato,HEX);
       checksum=dato;

       dato=port->read(); // este deberia ser el fin '}'
       //port->print(dato,HEX);
      #ifdef DEBUG
        byte cal_checksum;
        cal_checksum=getCheckSum();
        port->println("---------------");
        port->println(dato,HEX);
        port->print("id_modulo =");
        port->println(id_modulo);
        port->print("S o R =");
        port->println(t_func);
        port->print("nombre func =");
        port->println(cur_func);

        port->print("valor func =");
        port->println(cur_value);
        port->print("checksum = ");
        port->println(checksum);
        port->print("checksum calculado = ");
        port->println(cal_checksum);
       #endif

    }
} // fin available
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
void Serial_Pack::set_t_func(uint8_t new_t_func){
  t_func=new_t_func;
};
uint8_t Serial_Pack::get_t_func(){
  return t_func;
};
void Serial_Pack::enviar(Stream *port){
  uint8_t MSB_id=0;
  uint8_t LSB_id=0;

  MSB_id =  id_modulo >>8;
  LSB_id = id_modulo & 0x00FF;

  checksum=getCheckSum();

  port->write((uint8_t) START_PACK);
  port->write((uint8_t) MSB_id);
  port->write((uint8_t) LSB_id);
  //t_func=T_FUNC_SET;
  port->write((uint8_t) t_func);
  port->write((uint8_t) cur_func);
  port->write((uint8_t) cur_value.binary[3]);
  port->write((uint8_t) cur_value.binary[2]);
  port->write((uint8_t) cur_value.binary[1]);
  port->write((uint8_t) cur_value.binary[0]);

  port->write((uint8_t) checksum);
  port->write((uint8_t) END_PACK);
  port->flush();
}
