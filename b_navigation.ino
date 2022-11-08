/*
  b_navigation.ino: Motion for the motors, including PID calculation & complex turning algorithms to overcome compound turn andn fringe left turn cases
*/

/*Default motion*/
void move_forward() {
  left_motor.run(-FORWARD_SPEED + LEFT_DEVIATION);
  right_motor.run(FORWARD_SPEED - RIGHT_DEVIATION);
}

void move_forward_correction(int correction) {
  left_motor.run(-FORWARD_SPEED - correction);
  right_motor.run(FORWARD_SPEED - correction);
}

void turn_left() {
  left_motor.run(TURN_SPEED);
  right_motor.run(TURN_SPEED);
}

void turn_right() {
  left_motor.run(-TURN_SPEED);
  right_motor.run(-TURN_SPEED);
}

void stop_moving() {
  left_motor.stop();
  right_motor.stop();
}

/*PID calculation algorithm*/
float calculate_pid() {
  float wall_dist = get_ultrasonic_distance();
  float filtered_dist = apply_low_pass_filter(wall_dist);   // smoothen input to reduce jitters?

  // If wall is not present, return -1
  if (wall_dist == 0.0 or wall_dist > PID_SETPOINT * 1.5) {
    pid_i_mem = 0.0; prev_pid_error = 0.0;
    return -1;
  }

  float pid_error = filtered_dist - PID_SETPOINT; // Calculate PID error
  /*I gain which is not relevant*/
  pid_i_mem += PID_I_GAIN * pid_error;
  if (pid_i_mem > PID_MAX_I) pid_i_mem = PID_MAX_I;
  else if (pid_i_mem < PID_MAX_I * -1) pid_i_mem = PID_MAX_I * -1;

  /*PD gain*/
  float pid_output = PID_P_GAIN * pid_error + pid_i_mem + PID_D_GAIN * (pid_error - prev_pid_error);  // output takes in P and D correction 
  if (pid_output > PID_MAX_OUTPUT) pid_output = PID_MAX_OUTPUT;   // prevent overcompensation of error such that speed of motor is exceeded. check...
  else if (pid_output < PID_MAX_OUTPUT * -1) pid_output = PID_MAX_OUTPUT * -1; // in the reverse direction of overcompensation

#ifndef DEBUG_PID
  Serial.print("PID Output: ");
  Serial.println(pid_output);
#endif

  prev_pid_error = pid_error;
  return pid_output;
}

/*Complex Turning algorithms*/
void turn_left_time(int duration) {
  move_forward();
  delay(TURN_CORRECTION_TIME_MS);   // move forward slightly before turning left to align closer to the wall
  
  turn_left();
  delay(duration);

  stop_moving();
  global_state = FORWARD;
}

void turn_right_time(int duration) {
  move_forward();
  delay(TURN_CORRECTION_TIME_MS); // move forward slightly before turning right to align closer to the wall

  turn_right();
  delay(duration);

  stop_moving();
  global_state = FORWARD;
}

void uturn_time(int duration) {
  turn_left();
  delay(duration);

  stop_moving();
  global_state = FORWARD;
}

void compound_turn_left() {
  // First turn
  turn_left();
  delay(TWO_LEFT_TURN_TIME_MS); // purposely tuned to under-turn when turning left for compound to align to more closely to the wall on side of ultrasonic 

  // Move forward
  stop_moving();
  move_forward();
  delay(STRAIGHT_LEFT_TIME_MS);
  stop_moving();

  // Second turn
  turn_left();
  delay(TWO_LEFT_TURN_TIME_MS);

  stop_moving();
  global_state = FORWARD;
}

void compound_turn_right() {
  // First turn
  turn_right();
  delay(TWO_RIGHT_TURN_TIME_MS); // purposely tuned to under-turn when turning right for compound to align to more closely to the wall on side of ultrasonic

  // Move forward
  stop_moving();
  move_forward();
  delay(STRAIGHT_RIGHT_TIME_MS);
  stop_moving();
  
  // Second turn
  turn_right();
  delay(TWO_RIGHT_TURN_TIME_MS);

  stop_moving();
  global_state = FORWARD;
}
