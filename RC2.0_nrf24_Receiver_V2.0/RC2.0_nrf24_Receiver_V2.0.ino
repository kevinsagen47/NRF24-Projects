
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(PB0,PA4);   // nRF24L01 (CE, CSN)PB13,PB12 PB0, PA4
const byte address[6] = "11100";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
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

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  pinMode(PC13, OUTPUT);
  //pinMode(PB3, OUTPUT);
}
void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();

  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  Serial.println("COnnection Lost");
  }
  // Print the data in the Serial Monitor
  Serial.print("Lx: ");
  Serial.print(data.Lx);
  Serial.print("; Ly: ");
  Serial.print(data.Ly);
  Serial.print("Rx: ");
  Serial.print(data.Rx);
  Serial.print("; Ry: ");
  Serial.print(data.Ry);

  
  Serial.print("; b1: ");
  Serial.print(data.b1);
  Serial.print("; b2: ");
  Serial.print(data.b2);
  Serial.print("; b3: ");
  Serial.print(data.b3);
  Serial.print("; b4: ");
  Serial.print(data.b4);
  Serial.print("; bup: ");
  Serial.print(data.bup);
  Serial.print("; bdown: ");
  Serial.print(data.bdown);
  Serial.print("; bleft: ");
  Serial.print(data.bleft);
  Serial.print("; bright: ");
  Serial.print(data.bright);
  Serial.print("; R1: ");
  Serial.print(data.R1);
  Serial.print("; R2: ");
  Serial.print(data.R2);
  Serial.print("; L1: ");
  Serial.print(data.L1);
  Serial.print("; L2: ");
  Serial.print(data.L2);
  Serial.print("; analog: ");
  Serial.print(data.banalog);
  Serial.print("; start: ");
  Serial.print(data.bstart);
  Serial.print("; bselect: ");
  Serial.println(data.bselect);
  
  if(data.b2==1){
    digitalWrite(PC13, LOW);
  }
  else{
    digitalWrite(PC13,HIGH);
  }
}


//Channel 1 => Roll     => data.Rx
//Channel 2 => pitch    => data.Ry
//Channel 3 => throttle => data.Ly
//Channel 4 => yaw      => data.Lx


void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
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
