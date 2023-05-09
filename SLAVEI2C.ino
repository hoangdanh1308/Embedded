/*
  File: SLAVEI2C.ino
  Author: NGUYEN HOANG DANH
  Date: 08/05/2023
  Description: This is file for SLAVEI2C using Arduino
*/
#define SDA_PIN 3
#define SCL_PIN 4

#define TFULL 100
#define THALF 50

void setup() {
  // put your setup code here, to run once:
pinMode(SDA_PIN, INPUT);
pinMode(SCL_PIN, INPUT);
Serial.begin(9600);
delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
transferI2C();
while(digitalRead(SCL_PIN) == LOW);

}
/*
  Function: transferI2C
  Description: This function use for Slave to transfer data I2C.
  Input:
    input: None
  Output:
    return: None
*/
void transferI2C(void){
uint8_t addressSlave = 0x48;    // address: address: 0x48 (hex) -> 72(dec) -> 1001000 (bin)
uint8_t i = 0;
uint8_t checkAddress;
uint8_t tempAddress = 0;
uint8_t tempReadWriteBit = 2;
while(digitalRead(SDA_PIN) == HIGH);
while(digitalRead(SCL_PIN) == HIGH);
while(digitalRead(SCL_PIN) == LOW);
for(i = 0; i < 7; i ++){
   while(digitalRead(SCL_PIN) == LOW);
   tempAddress = tempAddress << 1;
   if(digitalRead(SDA_PIN) == HIGH){
   ++tempAddress;
    }
   while(digitalRead(SCL_PIN) == HIGH);
   }
 while(digitalRead(SCL_PIN) == LOW);
 if(digitalRead(SDA_PIN) == HIGH){tempReadWriteBit = 1;}else {tempReadWriteBit = 0;}
 while(digitalRead(SCL_PIN) == HIGH);
 if(addressSlave == tempAddress){
  slaveAck();
  if(tempReadWriteBit == 0){uint8_t byteIn; byteIn = readDataFromMaster(0);Serial.print(byteIn);Serial.print(' ');
  }else {
    sendDataToMaster(0x68);    // send data: 0x68 (hex) -> 104(dec) -> 1101000 (bin)
    while(digitalRead(SCL_PIN) == LOW);
    while(digitalRead(SCL_PIN) == HIGH);
  }
  }else{
   for(int i = 0; i < 10; i++){
   while(digitalRead(SCL_PIN) == LOW);
   while(digitalRead(SCL_PIN) == HIGH);
   }
  }
}
/*
  Function: slaveAck
  Description: This function use for sending Ack bit to Master.
  Input:
    input: None
  Output:
    return: None
*/
void slaveAck(void){
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);
  while(digitalRead(SCL_PIN) == LOW);
  digitalWrite(SDA_PIN, LOW);
  while(digitalRead(SCL_PIN) == HIGH);
  pinMode(SDA_PIN, INPUT);
  }
/*
  Function: slaveNAck
  Description: This function use for sending NAck bit to Master.
  Input:
    input: None
  Output:
    return: None
*/
void slaveNAck(void){
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  while(digitalRead(SCL_PIN) == LOW);
  digitalWrite(SDA_PIN, HIGH);
  while(digitalRead(SCL_PIN) == HIGH);
  pinMode(SDA_PIN, INPUT);
  }
/*
  Function: readDataFromMaster
  Description: This function use for reading data from Master.
  Input:
    input: uint8_t ack
  Output:
    return: uint8_t result
*/
 uint8_t readDataFromMaster(uint8_t ack){
  uint8_t i = 0;
  uint8_t result = 0;
  for(i = 0; i < 8; i++){
   while(digitalRead(SCL_PIN) == LOW);
   result = result<<1;
    if(digitalRead(SDA_PIN) == HIGH){
      ++result;
      }
   while(digitalRead(SCL_PIN) == HIGH);
    }
   if(ack){
    slaveNAck();
    }else {
    slaveAck();
    }
   return result;
 }
/*
  Function: sendDataToMaster
  Description: This function use for sending data to Master.
  Input:
    input: 
    uint8_t byteOut
  Output:
    return: None
*/
void sendDataToMaster(uint8_t byteOut){
  uint8_t tempBit;
  uint8_t coutByte = 0x80;
  pinMode(SDA_PIN, OUTPUT);
  for (coutByte = 0x80; coutByte > 0; coutByte = coutByte >> 1){
    tempBit = byteOut & coutByte;
    if(tempBit != 0){
      digitalWrite(SDA_PIN, HIGH);
      }else{
        digitalWrite(SDA_PIN,LOW);
        }
  while(digitalRead(SCL_PIN) == LOW);
  while(digitalRead(SCL_PIN) == HIGH);
}  
}

 
 
 
