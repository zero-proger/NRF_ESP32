#include <SPI.h>

#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

uint64_t counter1 = millis();
uint64_t counter2 = millis();

#define _delayS(_counter, _delay) if ((millis() - _counter > _delay) ? true : false) // DELAY START
#define _delayR(_counter) (_counter = millis())                                      // DELAY RESET

#define tx // rx | tx
/*
MISO 19
MOSI 18
SCK 5
*/

RF24 radio(2, 4); // CE, CSN 

const uint64_t address = 0xF0F0F0F0E1LL;
int counter = 0;

struct MyData
{
  uint8_t counter;
};
MyData data;

#ifdef tx /* Transiver */
void setup(void)
{
  Serial.begin(9600);
  radio.begin();                  // Starting the Wireless communication
  radio.openWritingPipe(address); // Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  // You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          // This sets the module as transmitter
  radio.printDetails();
}
void loop(void)
{
  _delayS(counter1, 1000)
  {
    radio.write(&data, sizeof(MyData));

    Serial.println("Data Packet Sent");
    Serial.println(data.counter);
    Serial.println("");

    data.counter++;
    _delayR(counter1);
  }

  _delayS(counter2, 10000)
  {
    radio.printDetails();
    _delayR(counter2);
  }
}
#endif

#ifdef rx /* Resiver */
void setup(void)
{
  Serial.begin(9600);
  radio.begin();                     // Starting the Wireless communication
  radio.openReadingPipe(1, address); // Setting the address where we will read the data
  radio.setPALevel(RF24_PA_MIN);     // You can set it as minimum
  radio.startListening();
  radio.printDetails();
}
void loop(void)
{
  if (radio.available())
  {
    radio.read(&data, sizeof(MyData));
  }

  _delayS(counter1, 500);
  {
    Serial.print("Received packet with data: ");
    Serial.print(data.counter);
    Serial.println("");
    _delayR(counter1);
  }
  _delayS(counter2, 10000);
  {
    //radio.printDetails();
    _delayR(counter2);
  }
}
#endif

#ifndef rx /* Validator */
#ifndef tx
#error "Please define either rx or tx"
#endif
#endif