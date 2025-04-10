const int BUTTON_PIN = 2;
const int LED_PIN = 13;
const int MOTOR_PIN = 9;

bool systemActive = false;
unsigned long timerStart;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  // Button press detection
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      handleButtonPress();
      while(digitalRead(BUTTON_PIN) == LOW); // Wait for release
    }
  }

  // 15-second timer logic
  if (systemActive) {
    if (millis() - timerStart >= 15000) {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(MOTOR_PIN, HIGH); // Activate motor
    }
  }
}

void handleButtonPress() {
  if (!systemActive) {
    // Start sequence
    systemActive = true;
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(MOTOR_PIN, LOW);
    timerStart = millis();
  } else {
    // Emergency stop
    systemActive = false;
    digitalWrite(LED_PIN, LOW);
    digitalWrite(MOTOR_PIN, LOW);
  }
} 
   