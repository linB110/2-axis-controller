void angle1Move() {
  encoder_1 = megaEncoderCounter.XAxisGetCount();
  // 讀取期望的角度
  // Serial.print("輸入期望的 Theta1: ");
  while(Serial.available() == 0) {}
  int desiredTheta1 = Serial.parseFloat();
  float theta1Count = desiredTheta1 * a1c1; 
  float tolerance = 5; // 設置容忍度

  // Serial.println("thetaCount");
  // Serial.println(theta1Count);

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
  Serial.println("done !");
}