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

void transferI2C(void){
uint8_t addressSlave = 0x44;
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
  }else{
    sendDataToMaster(0x68);
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
void slaveAck(void){
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);
  while(digitalRead(SCL_PIN) == LOW);
  digitalWrite(SDA_PIN, LOW);
  while(digitalRead(SCL_PIN) == HIGH);
  pinMode(SDA_PIN, INPUT);
  }
void slaveNAck(void){
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  while(digitalRead(SCL_PIN) == LOW);
  digitalWrite(SDA_PIN, HIGH);
  while(digitalRead(SCL_PIN) == HIGH);
  pinMode(SDA_PIN, INPUT);
  }

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
