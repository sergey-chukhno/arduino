#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

const int BUTTON = 2;
unsigned long start = 0;
bool isRunning = false;
bool lastStateButton = LOW;
unsigned long lastDisplayUpdate = 0; // Track last update time

void setup() {
  lcd.begin(16, 2);
  pinMode(BUTTON, INPUT_PULLUP);
  lcd.print("Chronometer :");
}

void loop() {
  bool stateButton = digitalRead(BUTTON);
  
  if (stateButton != lastStateButton) {
    delay(50);
    stateButton = digitalRead(BUTTON); // Debounce check
    if (stateButton == LOW) {
      isRunning = !isRunning;
      if (isRunning) {
        start = millis();
        lcd.setCursor(0, 1);
        lcd.print("                "); // Clear line only when starting
      }
    }
    lastStateButton = stateButton;
  }

  if (isRunning) {
    unsigned long currentMillis = millis();
    // Update display only once per second
    if (currentMillis - lastDisplayUpdate >= 1000) {
      lastDisplayUpdate = currentMillis;
      unsigned long passed = currentMillis - start;
      int hours = passed / 3600000;
      int minuts = (passed % 3600000) / 60000;
      int seconds = (passed % 60000) / 1000;

      lcd.setCursor(0, 1);
      lcd.print(
        (hours < 10 ? "0" : "") + String(hours) + ":" +
        (minuts < 10 ? "0" : "") + String(minuts) + ":" +
        (seconds < 10 ? "0" : "") + String(seconds)
      );
    }
  }
}