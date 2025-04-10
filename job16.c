LiquidCrystal lcd(12, 11, 6, 7, 8, 9); // RS, E, D4, D5, D6, D7

// Time variables
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second
int hours = 0;
int minuts = 0;
int seconds = 0;

void setup() {
  lcd.begin(16, 2); // 
  lcd.print("Current time:");
}

void loop() {
  unsigned long currentMillis = millis();

  // Update time each second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    seconds++;

    // Handling time incrementation
    if (seconds >= 60) {
      seconds = 0;
      minuts++;
      if (minuts >= 60) {
        minuts = 0;
        hours++;
        if (hours >= 24) {
          hours = 0;
        }
      }
    }

    // Display time on LCD
    lcd.setCursor(0, 1); // Second line, first column
    
    // Format HH:MM:SS with zeros at the beginning
    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", hours, minuts, seconds);
    lcd.print(buffer);
  }
}