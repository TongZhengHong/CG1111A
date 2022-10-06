#define TIMEOUT 2000 // Max microseconds to wait; choose according to max distance of wall
#define SPEED_OF_SOUND 340 // metres per second

#define ULTRASONIC 12

enum color {
  RED, GREEN, ORANGE, PURPLE, BLUE, WHITE 
};

int getUltrasonicDistance() {
  pinMode(ULTRASONIC, OUTPUT);
  
  digitalWrite(ULTRASONIC, LOW); 
  delayMicroseconds(2); 
  digitalWrite(ULTRASONIC, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(ULTRASONIC, LOW);
  
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // microseconds

  // m -> cm ( * 100) and microsecond -> second ( / 1000000)
  double conversion = 100 / 1000000; 
  int distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;

#ifndef DEBUG_ULTRASONIC
  Serial.print("Ultrasonic distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
#endif

  return distance_cm;
}

int getInfraredDistance() {
  return 0;
}

bool hasReachedWaypoint() {
  int sensorState = lineFinder.readSensors();
  return sensorState == S1_OUT_S2_OUT;
}
