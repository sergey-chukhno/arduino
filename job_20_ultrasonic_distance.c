#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

// Analog pins for joystick
const int VRx = A0;
const int VRy = A1;

// Thresholds to determine direction
const int MIN_THRESHOLD = 400;
const int MAX_THRESHOLD = 600;

unsigned long lastUpdate = 0;
String currentDirection = "";

void setup() {
  lcd.begin(16, 2);
  lcd.print("Direction :");
}

void loop() {
  // Reading analog values
  int xValue = analogRead(VRx);
  int yValue = analogRead(VRy);

  // Determine direction
  String newDirection = getDirection(xValue, yValue);

  // Updating screen
  if (newDirection != currentDirection && millis() - lastUpdate > 200) {
    currentDirection = newDirection;
    lastUpdate = millis();
    
    lcd.setCursor(0, 1);
    lcd.print("                "); 
    lcd.setCursor(0, 1);
    lcd.print(currentDirection);
  }
}

String getDirection(int x, int y) {
  // Giving priority to horizontal axis
  if (x < MIN_THRESHOLD) return "Gauche     ";
  if (x > MAX_THRESHOLD) return "Droite    ";
  
  // Directions verticales
  if (y < MIN_THRESHOLD) return "Haut      ";
  if (y > MAX_THRESHOLD) return "Bas       ";
  
  return "Centre    "; // Joystick in neutral position
}