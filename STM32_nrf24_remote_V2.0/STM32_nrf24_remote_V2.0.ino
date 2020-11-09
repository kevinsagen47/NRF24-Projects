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
RF24 radio(PA4, PB0);   // nRF24L01 (CE, CSN)
const byte address[6] = "11100"; // Address
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  short Lx;
  short Ly;
  short Rx;
  short Ry;
  byte L1;
  byte L2;
  byte R1;
  byte R2;
  byte bup;
  byte bdown;
  byte bright;
  byte bleft;
  byte b1;
  byte b2;
  byte b3;
  byte b4;
  byte bstart;
  byte bselect;
  byte banalog;
};
Data_Package data; //Create a variable with the above structure
void setup() {
  Serial.begin(115200);
   
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  
  // Activate the Arduino internal pull-up resistors
  pinMode(PB9, INPUT_PULLUP);
  pinMode(PB8, INPUT_PULLUP);
  pinMode(PB7, INPUT_PULLUP);
  pinMode(PB4, INPUT_PULLUP);
  pinMode(PB5, INPUT_PULLUP);
  pinMode(PB6, INPUT_PULLUP);
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PA15, INPUT_PULLUP);
  pinMode(PB12, INPUT_PULLUP);
  pinMode(PA10, INPUT_PULLUP);
  pinMode(PA9, INPUT_PULLUP);
  pinMode(PA8, INPUT_PULLUP);
  pinMode(PB15, INPUT_PULLUP);
  pinMode(PB13, INPUT_PULLUP);
  pinMode(PB14, INPUT_PULLUP);
  pinMode(PC13, OUTPUT);
 // pinMode(bA0, INPUT_PULLUP);
  
  // Set initial default values
  data.Lx = 1500; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.Ly = 1500;
  data.Rx = 1500;
  data.Ly = 1500;
  data.L1 =0;
  data.L2 =0;
  data.R1 =0;
  data.R2 =0;
  data.bup =0;
  data.bdown =0;
  data.bright =0;
  data.bleft =0;
  data.b1 =0;
  data.b2 =0;
  data.b3 =0;
  data.b4 =0;
  data.bstart =0;
  data.bselect =0;
  data.banalog =0;
}
void loop() {
  digitalWrite(PC13,HIGH);
  // Read all analog inputs and map them to one Byte value
  data.Lx = map(analogRead(PA0), 0, 4096, 1000, 2000); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.Ly = map(analogRead(PA1), 0, 4096, 1000, 2000);
  data.Rx = map(analogRead(PA2), 0, 4096, 1000, 2000);
  data.Ry = map(analogRead(PA3), 0, 4096, 1000, 2000);
  data.L1 =! digitalRead(PB8);
  data.L2 =! digitalRead(PB9);
  data.R1 =! digitalRead(PA9);
  data.R2 =! digitalRead(PA10);
  data.bup =! digitalRead(PB7);
  data.bdown =! digitalRead(PB5);
  data.bright =! digitalRead(PB4);
  data.bleft =! digitalRead(PB6);
  data.b1 =! digitalRead(PA8);
  data.b2 =! digitalRead(PB14);
  data.b3 =! digitalRead(PB13);
  data.b4 =! digitalRead(PB15);
  data.bstart =! digitalRead(PB12);
  data.bselect =! digitalRead(PB3);
  data.banalog =! digitalRead(PA15);
  // If toggle switch 1 is switched on
  // Send the whole data from the structure to the receiver
  //Serial.println(data.Rx);
  radio.write(&data, sizeof(Data_Package));
}
