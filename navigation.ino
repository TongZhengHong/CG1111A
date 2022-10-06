void moveFoward() {
  leftMotor.run(-FORWARD_SPEED); // Stop left motor 
  rightMotor.run(FORWARD_SPEED); // Stop right motor
  delay(100);
}

void turnLeft() {
  leftMotor.run(FORWARD_SPEED); // Stop left motor 
  rightMotor.run(FORWARD_SPEED); // Stop right motor
  delay(100);
}

void turnRight() {
  leftMotor.run(-FORWARD_SPEED); // Stop left motor 
  rightMotor.run(-FORWARD_SPEED); // Stop right motor
  delay(100);
}

void stopMoving() {
  leftMotor.stop(); // Stop left motor 
  rightMotor.stop(); // Stop right motor
  delay(100);
}
