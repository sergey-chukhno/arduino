#include <DHT.h>
#include <LiquidCrystal.h>

// PIN configuration
#define DHTPIN 2          
#define DHTTYPE DHT11
#define RED_PIN 3         
#define GREEN_PIN 5       
#define BLUE_PIN 10       

// Initialize components
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 6, 7, 8, 9); // 4-bit LCD connection

void setup() {
  Serial.begin(9600);
  
  // Configure RGB LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Station Meteo");
  delay(2000);
  lcd.clear();
}

void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void loop() {
  // Read sensor data
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Temperature display
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C  ");
  setColor(255, 0, 0);  // Red
  delay(2000);

  // Humidity display
  lcd.setCursor(0, 0);
  lcd.print("Hum:  ");
  lcd.print(hum);
  lcd.print(" %  ");
  setColor(0, 0, 255);  // Blue
  delay(2000);
}