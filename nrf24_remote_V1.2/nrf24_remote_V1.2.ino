/*
        DIY Arduino based RC Transmitter
  by Dejan Nedelkovski, www.HowToMechatronics.com
  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <Wire.h>
// Define the digital inputs


float elapsedTime, currentTime, previousTime;
int c = 0;
RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  short Lx;
  short Ly;
  short Rx;
  short Ry;
  byte b0;
  byte b1;
  byte b2;
  byte b3;
  byte b4;
  byte b5;
  byte b6;
  byte b7;
  byte b8;
  byte bA1;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(9600);
   
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
 // pinMode(bA0, INPUT_PULLUP);
  
  // Set initial default values
  data.Lx = 1500; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.Ly = 1500;
  data.Rx = 1500;
  data.Ly = 1500;
  data.b0 = 0;
  data.b1 = 0;
  data.b2 = 0;
  data.b3 = 0;
  data.b4 = 0;
  data.b5 = 0;
  data.b6 = 0;
  data.b7 = 0;
  data.b8 = 0;
  data.bA1 = 0;
}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.Lx = map(analogRead(A6), 0, 1023, 1000, 2000); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.Ly = map(analogRead(A7), 0, 1023, 1000, 2000);
  data.Rx = map(analogRead(A3), 0, 1023, 2000, 1000);
  data.Ry = map(analogRead(A2), 0, 1023, 2000, 1000);
  data.b0 =! digitalRead(0);
  data.b1 =! digitalRead(1);
  data.b2 =! digitalRead(2);
  data.b3 =! digitalRead(3);
  data.b4 =! digitalRead(4);
  data.b5 =! digitalRead(5);
  data.b6 =! digitalRead(6);
  data.b7 =! digitalRead(7);
  data.b8 =! digitalRead(8);
  data.bA1 =! digitalRead(A1);
  // If toggle switch 1 is switched on
  // Send the whole data from the structure to the receiver
  Serial.println(data.Rx);
  radio.write(&data, sizeof(Data_Package));
}
