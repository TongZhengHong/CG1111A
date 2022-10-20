void move_forward() {
  left_motor.run(-LEFT_FORWARD_SPEED); 
  right_motor.run(RIGHT_FORWARD_SPEED); 
}

void move_forward_correction(int correction) {
  left_motor.run(-LEFT_FORWARD_SPEED - correction); 
  right_motor.run(RIGHT_FORWARD_SPEED - correction); 
}

void turn_left() {
  left_motor.run(TURN_LEFT_SPEED); 
  right_motor.run(TURN_RIGHT_SPEED); 
}

void turn_right() {
  left_motor.run(-TURN_LEFT_SPEED); 
  right_motor.run(-TURN_RIGHT_SPEED); 
}

void stop_moving() {
  left_motor.stop(); 
  right_motor.stop(); 
}

float calculate_pid() {
  float wall_dist = get_ultrasonic_distance();
  if (wall_dist == 0.0 or wall_dist > PID_SETPOINT*2) {
    pid_i_mem = 0; prev_pid_error = 0;
    return 0.0;
  }
  
  float pid_error = wall_dist - PID_SETPOINT;
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
