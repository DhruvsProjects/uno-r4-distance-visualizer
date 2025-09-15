#include "NewPing.h"
#include "Arduino_LED_Matrix.h"

#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
ArduinoLEDMatrix matrix;

// 8x12 display buffer
byte cmOut[8][12] = {0};

// Digit bitmaps (3x5 font for digits 0–9)
byte Digits[5][30] = {
  {1,1,1,  0,0,1,  1,1,1,  1,1,1,  1,0,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1},
  {1,0,1,  0,0,1,  0,0,1,  0,0,1,  1,0,1,  1,0,0,  1,0,0,  0,0,1,  1,0,1,  1,0,1},
  {1,0,1,  0,0,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  1,1,1,  0,0,1,  1,1,1,  1,1,1},
  {1,0,1,  0,0,1,  1,0,0,  0,0,1,  0,0,1,  0,0,1,  1,0,1,  0,0,1,  1,0,1,  0,0,1},
  {1,1,1,  0,0,1,  1,1,1,  1,1,1,  0,0,1,  1,1,1,  1,1,1,  0,0,1,  1,1,1,  1,1,1}
};

// Draw a single digit at (s_x, s_y)
void displayDigit(int d, int s_x, int s_y) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {
      cmOut[j + s_y][i + s_x] = Digits[j][i + d * 3];
    }
  }
}

void clearMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 12; j++) {
      Time[i][j] = 0;
    }
  }
}

void setup() {
  Serial.begin(9600);
  matrix.begin();
}

void loop() {
  unsigned int distance = sonar.ping_cm();

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");

  clearMatrix();

  // Break distance into digits
  int hundreds = distance / 100;
  int tens = (distance / 10) % 10;
  int ones = distance % 10;

  // Fixed positions for each digit
  int hundredsX = 0;
  int tensX     = 4;
  int onesX     = 8;

  // Draw digits if applicable
  if (hundreds > 0) displayDigit(hundreds, hundredsX, 1);
  if (hundreds > 0 || tens > 0) displayDigit(tens, tensX, 1);
  displayDigit(ones, onesX, 1);

  // Render final bitmap
  matrix.renderBitmap(cmOut, 8, 12);

  delay(500);
}
