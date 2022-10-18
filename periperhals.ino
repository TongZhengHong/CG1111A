void display_color(int c) {
  if (c == -1) return;
  
  int ledColors[5][3] = {
    {255, 0, 0},    // RED
    {0, 255, 0},    // GREEN
    {255, 165, 0},  // ORANGE
    {200, 0, 200},  // PURPLE
    {0, 0, 255}     // BLUE
  };

  RGBled.setColorAt(0, ledColors[c][R], ledColors[c][G], ledColors[c][B]);
}

void checkpoint_sound() {
  
}

void celebrate() {
  buzzer.tone(523, 200);
  buzzer.tone(659, 200);
  buzzer.tone(784, 200);
  buzzer.tone(659, 150);
  buzzer.tone(784, 400);
  buzzer.noTone();
}
