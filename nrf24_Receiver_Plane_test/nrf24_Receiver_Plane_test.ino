#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
int val;
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
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //  Set the module as receiver
  resetData();
  pinMode(13,1);
  servo2.attach(2);
  servo3.attach(3);
  servo4.attach(4);
  servo5.attach(5);
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  servo5.writeMicroseconds(1500);
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
  Serial.print("j1PotX: ");
  Serial.print(data.Rx);
  Serial.print("; j1PotY: ");
  Serial.print(data.Ry);
  Serial.print("; button1: ");
  Serial.print(data.b1);
  Serial.print("; j2PotX: ");
  Serial.println(data.b2); 
  if(data.b2==1){
    digitalWrite(13,1);
  }
  else{
    digitalWrite(13,0); 
