#include "HCS301.h"

#define TE 400

HCS301::HCS301(uint8_t RX_PIN, uint8_t TX_PIN) {

  _RX_PIN = RX_PIN;
  _TX_PIN = TX_PIN;
  pinMode(_RX_PIN, INPUT);
  pinMode(_TX_PIN, OUTPUT);
}

int HCS301::receive() {
  
  received = 0;
  
  //looking for preamble
  HCS_preamble_counter = 0;
  while (HCS_preamble_counter < 12) {
    HCS_last_change = micros();
    while (digitalRead(_RX_PIN) == LOW) {
    }
    if (HCS_last_change + 500  < micros()) {
        goto exit;                          //exit when pulses are too long
    }
    HCS_last_change = micros();
    while (digitalRead(_RX_PIN) == HIGH) {
    }
    if (HCS_last_change + 300 > micros()) {
        goto exit;                          //exit when pulses are too short
    }
    HCS_preamble_counter ++;
  }
  
  //looking for header
  HCS_last_change = micros();
  while (HCS_last_change + 3800 > micros()) {
    if (digitalRead(_RX_PIN) == HIGH) {
      goto exit;
    }
  }

  //read data
  HCS_bit_counter = 0;
  while (HCS_bit_counter < 66) {
    HCS_last_change = micros();
    while (digitalRead(_RX_PIN) == LOW) {    
    }
    if (HCS_last_change + 1000 < micros()) {
      goto exit;                          //exit when pulses are too long
    }
    HCS_last_change = micros();
    while (digitalRead(_RX_PIN) == HIGH) {    
    }
    if (HCS_last_change + 300 > micros()) {
      goto exit;                          //exit when pulses are too short
    }
    if (HCS_last_change + 600 > micros()) {
      HCS_bit_array[HCS_bit_counter] = 1; //write 1 to bitarray on short puls
    }
    else {
      HCS_bit_array[HCS_bit_counter] = 0; //write 0 to bitarray on long puls
    }
    HCS_bit_counter ++;
  }

  received = 1;

  //format data
  rollingCode = 0;
  serialNr = 0;
  message = 0;
  
  for (int i = 34; i<66; i++) {
    rollingCode = (rollingCode << 1) + HCS_bit_array[65-i];
  }
  for (int i = 6; i<34; i++) {
    serialNr = (serialNr << 1) + HCS_bit_array[65-i];
  }
  for (int i = 0; i<6; i++) {
    message = (message << 1) + HCS_bit_array[65-i];
  }
  exit:;
  return received;
}

void HCS301::transmit() {

  pinMode(_TX_PIN, OUTPUT);

  //preamble
  for (int i=0; i<12; i++) {
    delayMicroseconds(TE);
    digitalWrite(_TX_PIN, HIGH);
    delayMicroseconds(TE);
    digitalWrite(_TX_PIN, LOW);
  }
  
  //header
  delayMicroseconds(TE*10);
  
  //rollingCode
  for (int i=0; i<32; i++) {
    digitalWrite(_TX_PIN, HIGH);
    if (rollingCode & (1 << i)) {
      delayMicroseconds(TE);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE*2);
    }
    else {
      delayMicroseconds(TE*2);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE);
    }
  }

  //serialNr
  for (int i=0; i<28; i++) {
    digitalWrite(_TX_PIN, HIGH);
    if (serialNr & (1 << i)) {
      delayMicroseconds(TE);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE*2);
    }
    else {
      delayMicroseconds(TE*2);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE);
    }
  }
  
  //message
  for (int i=0; i<6; i++) {
    digitalWrite(_TX_PIN, HIGH);
    if (message & (1 << i)) {
      delayMicroseconds(TE);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE*2);
    }
    else {
      delayMicroseconds(TE*2);
      digitalWrite(_TX_PIN, LOW);
      delayMicroseconds(TE);
    }
  }
  
  //guard time
  delayMicroseconds(TE*30);  
}
