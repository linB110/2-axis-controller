void M2Pstop() {
  analogWrite(enB, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  motorStopped = true; // 標記馬達停止
}

void speedScan(){
  encoder_2 = megaEncoderCounter.YAxisGetCount();
  if (!motorStopped) {
    error = desireRpm - (((encoder_2 - old_reading) / a2c2)*0.1666667 / (0.01)); 
    // Serial.println(desireRpm - error);
    float pid = pidFunction(error);

    if (abs(error) > 1){
      if (error > 0){
        // accelerate
        M2Forward(desireRpm + pid);   
      } 
      else {
        // decelerate
        M2Forward(desireRpm - pid);
      }
    }
  } 
  else {
    M2Forward(desireRpm); 
    M2Pstop();
  }
  old_reading = encoder_2;
}

float pidFunction(float error){
  float dt = 0.01;  // Assume the scan function is called every 10ms
  float kp = 50, ki = 5, kd = 0.5;
  float output;

  integral_2 += error * dt;
  derivative_2 = (error - preError) / dt;

  if (error == preError)
    output = desireRpm;

  else {
    preError = error;
    output = kp * error + ki * integral_2 + kd * derivative_2;
  }

  // 限制输出在范围内
  if (output > Max2_speed) {
    return Max2_speed;
  } else if (output < Min2_speed) {
    return Min2_speed;
  } else {
    return output;
  }
}

void motorPid() {
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
}
