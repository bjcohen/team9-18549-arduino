#include <SoftwareSerial.h>

#define ACK 0x6
#define NAK 0x15

#define RX 10
#define TX 9

SoftwareSerial tft(RX, TX);

char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

int led = 0;

void setup() {
  // setup serial connection
  Serial.begin(9600);  

  
  // reset screen
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delayMicroseconds(3); // 2 us required, so do 3
  digitalWrite(13, HIGH);
  
  
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
  serialReader(); 
}

void drawPixel(uint8_t buf[]) {
  tft.write(0x50);
  tft.write(buf, 6);
  while(tft.available() == 0) { delay(1); }
  char r = tft.read();
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
    digitalWrite(13, LOW);
    led == 0; 
  }
  else {
    digitalWrite(13, HIGH);
    led == 1;     
  }
  
}

void resetScreen(){
  tft.write(0x45);
  while(tft.available() == 0) { delay(1); }
  char r = tft.read();
}
