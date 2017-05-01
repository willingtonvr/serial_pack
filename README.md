## serial_pack
* Protocolo de comunicación para el cargador

# Hardware:

* La comunicación sera establecida por puerto serie

comunicación entre el master y el sclavo de los mensajes desde el arduino hacia el raspberry o viceversa

* voltaje en la batería
* corriente en la batería
* sensor de presencia activado
* sensor de presencia desactivado
* id de usuario
* id de modulo
* tipo de batería conectada
* settimer iniciar temporizador para calculo de energía / potencia
* readtimer lector de temporizador
* setleds leds de indicación al Usuario, elementos para indicar al usuario
* el paquete de datos estará conformado de la siguiente forma, paquete de longitud fija 9 bytes

+  **INCIO del frame**: 0X7B = '{'
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
* 	Sensor1		: 0xA4
*	Sensor2		: 0xA5
*	Sensor3		: 0xA5
*	Sensor4		: 0xA7
*	LED1		: 0xA8
*	LED2		: 0xA9
* 	LED3		: 0xAA
*	LED3		: 0xAB
* 	id modulo   : 0xB0
* 	id usuario	: 0xB1
* 	tipo bateria: 0xD0
+ **Valor de la funcion (2 bytes)**:
*	MSB byte
*	LSB byte
+ **Checksum: 255 menos la suma de los bytes todos los valores a partir del encabezado.**
+ **FIN PAQUETE** :0x7D = '}'
