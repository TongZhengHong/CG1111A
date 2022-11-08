/*
  Periperhals: buzzer sound & on-board arduino display colour
*/

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
  RGBled.show();  // show colours on-board arduino
}

/*TODO: Buzzer 5 marks!*/
void checkpoint_sound() {
  buzzer.tone(262, 250);
  buzzer.tone(196, 125);
  buzzer.tone(196, 125);
  buzzer.tone(220, 250);
  buzzer.tone(196, 250);
  buzzer.tone(0, 250);
  buzzer.tone(247, 250);
  buzzer.tone(262, 250);
  buzzer.noTone();
}

void celebrate() {
  buzzer.tone(523, 200);
  buzzer.tone(659, 200);
  buzzer.tone(784, 200);
  buzzer.tone(659, 150);
  buzzer.tone(784, 400);
  buzzer.noTone();
}
