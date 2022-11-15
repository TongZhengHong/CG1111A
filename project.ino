#include <EEPROM.h>
#include <MeMCore.h>

#define DEBUG_ULTRASONIC
#define DEBUG_FILTER
#define DEBUG_INFRARED
#define DEBUG_COLOR
#define DEBUG_PID

/* Hard-coded values for navigation */
#define FORWARD_SPEED 180 
#define RIGHT_DEVIATION 0
#define LEFT_DEVIATION 15

#define TURN_SPEED 125
#define TURN_RIGHT_TIME_MS 625
#define TURN_LEFT_TIME_MS 625
#define TURN_CORRECTION_TIME_MS 100
#define UTURN_TIME_MS 1125
#define STRAIGHT_RIGHT_TIME_MS 975
#define STRAIGHT_LEFT_TIME_MS 1050
#define TWO_RIGHT_TURN_TIME_MS 630    // Compound turning for Blue Tile
#define TWO_LEFT_TURN_TIME_MS 580     // Compound turning for Purple Tile, under-turn to maintain readable distance from wall

#define LDR_PIN 0
#define IR_PIN 1
#define D1 A2     // 2-to-4 decoder 1A select input
#define D2 A3     // 2-to-4 decoder 1B select input

/* Initialisation of mBot ports */
MeBuzzer buzzer;
MeRGBLed RGBled(0, 30);
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);
MeLineFollower line_finder(PORT_2);

/* Initialisation of color-sensing constants */
const char R = 0, G = 1, B = 2;
const char white = 0, black = 1, grey = 2;

/* PID Initialisation, utilised under b_navigation.ino */
const float PID_P_GAIN = 30.0;        // Custom proportional tuning - Correcting a setpoint proportional to the difference
const float PID_I_GAIN = 0.0;         // Custom integral tuning - Cumulating error result from P_GAIN to increase the correction factor
const float PID_D_GAIN = 800.0;       // Custom derivative tuning - Minimise overshooting by slowing correction factor applied as target is approached

const float PID_SETPOINT = 10.0;      // mbot maintain 10cm from the right wall
const float PID_MAX_I = 15.0;         // Not revelant because I_GAIN set to 0 in this scenario
const float PID_MAX_OUTPUT = 60.0;    // Prevent over-compensation of error such that it exceeds motor's speed range

float pid_i_mem = 0.0, prev_pid_error = 0.0;  

int balanceArray[3][3] = {
  {0, 0, 0}, // WHITE
  {0, 0, 0}, // BLACK
  {0, 0, 0}, // GREY
};

enum Color {  
  C_RED, C_GREEN, C_ORANGE, C_PURPLE, C_BLUE, C_WHITE
};
int currentColor[3] = {0, 0, 0};

enum Motion {
  TURN_LEFT, TURN_RIGHT, U_TURN, TWO_LEFT, TWO_RIGHT, CHALLENGE, FORWARD, FINISH
};
Motion global_state = FORWARD;  // Default state of motion is FORWARD (with PID)

void setup() {
  Serial.begin(9600);
  RGBled.setpin(13);  // In-built Arduino board RGB lighting to feedback colour tile sensed by colour sensor

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  read_eeprom();
}

void loop() {
  if (global_state == FORWARD) {
    float correction = calculate_pid();
    if (correction == -1) move_forward();             // Applies deviation to the forward if no wall detected (found in b_navigation.ino)
    else move_forward_correction((int) correction);   // (int) as arduino motor speed is in int

    if (has_reached_waypoint()) {                     // Black line is detected, change state to CHALLENGE (sense the tile)
      stop_moving();
      global_state = CHALLENGE;
    }

  } else if (global_state == CHALLENGE) {
    read_color_sensor();                              // Updates currentColor[3] with R,G,B values
    int predicted_color = match_color();              // Match the colour to our list of determined logic in a_utility.ino
    display_color(predicted_color);                   // Display the colour mbot thinks is correct

    if (predicted_color == C_WHITE) {
      stop_moving();
      celebrate();
      global_state = FINISH; // Terminates program

    } else global_state = static_cast<Motion>(predicted_color);   // Convert predicted_colour to motion to take

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
