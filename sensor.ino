#define TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // metres per second

#define ULTRASONIC 12

enum color {
  C_RED, C_GREEN, C_ORANGE, C_PURPLE, C_BLUE, C_WHITE 
};

float get_ultrasonic_distance() {
  pinMode(ULTRASONIC, OUTPUT);
  
  digitalWrite(ULTRASONIC, LOW); 
  delayMicroseconds(10); 
  digitalWrite(ULTRASONIC, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(ULTRASONIC, LOW);
  
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // microseconds

  // m -> cm ( * 100) and microsecond -> second ( / 1000000)
  float conversion = 100.0 / 1000000.0; 
  float distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;

#ifdef DEBUG_ULTRASONIC
  Serial.print("Ultrasonic distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
#endif

  return distance_cm;
}

int get_infrared_distance() {
  return 0;
}

bool has_reached_waypoint() {
  int sensor_state = line_finder.readSensors();
  return sensor_state == S1_OUT_S2_OUT;
}
