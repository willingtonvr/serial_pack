## serial_pack
* Protocolo de comunicación para el cargador

# Hardware:

* La comunicación sera establecida por puerto serie

comunicación entre el master y el sclavo de los mensajes desde el arduino hacia el raspberry o viceversa

* voltaje en realidad estas variables trasmiten el valor del ADC al computador.
* corriente
* sensor de presencia activado
* sensor de presencia desactivado
* id de usuario
* id de modulo
* tipo de batería conectada
* settimer iniciar temporizador para calculo de energía / potencia
* readtimer lector de temporizador
* setleds leds de indicación al Usuario, elementos para indicar al usuario
* el paquete de datos estará conformado de la siguiente forma, paquete de longitud fija 11 bytes

+  **INCIO del frame**: 0X7B = '{'
+ **id modulo (2 bytes)**:
* 	MSB id modulo
* 	LSB id modulo
+ **Tipo de Funcion**:
*	Request/set	: 0x53  = 'S'
*	Response	: 0x52  = 'R'
*	Ack			: 0x4B  = 'K'
+ **Nombre de Funcion**:
*	Voltaje1 	: 0xC1
*	Voltaje2 	: 0xC2
*	Voltaje3 	: 0xC3
*	Voltaje4 	: 0xC4
*	Corriente1: 0xD1
*	Corriente2: 0xD2
*	Corriente3: 0xD3
*	Corriente4: 0xD4
*	Tiempo1		: 0xE1
*	Tiempo2		: 0xE2
*	Tiempo3		: 0xE3
*	Tiempo4		: 0xE4
* Temp 1		: 0xA1
*	Temp 2		: 0xA2
*	Temp 3		: 0xA3
*	Temp 4		: 0xA4
*	LED1		  : 0xA8
*	LED2		  : 0xA9
* LED3		  : 0xAA
*	LED3      : 0xAB
* 	id modulo   : 0xB0
* 	id usuario	: 0xB1
* 	tipo bateria: 0xE0
+ **Valor de la funcion (4 bytes)**:
*	MSB byte
*	...
* ...
* LSB byte
+ **Checksum: 255 menos la suma de los bytes todos los valores a partir del encabezado.**
+ **FIN PAQUETE** :0x7D = '}'
