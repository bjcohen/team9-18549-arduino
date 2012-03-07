#include <SoftwareSerial.h>

#define ACK 0x6
#define NAK 0x15

#define RX 10
#define TX 9
#define RES 13

SoftwareSerial tft(RX, TX);

void setup() {
  // reset screen
  pinMode(RES, OUTPUT);
  digitalWrite(RES, LOW);
  delayMicroseconds(3); // 2 us required, so do 3
  digitalWrite(RES, HIGH);
  
  // initialize serial connection
  tft.begin(9600);

  // wait 500ms as instructed by documentation
  delay(500);
    tft.listen();
  
  // send auto-baud
  tft.write(0x55);  
  while(tft.available() == 0) { delay(1); }
  char r = tft.read();
  
  // draw a red circle
  tft.write(0x43); // draw a circle
  tft.write((byte)0); // x = 63
  tft.write(0x3f);
  tft.write((byte)0); // y = 63
  tft.write(0x3f);
  tft.write((byte)0); // r = 34
  tft.write(0x22);
  tft.write((byte)0); // color = red
  tft.write(0x1f);
  
  while(tft.available() == 0) { delay(1); }
  r = tft.read();
  
  // draw another red circle
  tft.write(0x43); // draw a circle
  tft.write((byte)0); // x = 63
  tft.write(0x7f);
  tft.write((byte)0); // y = 63
  tft.write(0x7f);
  tft.write((byte)0); // r = 34
  tft.write(0x22);
  tft.write((byte)0); // color = red
  tft.write(0x1f);
  
  while(tft.available() == 0) { delay(1); }
  r = tft.read();
  
}

void loop() {
}

