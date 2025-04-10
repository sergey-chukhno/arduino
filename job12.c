#include <LiquidCrystal.h>

// Initialize LCD in 4-bit mode: RS=12, E=11, DB4=6, DB5=7, DB6=8, DB7=9
LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

void setup() {
  lcd.begin(16, 2);      // Initialize 16x2 LCD
  lcd.print("Hello World!");
}

void loop() {}