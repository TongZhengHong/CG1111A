#include <MeMCore.h>

#define TURNING_TIME_MS 300
#define FORWARD_SPEED 255
#define TURNING_SPEED 20

MeBuzzer buzzer;
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeLineFollower lineFinder(PORT_2);

void setup() {
  Serial.begin(9600);
  celebrate();
}

void loop() {
  Serial.println("Test!");
  delay(500);
}
