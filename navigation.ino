void move_forward() {
  leftMotor.run(-FORWARD_SPEED); // Stop left motor 
  rightMotor.run(FORWARD_SPEED); // Stop right motor
  delay(100);
}

void turn_left() {
  leftMotor.run(FORWARD_SPEED); // Stop left motor 
  rightMotor.run(FORWARD_SPEED); // Stop right motor
  delay(100);
}

void turn_right() {
  leftMotor.run(-FORWARD_SPEED); // Stop left motor 
  rightMotor.run(-FORWARD_SPEED); // Stop right motor
  delay(100);
}

void stop_moving() {
  leftMotor.stop(); // Stop left motor 
  rightMotor.stop(); // Stop right motor
  delay(100);
}
