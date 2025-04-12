void angle2Move() {
  encoder_2 = megaEncoderCounter.YAxisGetCount();
 // Read the desired theta 
  // Serial.print(" enter Desired Theta2: ");
  while(Serial.available()==0){}
  int Desired_theta2 = Serial.parseFloat();

  float theta_2_count = Desired_theta2*a2c2; 

  while(theta_2_count > encoder_2){
    encoder_2 = megaEncoderCounter.YAxisGetCount();
    M2Forward(speed2);
   }

  while(theta_2_count < encoder_2){
    encoder_2 = megaEncoderCounter.YAxisGetCount();
    M2Backward(speed2);
  }
 
  M2stop();
  Serial.println(" done !");
}