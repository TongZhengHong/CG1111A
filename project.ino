#include <EEPROM.h>
#include <MeMCore.h>

//#define DEBUG_ULTRASONIC
#define DEBUG_INFRARED
#define DEBUG_COLOR
//#define DEBUG_PID

#define TURNING_TIME_MS 300
#define LEFT_FORWARD_SPEED 200
#define RIGHT_FORWARD_SPEED 200
#define TURNING_SPEED 20

MeBuzzer buzzer;
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);
MeLineFollower line_finder(PORT_2);

float PID_P_GAIN = 25.0;
float PID_I_GAIN = 0.005;
float PID_D_GAIN = 500.0;

float PID_SETPOINT = 8.0;
float PID_MAX_I = 15.0;
float PID_MAX_OUTPUT = 50.0;

float pid_i_mem = 0, prev_pid_error = 0;

enum motion {
  FORWARD, TURN_RIGHT, TURN_LEFT, U_TURN, TWO_RIGHT, TWO_LEFT, CHALLENGE 
};
motion state = FORWARD;

void setup() {
  Serial.begin(9600);
//  celebrate();
  delay(1000);

  // Start moving robot forward to get going
  move_foward();
}

void loop() {
  if (state == FORWARD) {
    float correction = calculate_pid();
    move_forward_correction((int) correction);

//    if (has_reached_waypoint()) {
//      stop_moving();
//      state = CHALLENGE;
//    }
    
  } else if (state == CHALLENGE) {
    
    
  } else if (state == TURN_RIGHT) { 
    
    
  } else if (state == TURN_LEFT) {

    
  } else if (state == U_TURN) {
    
  } else if (state == TWO_RIGHT) {
    
  } else if (state == TWO_LEFT) {

  } 
  
  delay(10);
}
