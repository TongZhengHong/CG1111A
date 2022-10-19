void display_color(int c) {
  
  int ledColors[6][3] = {
    {255, 0, 0},    // RED
    {0, 255, 0},    // GREEN
    {255, 100, 0},  // ORANGE
    {200, 0, 200},  // PURPLE
    {0, 0, 255},    // BLUE
    {255,255, 255}  // WHITE
  };

  RGBled.setColor(ledColors[c][R], ledColors[c][G], ledColors[c][B]);
  RGBled.show();
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
