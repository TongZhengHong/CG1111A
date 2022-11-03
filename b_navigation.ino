void move_forward() {
  left_motor.run(-FORWARD_SPEED);
  right_motor.run(FORWARD_SPEED);
}

void move_forward_correction(int correction) {
  left_motor.run(-FORWARD_SPEED + LEFT_DEVIATION - correction);
  right_motor.run(FORWARD_SPEED - RIGHT_DEVIATION - correction);
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

float calculate_pid() {
  float wall_dist = get_ultrasonic_distance();
  float filtered_dist = apply_low_pass_filter(wall_dist);

  // If wall is not present, return 0
  if (wall_dist == 0.0 or wall_dist > PID_SETPOINT * 1.5) {
    pid_i_mem = 0.0; prev_pid_error = 0.0;
    return 0.0;
  }

  float pid_error = filtered_dist - PID_SETPOINT; // Calculate PID error
  pid_i_mem += PID_I_GAIN * pid_error;
  if (pid_i_mem > PID_MAX_I) pid_i_mem = PID_MAX_I;
  else if (pid_i_mem < PID_MAX_I * -1) pid_i_mem = PID_MAX_I * -1;

  float pid_output = PID_P_GAIN * pid_error + pid_i_mem + PID_D_GAIN * (pid_error - prev_pid_error);
  if (pid_output > PID_MAX_OUTPUT) pid_output = PID_MAX_OUTPUT;
  else if (pid_output < PID_MAX_OUTPUT * -1) pid_output = PID_MAX_OUTPUT * -1;

#ifndef DEBUG_PID
  Serial.print("PID Output: ");
  Serial.println(pid_output);
#endif

  prev_pid_error = pid_error;
  return pid_output;
}

void turn_left_time(int duration) {
  turn_left();
  delay(duration);

  stop_moving();
  global_state = FORWARD;

}

void turn_right_time(int duration) {
  turn_right();
  delay(duration);

  stop_moving();
  global_state = FORWARD;
}

void compound_turn_left() {
  // First turn
  turn_left();
  delay(TURN_LEFT_TIME_MS);

  // Move forward
  stop_moving();
  move_forward();
  delay(STRAIGHT_LEFT_TIME_MS);
  stop_moving();

  // Second turn
  turn_left();
  delay(TURN_LEFT_TIME_MS);

  stop_moving();
  global_state = FORWARD;
}

void compound_turn_right() {
  // First turn
  turn_right();
  delay(TURN_RIGHT_TIME_MS);

  // Move forward
  stop_moving();
  move_forward();
  delay(STRAIGHT_RIGHT_TIME_MS);
  stop_moving();
  
  // Second turn
  turn_right();
  delay(TURN_RIGHT_TIME_MS);

  stop_moving();
  global_state = FORWARD;
}

// Can consider using???
//void extra_turn_correction() {
//  if (get_ultrasonic_distance() < PID_SETPOINT * 1.5) 
//    return; // Wall detected!
//
//  checkpoint_sound();
//  turn_right(); // Turn RIGHT for ultrasonic to be closer to wall
//  delay(EXTRA_TURN_TIME_MS);
//  stop_moving();
//}
