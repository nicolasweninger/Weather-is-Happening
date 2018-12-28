// Feather9x_RX
// -*- mode: C++ -*-
 
#include <SPI.h>
#include <RH_RF95.h>
 

/* ESP32 feather w/wing */
#define RFM95_RST     27   // "A"
#define RFM95_CS      33   // "B"
#define RFM95_INT     12   //  next to A

#define BUZZER        15 
#define BEACON        32

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 13

// Threstholds
#define FIRSTTHRESH   500
#define SECONDTHRESH  650
 
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BEACON, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(BEACON, LOW);
  digitalWrite(RFM95_RST, HIGH);
 
  Serial.begin(115200);

  delay(100);
 
  Serial.println("*********** WEATHER IS HAPPENING RX ***********");
 
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
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
 
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);

      buf[4] = 0;
      int windspeed = atoi((char*)buf); 
      Serial.println(windspeed);
      
      delay(100);

      // Logic to set outputs
      if(windspeed > FIRSTTHRESH && windspeed < SECONDTHRESH){
        soundBuzzer(1, 6);
      }
      else if(windspeed > SECONDTHRESH){
        soundBeacon(1, 6);
      }
      
      digitalWrite(LED, LOW);
    }
  }
  else{
    digitalWrite(LED, LOW);
  }
}

void soundBuzzer(int duration, int freq){
  Serial.print("sound buzzer!");
  unsigned long startTime = millis();
  while(millis() < startTime + duration*1000){
    digitalWrite(BUZZER, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1/freq);                       // wait for a second
    digitalWrite(BUZZER, LOW);    // turn the LED off by making the voltage LOW
    delay(1/freq);
  }
  return;  
}

void soundBeacon(int duration, int freq){
  Serial.print("sound beacon!");
  digitalWrite(BEACON, HIGH);
  unsigned long startTime = millis();
  while(millis() < startTime + duration*1000){
    digitalWrite(BUZZER, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1/freq);                       // wait for a second
    digitalWrite(BUZZER, LOW);    // turn the LED off by making the voltage LOW
    delay(1/freq);
  }
  digitalWrite(BEACON, LOW);
  return;    
}
