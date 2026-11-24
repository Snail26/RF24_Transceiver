// /*
//     A simple library that allows the sending and receiving of strings using nrf24, based on Rowan Zhang's transceiver based on the RF24 GettingStarted.ino
//     Copyright (C) 2026  Connor Borrell

//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License version 2 as published by
//     the Free Software Foundation

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License along
//     with this program; if not, see <https://www.gnu.org/licenses/>.
// */

#include <SPI.h>
#include "RF24.h"

//Default pins for the Elven Aerospace CANSAT flight controller (stm32F4O5R)
#define CE_PIN PB4 
#define CS_PIN PB3

const byte DEFAULT_ADDRESS[5] = {'0', '0', '0', '0', '1'};
RF24 radio(CE_PIN, CS_PIN);

int radioSetup(const byte ADDRESS[5] = DEFAULT_ADDRESS) {
  // Start hardware SPI on the STM32 first
  SPI.begin();
  
  // Initialize the nRF24 module
  if (!radio.begin()) {
    return 1; // Returns failure if SPI communication with nRF24 chip fails
  }

  radio.setPALevel(RF24_PA_HIGH);
  radio.setPayloadSize(1);
  radio.openWritingPipe(ADDRESS);
  radio.openReadingPipe(1, ADDRESS);
  radio.startListening();

  return 0;
}

int sendCharRadio(char payload) {
  radio.stopListening();
  bool success = radio.write(&payload, sizeof(char));
  radio.startListening();
  return success ? 1 : 0;
}

char pollRadio() {
  char payload = 0;
  if (radio.available()) {
    radio.read(&payload, 1);
  }
  return payload;
}

void sendStringRadio(char strSend[], int chars) {
  for (int i = 0; i < chars; i++) {
    sendCharRadio(strSend[i]);
  }
}
