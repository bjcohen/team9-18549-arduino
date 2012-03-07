#include <SoftwareSerial.h>

#define ACK 0x6
#define NAK 0x15

#define RX 10
#define TX 9
#define RES 13
#define LEDPIN 13

SoftwareSerial tft(RX, TX);

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

int led = 0;

void setup() {
  // setup serial connection
  Serial.begin(9600);  

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
}


void loop(){
  serialReflect();
}

void drawPixel(uint8_t buf[]) {
  tft.write(0x50);
  tft.write(buf, 6);
  while(tft.available() == 0) { delay(1); }
  char r = tft.read();
}

byte x = 0x7f;
byte y = 0x7f;

void serialReflect(){
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'w')
      y++;
    if (c == 's')
      y--;
    if (c == 'a')
      x++;
    if (c == 'd')
      x--;
      
    resetScreen();
    // draw another red circle
    tft.write(0x43); // draw a circle
    tft.write((byte)0); // x = 63
    tft.write(x);
    tft.write((byte)0); // y = 63
    tft.write(y);
    tft.write((byte)0); // r = 34
    tft.write(0x22);
    tft.write((byte)0); // color = red
    tft.write(0x1f);
    while(tft.available() == 0) { delay(1); }
    char r = tft.read();
    delay(10);
  }
}

void serialReader(){
  int makeSerialStringPosition;
  int inByte;
  uint8_t serialReadString[50];
  const int terminatingChar = 13; //Terminate lines with CR

  inByte = Serial.read();
  makeSerialStringPosition=0;

  if (inByte > 0 && inByte != terminatingChar) { //If we see data (inByte > 0) and that data isn't a carriage return
    delay(100); //Allow serial data time to collect (I think. All I know is it doesn't work without this.)

    while (inByte != terminatingChar && Serial.available() > 0){ // As long as EOL not found and there's more to read, keep reading
      serialReadString[makeSerialStringPosition] = inByte; // Save the data in a character array
      makeSerialStringPosition++; //Increment position in array
      //if (inByte > 0) Serial.println(inByte); // Debug line that prints the charcodes one per line for everything recieved over serial
      inByte = Serial.read(); // Read next byte
    }

    if (inByte == terminatingChar) //If we terminated properly
    {
      serialReadString[makeSerialStringPosition] = 0; //Null terminate the serialReadString (Overwrites last position char (terminating char) with 0
      // Serial.println((char*)serialReadString);
      drawPixel(serialReadString);      
    }
  } 
}

void toggleLed(){
 
  if (led == 1){
    digitalWrite(LEDPIN, LOW);
    led == 0; 
  }
  else {
    digitalWrite(LEDPIN, HIGH);
    led == 1;     
  }
  
}

void resetScreen(){
  tft.write(0x45);
  while(tft.available() == 0) { delay(1); }
  char r = tft.read();
}
