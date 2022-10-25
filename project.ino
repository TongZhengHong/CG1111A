#include <EEPROM.h>
#include <MeMCore.h>

#define DEBUG_ULTRASONIC
#define DEBUG_FILTER
#define DEBUG_DETECT_WALL
#define DEBUG_INFRARED
#define DEBUG_COLOR
#define DEBUG_PID

#define LEFT_FORWARD_SPEED 200
#define RIGHT_FORWARD_SPEED 180

#define TURN_LEFT_SPEED 125
#define TURN_RIGHT_SPEED 125
#define TURN_TIME_MS 700
#define UTURN_TIME_MS 1250
#define STRAIGHT_TIME_MS 1000

#define WHITE 0
#define BLACK 1
#define GREY 2

#define R 0 
#define G 1
#define B 2

#define D1 A2
#define D2 A3

MeBuzzer buzzer;
MeRGBLed RGBled(0,30);
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);
MeLineFollower line_finder(PORT_2);

float PID_P_GAIN = 30.0;
float PID_I_GAIN = 0.0;
float PID_D_GAIN = 150.0;

float PID_SETPOINT = 10.0;
float PID_MAX_I = 15.0;
float PID_MAX_OUTPUT = 50.0;

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

int currentColor[3] = {0,0,0};

enum Color {
  C_RED, C_GREEN, C_ORANGE, C_PURPLE, C_BLUE, C_WHITE
};

enum Motion {
  TURN_LEFT, TURN_RIGHT, U_TURN, TWO_LEFT, TWO_RIGHT, CHALLENGE, FORWARD
};
Motion state = FORWARD; 

long start_turn_time = 0;

enum CompoundTurnStage {
  FIRST_TURN, STRAIGHT, SECOND_TURN
};
CompoundTurnStage turn_stage = FIRST_TURN;

void setup() {
  Serial.begin(9600);
  RGBled.setpin(13);
  
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  read_eeprom();
  
//  celebrate();
  delay(1000);
  start_turn_time = millis();
}

void loop() {
  if (state == FORWARD) {
    float correction = calculate_pid();
    move_forward_correction((int) correction);

    if (has_reached_waypoint()) {
      stop_moving();
      state = CHALLENGE;
    }
    
  } else if (state == CHALLENGE) {
    read_color_sensor(); // Updates currentColor[3] with R,G,B values
    int predicted_color = match_color();
    
    read_color_sensor(); // Updates currentColor[3] with R,G,B values
    int second_color = match_color();
    
    if (predicted_color != second_color) {
      read_color_sensor(); // Updates currentColor[3] with R,G,B values
      predicted_color = match_color();
    }

//    read_color_sensor(); // Updates currentColor[3] with R,G,B values
//    int predicted_color = match_color();
    
    display_color(predicted_color); 
//    checkpoint_sound();

    if (predicted_color == C_WHITE) {
      stop_moving();
      celebrate();
    } else {
      state = static_cast<Motion>(predicted_color);
      start_turn_time = millis(); // Set start time of turn
    }
    
  } else if (state == TURN_LEFT or state == TURN_RIGHT) {
    bool within_time = millis() - start_turn_time < TURN_TIME_MS;
    bool not_close_to_wall = (millis() - start_turn_time > (float) TURN_TIME_MS*0.5) and not is_close_to_wall(state);
    
    if (within_time and true) {
      if (state == TURN_LEFT) turn_left();
      else if (state == TURN_RIGHT) turn_right();
      
    } else {
      stop_moving();
      state = FORWARD;
    }
    
  } else if (state == U_TURN) {
    bool within_time = millis() - start_turn_time < UTURN_TIME_MS;
    bool not_close_to_wall = (millis() - start_turn_time > (TURN_TIME_MS*0.75)) and not is_close_to_wall(state);
    
    if (within_time and true) 
      turn_left();
    else {
      stop_moving();
      state = FORWARD;
    }
    
  } else if (state == TWO_LEFT or state == TWO_RIGHT) {
    if (turn_stage == FIRST_TURN) {
      bool within_time = millis() - start_turn_time < TURN_TIME_MS;
//      bool not_close_to_wall = (millis() - start_turn_time > (TURN_TIME_MS*0.5)) and not is_close_to_wall(state);
      
      if (within_time and true) {
        if (state == TWO_LEFT) turn_left();
        else if (state == TWO_RIGHT) turn_right();
        
      } else {
        stop_moving();
        turn_stage = STRAIGHT;
        start_turn_time = millis();
      }
      
    } else if (turn_stage == STRAIGHT) {
      if (millis() - start_turn_time < STRAIGHT_TIME_MS) {
        move_forward();
//        float correction = calculate_pid();
//        move_forward_correction((int) correction);
        
      } else {
        stop_moving();
        turn_stage = SECOND_TURN;
        start_turn_time = millis();
      }
      
    } else if (turn_stage == SECOND_TURN) {
      bool within_time = millis() - start_turn_time < TURN_TIME_MS;
      bool not_close_to_wall = (millis() - start_turn_time > (TURN_TIME_MS*0.5)) and not is_close_to_wall(state);
      
      if (within_time and true) {
        if (state == TWO_LEFT) turn_left();
        else if (state == TWO_RIGHT) turn_right();
        
      } else {
        stop_moving();
        state = FORWARD;
        turn_stage == FIRST_TURN;
      }
    }
  }
  
  delay(10);
}
