// rf95_reliable_datagram_post.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliabledatagram class, using the RH_RF95 driver to control a RF95 radio.
// It is designed to work with the other example rf95_reliable_datagram_client
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with the RFM95W

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define CLIENT_ADDRESS 1 //give address
#define TRAIN_ADDRESS 2 //give address

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

// Singleton instance of the radio driver
//RH_RF95 driver;
RH_RF95 driver(RFM95_CS, RFM95_INT); // select 32u4

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, TRAIN_ADDRESS);

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

void setup()
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
  //  pinMode(4, OUTPUT);
  //  digitalWrite(4, HIGH);

  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!manager.init())
    Serial.println("init failed");
  driver.setFrequency(868.0);
  driver.setTxPower(20, false);
  // If you are using Modtronix inAir4 or inAir9, or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for 0 to 15 dBm and with useRFO true.
  // Failure to do that will result in extremely low transmit power
  // the CAD timeout to non-zero:
  //  driver.setCADTimeout(10000);
}

uint8_t data[] = "Got signal";
uint8_t signal_code[] = "RYYG";

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println("Sending to rf95_reliable_datagram_server");

  // Send a message to manager_server
  int t1 = millis();
  if (manager.sendtoWait(data, sizeof(data), TRAIN_ADDRESS))
  {
    // Now wait for a reply from the server
    Serial.print("Sent:");
    Serial.println((char *)data);
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
      if (strcmp(buf, "yes")) {
        if (manager.sendtoWait(signal_code, sizeof(signal_code), TRAIN_ADDRESS))
        {
          // Now wait for a reply from the server
          if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
          {
            Serial.print("got reply from : 0x");
            Serial.print(from, HEX);
            Serial.print(": ");
            Serial.println((char*)buf);
            if (strcmp(buf, signal_code)) {
              Serial.println("Successfully sent!");
            }
            else Serial.println("Error!!");

          }
        }
        else
        {
          Serial.println("No reply, the train is nowhere near!");
        }
      }
      else
        Serial.println("sendtoWait failed");
      delay(500);
    }
  }
  int t2 = millis();
  Serial.println(t2 - t1);
}
