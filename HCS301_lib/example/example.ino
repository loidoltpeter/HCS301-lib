// This simple example looks for an HCS301 message on the RX_PIN and then repeates it over the TX_PIN.
// It should work with every Arduino compatible microcontroller on almost every pin.

#include <HCS301.h>

#define RX_PIN 11 //receiving pin
#define TX_PIN 10 //transmitting pin

HCS301 radio1(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Setup OK");
  
  while(!radio1.receive()) {
   //loop until a message is received
  }
  //the following variables store the message for receiving and transmitting
  Serial.println(radio1.rollingCode);
  Serial.println(radio1.serialNr);
  Serial.println(radio1.message);
}

void loop() {
  //repeats received message every second
  Serial.println("transmitting");
  radio1.transmit();
  delay(1000);
}
