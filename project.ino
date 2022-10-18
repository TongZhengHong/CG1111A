#include <EEPROM.h>
#include <MeMCore.h>

#define DEBUG_ULTRASONIC
#define DEBUG_INFRARED
//#define DEBUG_COLOR
#define DEBUG_PID

#define TURNING_TIME_MS 300
#define LEFT_FORWARD_SPEED 200
#define RIGHT_FORWARD_SPEED 200
#define TURNING_SPEED 20

#define WHITE 0
#define BLACK 1
#define GREY 2

#define R 0 
#define G 1
#define B 2

MeBuzzer buzzer;
MeRGBLed RGBled(0,30);
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

int balanceArray[3][3] = {
  {0, 0, 0}, // WHITE
  {0, 0, 0}, // BLACK
  {0, 0, 0}, // GREY
};

int colorsArray[5][3] = {
  {0, 0, 0}, // RED
  {0, 0, 0}, // GREEN
  {0, 0, 0}, // ORANGE
  {0, 0, 0}, // PURPLE
  {0, 0, 0}, // BLUE
};

int currentColor[3] = {0,0,0};

enum Color {
  C_RED, C_GREEN, C_ORANGE, C_PURPLE, C_BLUE
};

enum Motion {
  TURN_LEFT, TURN_RIGHT, U_TURN, TWO_LEFT, TWO_RIGHT, CHALLENGE, FORWARD
};
Motion state = CHALLENGE; // FORWARD

void setup() {
  Serial.begin(9600);
  RGBled.setpin(13);
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  read_eeprom();
  
//  celebrate();
  delay(1000);

  // Start moving robot forward to get going
//  move_foward();
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
    int predictedColor = read_color_sensor();
    display_color(predictedColor);

    if (predictedColor > 0)
      state = static_cast<Motion>(predictedColor);
    
  } else if (state == TURN_LEFT) {
    
    
  } else if (state == TURN_RIGHT) {

    
  } else if (state == U_TURN) {

    
  } else if (state == TWO_LEFT) {

    
  } else if (state == TWO_RIGHT) {


  } 
  
  delay(10);
}
