//  motor1 control program
void M1control(float desiredTheta1) {
  megaEncoderCounter.XAxisReset();
  encoder_1 = megaEncoderCounter.XAxisGetCount();
  float theta1Count = desiredTheta1 * a1c1; 
  float tolerance = 5; // 設置容忍度

  // 向後運動直到達到期望的角度
  while (encoder_1 < theta1Count - tolerance) {
    encoder_1 = megaEncoderCounter.XAxisGetCount();
    M1Forward();
  }

  // 向前運動直到達到期望的角度
  while (encoder_1 > theta1Count + tolerance) {
    encoder_1 = megaEncoderCounter.XAxisGetCount();
    M1Backward();
  }
 
  // 停止軸運動
  M1stop();
}
// 

//  motor2 control program
void M2control(float desiredTheta2) {
  megaEncoderCounter.YAxisReset();
  encoder_2 = megaEncoderCounter.YAxisGetCount();
  float theta2Count = desiredTheta2 * a2c2; 
  float tolerance2 = 5; // 設置容忍度

  // 向後運動直到達到期望的角度
  while (encoder_2 < theta2Count - tolerance2) {
    encoder_2 = megaEncoderCounter.YAxisGetCount();
    M2Forward(speed2);
  }

  // 向前運動直到達到期望的角度
  while (encoder_2 > theta2Count + tolerance2) {
    encoder_2 = megaEncoderCounter.YAxisGetCount();
    M2Backward(speed2);
  }
 
  // 停止軸運動
  M2stop();
}
// 


// infrared function
void checkInfrared1() {
  int val1 = digitalRead(infrared_1);
  // Serial.println(val1);
}

void checkInfrared2() {
  int val2 = digitalRead(infrared_2);
  // Serial.println(val2);
}

// return function
void simultaneousReturn(){
  int cmd1 = 0;
  int cmd2 = 0;

  while (cmd1 == 0 && cmd2 == 0){
    while (digitalRead(infrared_1) == LOW && digitalRead(infrared_2) == LOW){
      M1Backward();
      M2Forward(speed2+5);
    }

    while (digitalRead(infrared_1) == HIGH && digitalRead(infrared_2) == LOW){
      M1stop();
      M2Forward(speed2+5);
      cmd1 = 1;
    }
  }

  if (digitalRead(infrared_1) == HIGH && digitalRead(infrared_2) == HIGH){
    M1stop();
    M2stop();
    delay(1000);
    cmd1 = 1;
    cmd2 = 1;
    megaEncoderCounter.XAxisReset();
    megaEncoderCounter.YAxisReset();
  }

  if (cmd1 == 1 && cmd2 == 1){
    M1stop();
    M2stop();
    // M1control(90);
    // M2control(130);
    while (encoder_1 < 87*a1c1 && encoder_2 < 180*a2c2 ){
      M1Forward();
      M2Backward(speed2+10);
      encoder_1 = megaEncoderCounter.XAxisGetCount();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
    }
  }
  M1stop();
  M2stop();
  M2control(50);  // 消除慣性擺動角度
  delay(2000);
  megaEncoderCounter.XAxisReset();
  megaEncoderCounter.YAxisReset();
  Serial.println(" done !");
}
