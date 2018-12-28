
// Feather9x_TX
// -*- mode: C++ -*-

#include <SPI.h>
#include <RH_RF95.h>

/* Define anemometer pin8 */
#define ANEMOMETER A4

/* ESP32 feather w/wing */
#define RFM95_RST     27   // "A"
#define RFM95_CS      33   // "B"
#define RFM95_INT     12   //  next to A

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on Transmit
#define LED 13

void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(115200);

  delay(100);

  Serial.println("*********** WEATHER IS HAPPENING TX ***********");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void loop()
{
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  digitalWrite(LED, HIGH);
  Serial.println("Transmitting..."); // Send a message to rf95_server

  int windspeed = analogRead(ANEMOMETER); // get windspeed
  
  char radiopacket[10];
  itoa(windspeed, radiopacket, 10);
  radiopacket[9] = 0;
  
  Serial.print("Sending "); Serial.println(windspeed);
  
  Serial.println("Sending...");
  delay(10);
  rf95.send((uint8_t *)radiopacket, 10);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  Serial.println("sent");
  digitalWrite(LED, LOW);
}
