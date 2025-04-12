void calculation() {

  float desire_theta1;
  float desire_theta2;
  encoder_2 = megaEncoderCounter.YAxisGetCount();
  encoder_1 = megaEncoderCounter.XAxisGetCount();

  // calculation part
  Serial.println("start");
  Serial.println("enter x cord");
  while (Serial.available() == 0) {}
  float X = Serial.parseFloat();
  Serial.println("enter y cord");
  while (Serial.available() == 0) {}
  float Y = Serial.parseFloat();
  float Theta2 = megaEncoderCounter.YAxisGetCount();
  float Theta1 = megaEncoderCounter.XAxisGetCount();

  Serial.println("x is ");
  Serial.println(X);
  Serial.println("y is ");
  Serial.println(Y);

  float Theta21 = acos((X*X+Y*Y-a1*a1-a2*a2)/(2*a1*a2));
  float Theta11 = atan2(Y,X)-atan2(a2*sin(Theta21),(a1+a2*cos(Theta21)));
  Theta21 = RAD_TO_DEG *Theta21;
  Theta11 = RAD_TO_DEG *Theta11;


  float Theta22 = -acos((X*X+Y*Y-a1*a1-a2*a2)/(2*a1*a2));
  float Theta12 = atan2(Y,X)-atan2(a2*sin(Theta21),(a1+a2*cos(Theta21)));
  Theta22 = RAD_TO_DEG *Theta22;
  Theta12 = RAD_TO_DEG *Theta12;

  if(abs(Theta11)<=90 && abs(Theta21)<=180 && abs(Theta12)<=90 && abs(Theta22)<=180)
  {
    Serial.println("both solution are in work region");
    float displacement_1 = abs(Theta11 - Theta1) + abs(Theta21 - Theta2);
    float displacement_2 = abs(Theta12 - Theta1) + abs(Theta22 - Theta2);
    Serial.print("displacement_1 : ");
    Serial.println(displacement_1);
    Serial.print("displacement_2 : ");
    Serial.println(displacement_2);
    Serial.println("");

    if (displacement_1 < displacement_2){
      Serial.println("angle1 solution is ");
      Serial.println(Theta11);
      Serial.println("angle2 solution is ");
      Serial.println(Theta21);
    }

    else if (displacement_1 > displacement_2){
      Serial.println("angle1 solution is ");
      Serial.println(Theta12);
      Serial.println("angle2 solution is ");
      Serial.println(Theta22);
    }
  }

  else {
    // solution 1
    if (abs(Theta11)<=90 && abs(Theta21)<=180 && abs(Theta12)>=90 || abs(Theta22)>=180){
      Serial.println("angle1 solution is ");
      Serial.println(Theta11);
      desire_theta1 = Theta11;
      Serial.println("angle2 solution is ");
      Serial.println(Theta21);
      desire_theta2 = Theta21;
    }

    // solution 2 
    else if (abs(Theta11)>=90 || abs(Theta21)<=180 && abs(Theta12)<=90 && abs(Theta22)<=180){
      Serial.println("angle1 solution is ");
      Serial.println(Theta12);
      desire_theta1 = Theta12;
      Serial.println("angle2 solution is ");
      Serial.println(Theta22);
      desire_theta2 = Theta22;
    }

    //  no solution
    else{
      Serial.println("no such solution");
      desire_theta1 = Theta1;
      desire_theta2 = Theta2;
    }
  }

  // motion part
  float theta_2_count = desire_theta2*a2c2; 
  float theta_1_count = desire_theta1*a1c1; 

  Serial.println("desire_theta1");
  Serial.println(desire_theta1);
  Serial.println("desire_theta2");
  Serial.println(desire_theta2);

  Serial.println("theta_1_count");
  Serial.println(theta_1_count);
  Serial.println("theta_2_count");
  Serial.println(theta_2_count);

  while (digitalRead(infrared_1) == LOW && abs(encoder_1 - theta_1_count) > 5 ){
    // both forward
    if (theta_1_count > encoder_1 && theta_2_count > encoder_2){
      encoder_1 = megaEncoderCounter.XAxisGetCount();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
      M1Forward();
      M2Forward(speed2);
    }

    // both backward
    else if  (theta_1_count < encoder_1 && theta_2_count < encoder_2){
      encoder_1 = megaEncoderCounter.XAxisGetCount();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
      M1Backward();
      M2Backward(speed2);
    }

    // 1 forward 2 backward 
    else if (theta_1_count > encoder_1 && theta_2_count < encoder_2){
      encoder_1 = megaEncoderCounter.XAxisGetCount();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
      M1Forward();
      M2Backward(speed2);
    }

    // 1 backward  2 forward 
    else if (theta_1_count < encoder_1 && theta_2_count > encoder_2){
      encoder_1 = megaEncoderCounter.XAxisGetCount();
      encoder_2 = megaEncoderCounter.YAxisGetCount();
      M1Backward();
      M2Forward(speed2);
    }

    else if (digitalRead(infrared_1) == HIGH){
    Serial.println("   ");
    Serial.println("limit");
    Serial.println("   ");
    M1stop();
    delay(2000);
    break;
    }
  }

  // if (digitalRead(infrared_1) == HIGH){
  //   Serial.println("   ");
  //   Serial.println("limit");
  //   Serial.println("   ");
  //   delay(2000);
  //   M1stop();
  //   delay(2000);
  // }

  M1stop();
  M2stop();
  Serial.println(" done !");
}