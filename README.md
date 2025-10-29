Receptor v202
==========

Descripción
--------------------

Este código decodifica tramas de los transmisores v202/v222/v262/v282... con un Arduino y un chip nrf24L01.

Hardware
--------------------

Para probarlo, utilizo un Arduino DIP sencillo en una placa de prototipos y un chip nrf24l01+. Sin embargo, un Arduino UNO u otro con el mismo chip también sirve. Conecta SCK, MISO y MOSI a los pines D13, D12 y D11, respectivamente. Luego, conecta CE y CS a los pines digitales que hayas definido en el código (consulta el método `wireless.setPins()` más abajo). Finalmente, conecta VCC y GND del nrf24l01 a los pines de 3.3V y GND del Arduino, recomiendo el uso del Arduino Mega par mas velocidad en la recepción de los datos.

Modo de uso
--------------------

Existen dos clases:

  * nrf24l01p: gestiona el protocolo SPI para comunicarse con el chip nrf24l01p.

  * v202_protocol: gestiona el protocolo v2xx.

v202_rx.ino es un ejemplo que muestra cómo usar estas clases.

Se instancian las siguientes clases:

    nrf24l01p wireless;
    v202Protocol protocol;

Los pines del nrf24l01 deben definirse en la función de configuración de Arduino. Los argumentos del método `setPins` definen los pines CE (habilitación del chip) y CS (selección de chip SPI) del nrf24L01 en este orden. En este ejemplo, no se utiliza el pin SS de Arduino como CS, sino el D7. Sin embargo, el pin SS debe configurarse como salida para activar el modo SPI maestro.

    void setup() {
        // SS pin must be set as output to set SPI to master !
        pinMode(SS, OUTPUT);
        Serial.begin(115200);
        // Set CS pin to D7 and CE pin to D8
        wireless.setPins(8,7);
        protocol.init(&wireless);
        ...
    }

El encapsulador SPI (clase nrf24l01p) se vincula al protocolo en la función de configuración:

    protocol.init(&wireless);

En la función loop, el método v202Protocol Run() debe llamarse como máximo cada 4 ms con la estructura `rx_values_t`.

    uint8_t value = protocol.run(&rxValues);

Esta función tiene varios tipos de valores devueltos por una enumeración:

    enum rxReturn
     {
        BOUND_NEW_VALUES = 0,   // Estado conectado, trama recibida con nuevos valores TX
        BOUND_NO_VALUES,        // Estado conectado, ninguna trama recivida
        NOT_BOUND,              // No conectado, estado inicial
        BIND_IN_PROGRESS,       // Enlace en progreso, Se ha recibido la primera trama con el ID TX, espera sin trama de datos.
        ERROR_SIGNAL_LOST,      // Señal perdida
        UNKNOWN                 // No usado por el momento
     };

Cuando se recibe una trama (`BOUND_NEW_VALUES`), se puede leer la estructura `rx_values_t`:

    typedef struct __attribute__((__packed__)) {
         uint8_t throttle;
         int8_t yaw;
         int8_t pitch;
         int8_t roll;
         int8_t trim_yaw;
         int8_t trim_pitch;
         int8_t trim_roll;
         uint8_t flags;
       } rx_values_t;

Dispone de cuatro ejes, tres valores de ajuste y banderas. El último valor depende de los botones pulsados ​​en el transmisor.

