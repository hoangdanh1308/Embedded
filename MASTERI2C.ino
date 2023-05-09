/*
  File: MASTERI2C.ino
  Author: NGUYEN HOANG DANH
  Date: 08/05/2023
  Description: This is file for MASTERI2C using Arduino
*/

#define SDA_PIN 3
#define SCL_PIN 4

#define TFULL 100
#define THALF 50


void setup() {
  // put your setup code here, to run once:
pinMode(SDA_PIN, OUTPUT);
pinMode(SCL_PIN, OUTPUT);
digitalWrite(SDA_PIN, HIGH);
digitalWrite(SCL_PIN, HIGH);
Serial.begin(9600);
delay(1);
}

void loop() {
  // put your main code here, to run repeatedly:
startConditionI2C();
selectAddressandRW(0x48 ,0 );   // address: 0x48 (hex) -> 72(dec) -> 1001000 (bin), write data
checkAckBit();
sendDataToSlave(0x86);          // data: 0x86(hex) -> 134(dec) -> 10000110 (bin)
checkAckBit();
endConditionI2C();

startConditionI2C();
selectAddressandRW(0x44 ,1 );    // address: 0x44 (hex) -> 68(dec) -> 1000100 (bin), read data
checkAckBit();
uint8_t receive = readDataFromSlave(0);
Serial.print(receive);
Serial.print(' ');
endConditionI2C();
}
/*
  Function: startConditionI2C
  Description: This function use for sending bit start to start I2C transfer.
  Input:
    input: None
  Output:
    return: None
*/
void startConditionI2C(void){
pinMode(SDA_PIN, OUTPUT);
digitalWrite(SDA_PIN, HIGH);
digitalWrite(SCL_PIN, HIGH);
delay(THALF);
digitalWrite(SDA_PIN, LOW);
delay(THALF);
digitalWrite(SCL_PIN, LOW);
}
/*
  Function: endConditionI2C
  Description: This function use for sending bit end to end I2C transfer.
  Input:
    input: None
  Output:
    return: None
*/
void endConditionI2C(void){
 pinMode(SDA_PIN, OUTPUT);
 digitalWrite(SDA_PIN, LOW);
 digitalWrite(SCL_PIN, LOW);
 delay(THALF);
 digitalWrite(SCL_PIN, HIGH);
 delay(THALF);
 digitalWrite(SDA_PIN, HIGH);
}
/*
  Function: selectAddressandRW
  Description: This function use for select slave address and select Write or read data.
  Input:
    input: 
    uint8_t addressSlave
    uint8_t readwriteBit
  Output:
    return: None
*/
void selectAddressandRW(uint8_t addressSlave,uint8_t readwriteBit){
  uint8_t tempBit;
  uint8_t coutByte = 0x40;
  pinMode(SDA_PIN, OUTPUT);
  for (coutByte = 0x40; coutByte > 0; coutByte = coutByte >> 1){
    tempBit = addressSlave & coutByte;
    if(tempBit != 0){
      digitalWrite(SDA_PIN, HIGH);
      }else{
        digitalWrite(SDA_PIN,LOW);
        }
    delay(THALF);
    digitalWrite(SCL_PIN,HIGH);
    delay(THALF);
    digitalWrite(SCL_PIN,LOW);
}
if(readwriteBit != 0)
{
  digitalWrite(SDA_PIN, HIGH);
  }else {
  digitalWrite(SDA_PIN, LOW);
  }
delay(THALF);
digitalWrite(SCL_PIN,HIGH);
delay(THALF);
digitalWrite(SCL_PIN,LOW);
}
/*
  Function: sendDataToSlave
  Description: This function use for sending data to Slave.
  Input:
    input: 
    uint8_t byteOut
  Output:
    return: None
*/
void sendDataToSlave(uint8_t byteOut){
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
    delay(THALF);
    digitalWrite(SCL_PIN,HIGH);
    delay(THALF);
    digitalWrite(SCL_PIN,LOW);
}  
}
/*
  Function: masterAck
  Description: This function use for sending Ack bit to Slave.
  Input:
    input: None
  Output:
    return: None
*/
void masterAck(void){
    pinMode(SDA_PIN, OUTPUT);
    digitalWrite(SDA_PIN,LOW);
    delay(THALF);
    digitalWrite(SCL_PIN,HIGH);
    delay(THALF);
    digitalWrite(SCL_PIN,LOW);
}
/*
  Function: masterNAck
  Description: This function use for sending NAck bit to Slave.
  Input:
    input: None
  Output:
    return: None
*/
void masterNAck(void){
    pinMode(SDA_PIN, OUTPUT);
    digitalWrite(SDA_PIN,HIGH);
    delay(THALF);
    digitalWrite(SCL_PIN,HIGH);
    delay(THALF);
    digitalWrite(SCL_PIN,LOW);
}
/*
  Function: readDataFromSlave
  Description: This function use for reading data from Slave.
  Input:
    input: uint8_t ack
  Output:
    return: uint8_t result
*/
uint8_t readDataFromSlave(uint8_t ack){
  uint8_t i = 0;
  uint8_t result = 0;
  pinMode(SDA_PIN, INPUT);
  for(i = 0; i < 8; i++){
    delay(THALF);
    digitalWrite(SCL_PIN,HIGH);
     result = result<<1;
    if(digitalRead(SDA_PIN) == HIGH){
      ++result;
      }
    delay(THALF);
    digitalWrite(SCL_PIN,LOW);
    }
   if(ack){
    masterNAck();
    }else {
    masterAck();
    }
   return result;
}
/*
  Function: checkAckBit
  Description: This function use for check Ack or NAck bit from Slave.
  Input:
    input: none
  Output:
    return: none
*/
void checkAckBit(void){
 uint8_t countTime = 0;
 pinMode(SDA_PIN, OUTPUT);
 delay(THALF);
 digitalWrite(SCL_PIN,HIGH);
 pinMode(SDA_PIN, INPUT);
 while(digitalRead(SDA_PIN) == HIGH){
  ++countTime;
  if(countTime > 250){
    Serial.print("go to transfer again");
    }
  }
  delay(THALF);
  digitalWrite(SCL_PIN,LOW);
  }
