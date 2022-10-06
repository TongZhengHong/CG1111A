bool has_reached_waypoint() {
  int sensorState = lineFinder.readSensors();
  return sensorState == S1_OUT_S2_OUT;
}
