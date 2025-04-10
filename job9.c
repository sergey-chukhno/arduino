const int BUTTON_PIN = 2;
const int LED_PIN = 9;

// Button variables
unsigned long lastDebounce = 0;
unsigned long delayDebounce = 50;
bool stateButton = HIGH;
bool lastStateButton = HIGH;

// Pressing variables
unsigned long startPressing = 0;
bool isPressed = false;
int counterClicks = 0;
unsigned long lastClick = 0;
const unsigned long timeoutClicks = 400;
const unsigned long timeLongPress = 1000;

// LED state
enum ModeLED { OFF, ON, BLINKING };
ModeLED modeLED = OFF;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Managing the button with release
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastStateButton) {
    lastDebounce = millis();
  }
  
  if ((millis() - lastDebounce) > delayDebounce) {
    if (reading != stateButton) {
      stateButton = reading;
      
      if (stateButton == LOW) { // Start pressing
        startPressing = millis();
        isPressed = true;
      } else { // End of pressing
        isPressed = false;
        if (millis() - startPressing < timeLongPress) {
          counterClicks++;
          lastClick = millis();
        }
      }
    }
  }
  lastStateButton = reading;

  // Detecting long press
  if (isPressed && (millis() - startPressing >= timeLongPress)) {
    modeLED = OFF;
    counterClicks = 0;
  }

  // Treating clicks after timeout
  if (!isPressed && (millis() - lastClick >= timeoutClicks)) {
    if (counterClicks > 0) {
      switch(counterClicks) {
        case 1: modeLED = ON; break;
        case 2: modeLED = ON; break; // Same as one click 
        case 3: modeLED = BLINKING; break;
      }
      counterClicks = 0;
    }
  }

  // Controlling LED
  switch(modeLED) {
    case OFF:
      digitalWrite(LED_PIN, LOW);
      break;
      
    case ON:
      digitalWrite(LED_PIN, HIGH);
      break;
      
    case BLINKING:
      static unsigned long lastChange = 0;
      if (millis() - lastChange >= 500) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastChange = millis();
      }
      break;
  }
}
