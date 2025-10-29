/* v202_rx.ino -- An arduino sketch to test the protocol v202
 *
 * Copyright (C) 2016 execuc
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <SPI.h>
#include "v202_protocol.h"

nrf24l01p wireless; 
v202Protocol protocol;

unsigned long time = 0;
 
// La rutina setup funciona una sola vez cuando presionas el boton reset:
void setup() {
  // SS pin debe ser configurado como salida para configurar el spi como maestro !
  pinMode(SS, OUTPUT);
  Serial.begin(115200);
  // Configurar el pin CS al D7 y el pin CE al D8
  wireless.setPins(8,7);
  protocol.init(&wireless);
  
  time = micros();
  Serial.println("Start");
}

rx_values_t rxValues;

bool bind_in_progress = false;
unsigned long newTime;

void loop() 
{
  time = micros();
  uint8_t value = protocol.run(&rxValues); 
  newTime = micros();
   
  switch( value )
  {
    case  BIND_IN_PROGRESS:
      if(!bind_in_progress)
      {
        bind_in_progress = true;
        Serial.println("Bind in progress");
      }
    break;
    
    case BOUND_NEW_VALUES:
      //newTime = micros();
      Serial.print(newTime - time); //120 ms for 16 Mhz
      Serial.print(" :\t");Serial.print(rxValues.throttle);
      Serial.print("\t"); Serial.print(rxValues.yaw);
      Serial.print("\t"); Serial.print(rxValues.pitch);
      Serial.print("\t"); Serial.print(rxValues.roll);
      Serial.print("\t"); Serial.print(rxValues.trim_yaw);
      Serial.print("\t"); Serial.print(rxValues.trim_pitch);
      Serial.print("\t"); Serial.print(rxValues.trim_roll);
      Serial.print("\t"); Serial.print(rxValues.flags);
      Serial.print("\t"); Serial.println(rxValues.crc);
      //time = newTime;
    break;
    
    case BOUND_NO_VALUES:
      //Serial.print(newTime - time); Serial.println(" : ----"); // 32ms for 16Mhz
    break;

    case ERROR_SIGNAL_LOST:
	//Serial.print(newTime - time); Serial.println(" : ----")
    break;
    
    default:
    break;
  }
  delay(2);
}
