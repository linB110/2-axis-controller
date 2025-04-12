#include <MegaEncoderCounter.h>
#include <MsTimer2.h>
#include <math.h>
#define a1 150
#define enA  6               // 第一軸Enable腳位
#define in1 8               // 第一軸(+),輸出時第一軸正轉
#define in2 9 
#define a2 50
#define enB  7               // 第二軸Enable腳位
#define in3 10               // 第二軸(+),輸出時第二軸正轉
#define in4 11 
#define a1c1 15
#define a2c2 23.17
#define infrared_1 2
#define infrared_2 3
#define Motor2_speed 50
#define Motor1_speed 70
#define Min2_speed 40
#define Max2_speed 100

//  PID parameters
int const speed2 = 40;
MegaEncoderCounter megaEncoderCounter(4);
signed long encoder_2 = 0 ;
signed long encoder_1 = 0 ;
signed long old_reading = 0;
float integral_2 = 0, derivative_2 = 0;
float error = 0;
float preError = 0;
float desireRpm;
float desireAngle;
volatile bool motorStopped = false; // 用於標記馬達是否停止

//  motor control cunction
void M1Forward() {
  analogWrite(enA,Motor1_speed);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  }
  
void M1Backward(){
  analogWrite(enA,Motor1_speed);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH); 
  }

void M1stop(){
  analogWrite(enA,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
}

void M2Forward(float motor2Speed) {
  analogWrite(enB, motor2Speed);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void M2Backward(float motor2Speed){
  analogWrite(enB,Motor2_speed);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH); 
  }

void M2stop(){
  analogWrite(enB,0);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}


void setup() {
  Serial.begin(9600);
  pinMode(enB,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(infrared_1, INPUT);
  pinMode(infrared_2, INPUT);
  megaEncoderCounter.YAxisReset();
  megaEncoderCounter.XAxisReset();
  attachInterrupt(digitalPinToInterrupt(infrared_1), checkInfrared1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(infrared_2), checkInfrared2, CHANGE);
  // Serial.println("return mode");
  // delay(2000);
  // simultaneousReturn();
  // Serial.println("return done");
  // reset the oringin
  megaEncoderCounter.XAxisReset();
  megaEncoderCounter.YAxisReset();

  // start timer
  // MsTimer2::set(10, posScan);
  // MsTimer2::set(10, speedScan);
  // MsTimer2::start();

}

void displayMenu() {
    Serial.println("press 0 to start");
    while (Serial.available() == 0) {}
    char start = Serial.read();
    if (start == '0') {
        Serial.println(" choose mode by entering following cmd letter");
        Serial.println("       ");
        Serial.println("       ");
        Serial.println(" axis 1 move -->  press 「 a 」");
        Serial.println(" axis 2 move -->  press 「 b 」");
        Serial.println(" move by cord simutaneously  -->  press 「 c 」");
        Serial.println(" move by angle simutaneously  -->  press 「 d 」");
        Serial.println(" return simutaneously -->  press 「 r 」");
        Serial.println(" pid angle 2 control -->  press 「 p 」");
        Serial.println(" pid motor 2 control -->  press 「 m 」");
        Serial.println(" if you want to stop -->  press 「 s 」");
        Serial.println("       ");
    }
}

void loop() {
  displayMenu();
  // Serial.println("press 0 to start");
  while (Serial.available() == 0){}

  // Serial.println(" choose mode by entering following cmd letter");
  // Serial.println(" axis 1 move -->  press 「 a 」");
  // Serial.println(" axis 2 move -->  press 「 b 」");
  // Serial.println(" move by cord simutaneously  -->  press 「 c 」");
  // Serial.println(" move by angle simutaneously  -->  press 「 d 」");
  // Serial.println(" return simutaneously -->  press 「 r 」");
  // Serial.println(" pid angle 2 control -->  press 「 p 」");
  // Serial.println(" pid motor 2 control -->  press 「 m 」");
  // Serial.println(" if you want to stop -->  press 「 s 」");

  char cmd = Serial.read();
  
  switch (cmd){
    Serial.println("       ");

    case 'a':
      Serial.println(" axis 1 control mode");
      Serial.println(" enter desired angle 1");
      angle1Move();
      break;
    
    case 'b':
      Serial.println(" axis 2 control mode");
      Serial.println(" enter desired angle 2");
      angle2Move();
      break;
    
    case 'c':
      Serial.println(" simutaneous cord move control mode");
      Serial.println(" enter desired x cord");
      Serial.println(" enter desired y cord");
      calculation();
      break;

    case 'd':
      Serial.println(" simutaneous angle move control mode");
      simultaneousAngleMove();
      break;

    case 'r':
      Serial.println(" simutaneous return mode");
      simultaneousReturn();
      break;
    
    case 'p':
      Serial.println(" pid position contol mode of axis 2");
      Serial.println(" enter desired angle 2");
      MsTimer2::set(10, posScan);
      MsTimer2::start();
      while (Serial.available() == 0) {}
      desireAngle = Serial.parseFloat();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
      motorStopped = false;

      // 等待直到馬達停止
      while (!motorStopped) {
        if (Serial.available() > 0) {
          char cmd = Serial.read();
          if (cmd == 's' || cmd == 'S') {
            motorStopped = true;
          }
        }
        delay(10);
      }
      Serial.println("Reached desired angle!");
      break;
    
    case 'm':
      Serial.println(" pid speed contol mode of motor 2");
      Serial.println(" enter desired RPM");

      MsTimer2::set(10, speedScan);
      MsTimer2::start();

      while (Serial.available() == 0) {}
      desireRpm = Serial.parseFloat();
      motorStopped = false;
      Serial.println("if you want to stop, press s or S");

      // 等待直到馬達停止
      while (!motorStopped) {
        if (Serial.available() > 0) {
          char cmd = Serial.read();
          if (cmd == 's' || cmd == 'S') {
            motorStopped = true;
          }
        }
        delay(10);
      }
      Serial.println("Motor stopped.");
      MsTimer2::stop();
      break;
    
    case 's':
      Serial.println(" stop mode ");
      MsTimer2::stop();
      break;

    default :
      Serial.println(" this letter is not in list");
      Serial.println(" please retry");
    
  }

  // Serial.println(" countine to next operation?, if yes press any letter");
  // while (Serial.available() == 0){}
  // char next = Serial.read();
  // if (next > 0)
  //   delay(500);

  Serial.println("       ");
  Serial.println("       ");
}
