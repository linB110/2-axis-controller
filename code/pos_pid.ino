// Use scan func to get error
void posScan() {
  encoder_2 = megaEncoderCounter.YAxisGetCount();
  error = desireAngle * a2c2 - encoder_2;
  // Serial.println(error);
  float pid_cal = pidCompute(error);

  if (abs(error) > 3 * a2c2) { // 判斷誤差範圍，避免頻繁啟停
    if (error > 0) {
      M2Forward(abs(pid_cal));
      motorStopped = false;
    }

    else if (error < 0) {
      M2Backward(abs(pid_cal));
      motorStopped = false;
    }
  }
  else {
    M2Pstop();
  }
}


float pidCompute(float error) {
  float dt = 0.01;  // scan function is called every 10ms
  float kp = 5, ki = 2, kd = 5;
  float output;
  integral_2 += error * dt;
  derivative_2 = (error - preError) / dt;
  if (error == preError)
    output = 0;

  else {
    preError = error;
    // Serial.println("integral_2");
    // Serial.println(integral_2);
    output = kp * error + ki * integral_2 + kd * derivative_2;
  }
  // Serial.println(output);
  

  if (abs(output) > Max2_speed)
    return Max2_speed;
  else if (abs(output) < Min2_speed)
    return Min2_speed;
  else
    return output;
}

void posPid() {
  // Serial.println("Enter desired angle: ");
  while (Serial.available() == 0) {}
  desireAngle = Serial.parseFloat();
  encoder_2 = megaEncoderCounter.YAxisGetCount();
  motorStopped = false;

  // 等待直到馬達停止
  while (!motorStopped) {
    delay(10); 
  }
  // MsTimer2::stop();
}
