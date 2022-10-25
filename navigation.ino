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
  float filtered_dist = apply_low_pass_filter(wall_dist);
  
  if (wall_dist == 0.0 or wall_dist > PID_SETPOINT*1.5) {
    pid_i_mem = 0.0; prev_pid_error = 0.0;
    return 0.0;
  }
  
  float pid_error = filtered_dist - PID_SETPOINT;
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

float prev_dist = 0.0, prev_prev_dist = 0.0;
bool is_close_to_wall(int state) {
  float lower, upper;
  
  if (state == TURN_RIGHT or state == TWO_RIGHT) {
    lower = 6.0; upper = 7.0; 
  } else {
    lower = 9.5; upper = 10.5;
  }

  int counter = 0;
  float reading = get_ultrasonic_distance();
  
  if (reading > lower and reading < upper) counter++;
  if (prev_dist > lower and prev_dist < upper) counter++;
//  if (prev_prev_dist > lower and prev_prev_dist < upper) counter++;

  prev_prev_dist = prev_dist;
  prev_dist = reading;

#ifndef DEBUG_DETECT_WALL
  if (counter == 2) Serial.println("WALL DETECTED!");
  else Serial.println("NO WALL");
#endif

  return counter == 2;
}
