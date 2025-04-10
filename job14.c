#include <LiquidCrystal.h>

// Pin configuration for LCD
LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

// Button configuration
const int buttonPin = 2;  // Pin for the button
int buttonState = 0;      // Variable to store the button state
int lastButtonState = 0;  // Variable to store the last button state
bool counterRunning = true;  // Flag to check if the counter is running
unsigned long counter = 0;  // Counter variable

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);  // 16x2 LCD

  // Display "Sergey" on the first line
  lcd.setCursor(0, 0);  // Set cursor to the beginning of the first line
  lcd.print("Sergey");

  // Set up the button pin
  pinMode(buttonPin, INPUT_PULLUP);  // Button is active LOW
}

void loop() {
  // Read the button state
  buttonState = digitalRead(buttonPin);

  // Check for button press (button is active LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Toggle counter running state when button is pressed
    counterRunning = !counterRunning;
    delay(200);  // Debounce delay
  }

  lastButtonState = buttonState;

  // If counter is running, increment it
  if (counterRunning) {
    counter++;
  }

  // Clear the second line and display the counter
  lcd.setCursor(0, 1);  // Move the cursor to the second line
  lcd.print("Counter: ");
  lcd.print(counter);

  // Wait for a short time to update the counter every second
  delay(1000);
}