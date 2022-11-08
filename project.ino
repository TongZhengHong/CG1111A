#include <EEPROM.h>
#include <MeMCore.h>

#define DEBUG_ULTRASONIC
#define DEBUG_FILTER
#define DEBUG_INFRARED
#define DEBUG_COLOR
#define DEBUG_PID

#define FORWARD_SPEED 180 
#define RIGHT_DEVIATION 0
#define LEFT_DEVIATION 15

#define TURN_SPEED 125
#define TURN_RIGHT_TIME_MS 625
#define TURN_LEFT_TIME_MS 625
#define TURN_CORRECTION_TIME_MS 100
#define UTURN_TIME_MS 1125
#define STRAIGHT_RIGHT_TIME_MS 975
#define STRAIGHT_LEFT_TIME_MS 1100
#define TWO_RIGHT_TURN_TIME_MS 630
#define TWO_LEFT_TURN_TIME_MS 590

#define D1 A2
#define D2 A3

MeBuzzer buzzer;
MeRGBLed RGBled(0, 30);
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);
MeLineFollower line_finder(PORT_2);

const char R = 0, G = 1, B = 2;
const char white = 0, black = 1, grey = 2;

const float PID_P_GAIN = 30.0;
const float PID_I_GAIN = 0.0;
const float PID_D_GAIN = 800.0;

const float PID_SETPOINT = 10.0;
const float PID_MAX_I = 15.0;
const float PID_MAX_OUTPUT = 60.0;

float pid_i_mem = 0.0, prev_pid_error = 0.0;

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

int currentColor[3] = {0, 0, 0};

enum Color {
  C_RED, C_GREEN, C_ORANGE, C_PURPLE, C_BLUE, C_WHITE
};

enum Motion {
  TURN_LEFT, TURN_RIGHT, U_TURN, TWO_LEFT, TWO_RIGHT, CHALLENGE, FORWARD, FINISH
};
Motion global_state = FORWARD;

void setup() {
  Serial.begin(9600);
  RGBled.setpin(13);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  read_eeprom();
}

void loop() {
  if (global_state == FORWARD) {
    float correction = calculate_pid();
    if (correction == -1) move_forward(); // Applies deviation to the forward if no wall detected
    else move_forward_correction((int) correction);

    if (has_reached_waypoint()) {
      stop_moving();
      global_state = CHALLENGE;
    }

  } else if (global_state == CHALLENGE) {
    read_color_sensor(); // Updates currentColor[3] with R,G,B values
    int predicted_color = match_color();
    display_color(predicted_color);

    if (predicted_color == C_WHITE) {
      stop_moving();
      celebrate();
      global_state = FINISH; // Terminates program

    } else global_state = static_cast<Motion>(predicted_color);

  } else if (global_state == TURN_LEFT) 
    turn_left_time(TURN_LEFT_TIME_MS);

  else if (global_state == TURN_RIGHT) 
    turn_right_time(TURN_RIGHT_TIME_MS);
    
  else if (global_state == U_TURN) 
    uturn_time(UTURN_TIME_MS);  // Turn LEFT for U-turn

  else if (global_state == TWO_LEFT) 
    compound_turn_left();
    
  else if (global_state == TWO_RIGHT) 
    compound_turn_right();

  delay(10);
}
