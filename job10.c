const int POTENTIOMETER = A0;
const int LED1 = 9;
const int LED2 = 10; 

void setup() {
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT);
}
  

void loop() {
  //Read value of Potentionmetre (from 0 to 1023)
  int valuePot = analogRead(POTENTIOMETER);
  
  //Convert potentiometer value to LED brightness 
  int brightnessLED1 = map(valuePot, 0, 1023, 0, 255);
  int brightnessLED2 = 255 - brightnessLED1; 
  
  //Apply brightness values to LEDS
  analogWrite(LED1, brightnessLED1);
  analogWrite(LED2, brightnessLED2); 
  
  delay(10);
}
  