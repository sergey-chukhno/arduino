#include <DHT.h>
#include <LiquidCrystal.h>

// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT11
#define BTN_START 7
#define BTN_DISPLAY 4
#define US_SIG 8
#define BUZZER 10
#define FAN 9
#define LED_BLUE 3
#define LED_RED 5
#define LED_GREEN 6
#define LED_YELLOW 13
#define LUM_PIN A0

// Thresholds
const float TEMP_LOW = 10.0;
const float TEMP_HIGH = 22.0;
const int HUM_THRESHOLD = 60;
const int DISTANCE_ALERT = 50; // cm

// Initialize Components
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, A1, A2, A3, A4);

// System State
bool systemActive = false;
int displayMode = 0;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

void setup() {
  // Initialize pins
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_DISPLAY, INPUT_PULLUP);
  pinMode(US_SIG, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // Start sensors
  dht.begin();
  
  // LCD Setup
  lcd.begin(16, 2);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Press START");
}

float getDistance() {
  // Trigger pulse
  pinMode(US_SIG, OUTPUT);
  digitalWrite(US_SIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_SIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_SIG, LOW);

  // Receive echo
  pinMode(US_SIG, INPUT);
  long duration = pulseIn(US_SIG, HIGH);
  
  // Temperature-compensated speed
  float temp = dht.readTemperature();
  float soundSpeed = 331.4 + (0.6 * temp);
  
  return (duration * soundSpeed) / 20000;
}

void updateClimateControl(float temp, float hum) {
  // Temperature control
  if(temp < TEMP_LOW) {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(FAN, LOW);
  } 
  else if(temp <= TEMP_HIGH) {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(FAN, LOW);
  } 
  else {
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_RED, LOW);
    analogWrite(FAN, 255);
  }

  // Humidity control
  digitalWrite(LED_GREEN, hum < HUM_THRESHOLD ? HIGH : LOW);
  digitalWrite(LED_YELLOW, hum >= HUM_THRESHOLD ? HIGH : LOW);
}

void updateDisplay(float temp, float hum, int lum, float dist) {
  if((millis() - lastDebounceTime) > debounceDelay) {
    if(digitalRead(BTN_DISPLAY) == LOW) {
      displayMode = (displayMode + 1) % 4;
      lastDebounceTime = millis();
      lcd.clear();
    }
  }

  switch(displayMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temp, 1);
      lcd.print("C ");
      lcd.setCursor(0, 1);
      lcd.print("Hum:  ");
      lcd.print(hum, 0);
      lcd.print("% ");
      break;
      
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Luminosity: ");
      lcd.print(lum);
      lcd.print("   ");
      break;
      
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      lcd.print(dist, 1);
      lcd.print("cm ");
      break;
      
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("System Active ");
      lcd.setCursor(0, 1);
      lcd.print("Mode: ");
      lcd.print(systemActive ? "ON " : "OFF");
      break;
  }
}

void handleBuzzer(float distance) {
  if(distance < DISTANCE_ALERT && systemActive) {
    int frequency = map(distance, 0, DISTANCE_ALERT, 2000, 500);
    tone(BUZZER, frequency, 100);
  }
}

void loop() {
  // System activation
  if(digitalRead(BTN_START) == LOW) {
    systemActive = !systemActive;
    lcd.clear();
    delay(500);
    if(!systemActive) {
      digitalWrite(FAN, LOW);
      noTone(BUZZER);
    }
  }

  if(systemActive) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int lum = analogRead(LUM_PIN);
    float dist = getDistance();

    updateClimateControl(temp, hum);
    updateDisplay(temp, hum, lum, dist);
    handleBuzzer(dist);
  }
}