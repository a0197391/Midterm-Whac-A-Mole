#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         0          
#define SS_PIN          5  //就是模組上的SDA接腳

#define ROW1 25 
#define ROW2 26 
#define ROW3 27 
#define ROW4 14 
#define ROW5 12 
#define ROW6 34 
#define ROW7 35 
#define ROW8 32 

#define COL1 17 
#define COL2 16 
#define COL3 4 
#define COL4 2 
#define COL5 15 
#define COL6 33 
#define COL7 13
#define COL8 21 

int flag=8;
int mouseappear=1;
int times=0;
int ranx;
int rany;
int x=0,y=0;

int id1[]={117,5,188,4};
int id2[]={101,30,252,3};
int id3[]={133,242,195,4};
int id4[]={37,224,7,4};
int id5[]={208,177,247,121};
int id6[]={5,1722,546,7};

const int row[]={ROW1,ROW2,ROW3,ROW4,ROW5,ROW6,ROW7,ROW8};
const int col[]={COL1,COL2,COL3,COL4,COL5,COL6,COL7,COL8};

MFRC522 mfrc522;

byte scan[8][8]={
  {1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,1}
  };
byte mouse[8][8]={
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}
  };

  byte wrong[8][8]={
  {0,1,1,1,0,1,1,1},
  {1,0,1,0,1,1,1,1},
  {1,1,0,1,1,1,1,1},
  {1,0,1,0,1,1,1,1},
  {0,1,1,1,0,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1}
  };


void randommouse(){
  x=0;y=0;
  mouse[ranx][rany] = 1;
  ranx = random(5);
  rany = random(5);
  mouse[ranx][rany] = 0;
  Serial.print("x,y=");
  Serial.print(ranx+1);
  Serial.println(rany+1);
  mouseappear=1;
}

 void dump_byte_array(byte *buffer, byte bufferSize) 
 {
  showmouse();
  for (byte i = 0; i < bufferSize; i++) {
  showmouse();
  Serial.print(buffer[i],  DEC);
  }
 
 showmouse();
 
 if(buffer[1]==id1[1]){flag=1;times=times+1;}
 else if(buffer[1]==id2[1]){flag=2;times=times+1;}
 else if(buffer[1]==id3[1]){flag=3;times=times+1;}
 else if(buffer[1]==id4[1]){flag=4;times=times+1;}
 else if(buffer[1]==id5[1]){flag=5;times=times+1;}
 else if(buffer[1]==id6[1]){flag=6;times=times+1;}
 Serial.println();
 showmouse();
 showmouse();
 showmouse();
}//RFID辨識*/
 
void showmouse(){
  for(byte a = 0; a < 8; a++){
   for(byte i = 0; i < 8; i++){
      for(byte j = 0; j < 8; j++){
      digitalWrite(row[j],1-scan[i][j]);
      digitalWrite(col[j],1-mouse[i][j]);
       }
      for(byte j = 0; j < 8; j++){
      digitalWrite(row[j],HIGH);
      digitalWrite(col[j],LOW);
      }
  }
   }   
}
  
void scanhit(){
 if((mfrc522.PICC_IsNewCardPresent())&&(mfrc522.PICC_ReadCardSerial()))
 {
    showmouse();   
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    showmouse();   
    delay(500);
    hitmouse();
 }   
}

void hitmouse(){
  
  if( (times%2)==1){
    x=flag-1;
    //Serial.println(x);
    }
   /* else if((y!=(rany+1))&&(x!=0)){
    Serial.println("wrong");
    mouseappear=0;}*/
  else if( (times%2)==0){
    y=flag-1;
   // Serial.println(y); 
    if((x==ranx)&&(y==rany)){
    Serial.println("right");
    mouseappear=0;
    }
    else if((x!=ranx)||(y!=rany)){
    Serial.println("wrong");
    cross();
    mouseappear=0;
    }
  }
}
void cross(){
  for(int a=0;a<10;a++){
 for(byte i = 0; i < 8; i++){
      for(byte j = 0; j < 8; j++){
      digitalWrite(col[j],LOW);
      digitalWrite(row[j],HIGH);
      digitalWrite(row[j],1-scan[i][j]);
      digitalWrite(col[j],1-wrong[i][j]);
      
      }
      delay(10); 
      }  
  }
}



void setup(){
//RFID setup
  Serial.begin(9600); 
  SPI.begin();        // 初始化SPI介面
  mfrc522.PCD_Init(SS_PIN, RST_PIN); // 初始化MFRC522卡
  Serial.print(F("Reader "));
  Serial.print(F(": "));
  mfrc522.PCD_DumpVersionToSerial();
//matrix setup
  pinMode(row[0],OUTPUT); 
  pinMode(row[1],OUTPUT);
  pinMode(row[2],OUTPUT);
  pinMode(row[3],OUTPUT);
  pinMode(row[4],OUTPUT); 

  pinMode(col[0],OUTPUT);
  pinMode(col[1],OUTPUT);
  pinMode(col[2],OUTPUT);
  pinMode(col[3],OUTPUT);
  pinMode(col[4],OUTPUT);
}

void loop(){
 
  randommouse();
  while(mouseappear!=0){  
      showmouse();
      showmouse();
      showmouse();
      scanhit();
    } 
}
//mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()

