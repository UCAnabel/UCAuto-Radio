# UCAuto-Radio
El equipo UCAuto RadioTeam es el responsable del apartado de comunicaciones dentro del proyecto UCAuto Sailing Team. 
Para la comunicación entre el velero y el centro de control se ha determinado utilizar un sistema basado en LoRa (Long Range).
# LoRa LilyGo
Se trata de una placa de desarrollo que integra en un solo módulo varios componentes orientados a aplicaciones de comunicaciones e IoT. Incorpora un microcontrolador ESP32, que incluye conectividad WiFi y Bluetooth, una pantalla OLED, un receptor GPS NEO-6M y un módulo LoRa de 868 MHz.

<img width="699" height="607" alt="image" src="https://github.com/user-attachments/assets/4ed13b38-43e2-4001-9ead-83269702cf69" />

En la parte posterior dispone de un conector para baterías LiPo de 3,7 V tipo 18650, lo que la hace adecuada para proyectos portátiles.
El ESP32 es un microcontrolador ampliamente documentado y con soporte en plataformas como Arduino, lo que facilita su programación y uso en proyectos experimentales o de investigación.
ESP32

•	Microcontrolador ESP32 REV1

•	WIFI

•	Bluetooth

•	8MB PSRAM

•	4MB Flash

•	3D antena

LORA

•	Voltaje de funcionamiento: 1,8 ~ 3,7 v

•	Corriente aceptable: 10 ~ 14mA

•	Corriente de transmisión: 120mA @ + 20dBm

•	90mA @ + 17dBm

•	29mA @ + 13dBm

•	Frecuencia de funcionamiento: Ajustable de 868MHZ a 915MHz

•	Potencia de transmisión: + 20dBm

•	Sensibilidad de recepción:-139dBm @ LoRa & 62,5 Khz & SF = 12 & 146bps

•	-136dBm @ LoRa y 125Khz y SF = 12 y 293bps

•	-118dBm @ LoRa y 125Khz y SF = 6 y 9380bps

•	-123dBm @ FSK y 5Khz y 1,2 Kbps

•	Error de frecuencia: +/-15KHz

•	Espacio FIFO: 64 bytes

•	Tasa de datos: 1,2 K ~ 300Kbps @ FSK

•	0.018K ~ 37,5 Kbps @ LoRa

•	Modo de modulación: FSK,GFSK MSK… el sistema GMSK LoRa TM OOK

•	Interfaz forma: SPI

•	Corriente de sueño: 0.2uA @ Sleep

•	1.5uA @ IDLE

•	Temperatura de funcionamiento:-40 ℃- + 85 ℃

•	Digital RSSI función

•	De Frecuencia automático de corrección

•	Control automático de ganancia

•	Sensor de temperatura y detección de bajo voltaje

•	Activación rápida y salto de frecuencia

•	Controlador de paquetes de datos altamente configurable

GPS

•	Módulos GPS NEO-6M, fuente de alimentación Universal de 3V-5V

•	Módulo con antena de cerámica, super señal

•	Guardar los datos de parámetros de configuración EEPROM Down

•	Con los datos de copia de seguridad de la batería

•	Hay indicador de señal LED

•	Tasa de baudios predeterminada: 9600

Especificaciones:

•	Cargador LiPo integrado TP5400 (1A)

•	Indicador LED: hay 4 ledes que muestran el estado de la batería en tiempo real.

•	USB

•	CP2104-GMR

•	Pre-Flash del Firmware

•	SoftRF V4.0

•	Soporte para batería 18650

•	Dimensiones: 100.13 × 32.89 milímetros

Documentación:

https://github.com/lyusupov/SoftRF/wiki/Prime-Edition-MkII

https://github.com/LilyGO/SoftRF

# Arquitectura del sistema
Definición del sistema de comunicación:
El sistema de comunicación se concibe con dos nodos principales:

1.	Nodo móvil, ubicado a bordo del velero autónomo.
2.	Nodo fijo, que puede estar en tierra o en una embarcación de apoyo, desde donde se realizará el monitoreo del velero y, en caso necesario, su control.

El velero podrá operar en dos modos de control:

1.	Modo autónomo, en el que el propio velero gestiona su navegación.
2.	Modo manual, en el que el nodo fijo puede enviar comandos para tomar el control.

Para soportar estos modos de operación, se han identificado los siguientes tipos de mensajes:
1.	Mensaje de selección de modo: enviado desde el nodo fijo al nodo móvil, permite establecer si el velero funcionará en modo autónomo o manual.
2.	Mensajes de telemetría (móvil → fijo): generados en ambos modos, transmiten información del velero al centro de control (estado, sensores, posición, etc.).
3.	Mensajes de control (fijo → móvil): se utilizan únicamente en el modo manual para transmitir órdenes directas al velero.
4.	Mensaje de consulta de modo (adicional recomendado): enviado desde el nodo fijo al nodo móvil para confirmar en qué modo se encuentra el sistema. Este mensaje, además de permitir verificar y sincronizar el estado de control, constituye una forma sencilla de comprobar que la comunicación está activa.

Con estos cuatro tipos de mensajes se cubren los requisitos básicos de comunicación y control, garantizando tanto la supervisión del sistema como la posibilidad de intervención remota cuando sea necesario.

<img width="1004" height="1284" alt="image" src="https://github.com/user-attachments/assets/94c0492d-f846-45bc-a031-4edc6f5004c7" />
<img width="1004" height="812" alt="image" src="https://github.com/user-attachments/assets/466a8e83-9a8f-4914-97d2-f5f6683644f4" />


# Formato mensaje
El mensaje del Nodo móvil al Nodo fijo es indispensable determinar y optimizarlo, para poder enviar todos los datos mínimos y de manera eficaz para seguir el monitoreo del Nodo móvil. Los mensajes de comando son mas cortos y no es crítico el tamaño (en principio).

Se ha determinado utilizar el formato JSON para enviar los datos y para que este envío de datos sea lo mas eficaz posible se va a determinar una especie de codificación por posición, es decir vamos a crear un mensaje estructurado donde siempre el dato que este en la posición x sea el mismo tipo y también es critico la longitud (decimales) de cada dato de los sensores. De modo que
|Campo	|Descripción	|Decimales	Máx. |valor típico	|Ejemplo de valor	|Nº caracteres|
|---|---|---|---|---|---|
|lat	|Latitud GPS	|5	|89.99999	|37.12345	|8|
|lon	|Longitud GPS	|5	|-179.99999	|-3.65432|10|
|spd	|Velocidad (m/s)	|1	|999.9	|12.3	|5|
|hdg	|Rumbo (°)	|1	|359.9	|123.4	|5|
|w_dir	|Dirección del viento (°)|0	|359	|270	|3|
|w_spd	|Velocidad del viento (m/s)	|1	|99.9	|5.2	|4|
|acc	|Aceleración X/Y/Z (g)	|3	|9.999	|0.980	|5|
|gyro	|Vel. angular X/Y/Z (°/s)	|2	|99.99	|-0.05	|5|
|mag	|Magnetómetro X/Y/Z (µT)	|1	|999.9	|40.2	|5|
|servo	|Posición de 2 servos (°)	|0	|180	|120	|3|

Ejemplo mensaje JSON (con indicadores):

{"ts":1729950200,"la":37.12345,"lo":-3.65432,"sp":2.1,"hd":123.4,"wd":270,"ws":5.2,"ac":[0.012,-0.023,0.98],"gy":[0.1,-0.05,0.0],"mg":[-30.1,14.0,40.2],"sv":[75,120]}

166 bytes / 861 ms SF9 muy justo

Ejemplo mensaje JSON (sin indicadores):

{1729950200,37.12345,-3.65432,2.1,123.4,270,5.2,0.012,-0.023,0.98,0.1,-0.05,0.0,-30.1,14.0,40.2,75,120}

103 bytes / 574.46 ms SF9 ok

calculadora utilizada:

https://www.semtech.com/search?crawler_semtech%5Bquery%5D=AN1200.13
