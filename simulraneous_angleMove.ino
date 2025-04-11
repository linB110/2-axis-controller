void simultaneousAngleMove() {
    float desire_angle1;
    float desire_angle2;
    bool scmd = 0;
    encoder_2 = megaEncoderCounter.YAxisGetCount();
    encoder_1 = megaEncoderCounter.XAxisGetCount();

    Serial.println("Enter desired angle of axis 1:");
    while (Serial.available() == 0) {}
    desire_angle1 = Serial.parseFloat();
    Serial.println("Enter desired angle of axis 2:");
    while (Serial.available() == 0) {}
    desire_angle2 = Serial.parseFloat();

    float theta1Count1 = desire_angle1 * a1c1;
    float theta2Count2 = desire_angle2 * a2c2;

    while (abs(encoder_1 - theta1Count1) > 3 * a1c1 && scmd == 0 || abs(encoder_2 - theta2Count2) > 3 * a2c2 ) {
        encoder_1 = megaEncoderCounter.XAxisGetCount();
        encoder_2 = megaEncoderCounter.YAxisGetCount();

        if (digitalRead(infrared_1) == HIGH){
          scmd = 1;
        }

        // 控制馬達1
        if (abs(encoder_1 - theta1Count1) > 3 * a1c1 ) {
            if (encoder_1 < theta1Count1) {
                M1Forward();
            } else {
                M1Backward();
            }
        } else {
            M1stop();
        }

        // 控制馬達2
        if (abs(encoder_2 - theta2Count2) > 3 * a2c2) {
            if (encoder_2 < theta2Count2) {
                M2Forward(speed2);
            } else {
                M2Backward(speed2);
            }
        } else {
            M2stop();
        }
    }
    if (scmd == 1){
      M1stop();
    }
    M1stop();
    M2stop();
    Serial.println("Move completed!");
}
