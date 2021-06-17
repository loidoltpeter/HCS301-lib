#ifndef HCS301_h
#define HCS301_h

#include "Arduino.h"

class HCS301 {

  public:
  
    HCS301(uint8_t RX_PIN, uint8_t TX_PIN);
    int receive(); //returns 1 if message is received
    void transmit();

    uint32_t rollingCode;
    uint32_t serialNr;
    uint8_t message;

  private:

    uint8_t _RX_PIN;
    uint8_t _TX_PIN;
    
    uint32_t HCS_last_change;
    uint8_t HCS_preamble_counter;
    uint8_t HCS_bit_counter;
    uint8_t HCS_bit_array[66];
    uint8_t received;
};
    
#endif
