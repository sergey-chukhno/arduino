const int POT = A0; 
const int R = 9; 
const int G = 10; 
const int B = 11; 

void setup() {
  pinMode(R, OUTPUT); 
  pinMode(G, OUTPUT); 
  pinMode(B, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(POT); 
  int mappedValue = map(potValue, 0, 1023, 0, 765); 
  
  // Distinguish three phases (for Red, Green and Blue)
  if (mappedValue <= 255) {
    analogWrite(R, 255 - mappedValue); 
    analogWrite(G, mappedValue); 
    analogWrite(B, 0); 
  } else if (mappedValue <= 510) {
    analogWrite(R, 0); 
    analogWrite(G, 510 - mappedValue); 
    analogWrite(B, mappedValue - 255); 
  } else {
    analogWrite(R, mappedValue - 510); 
    analogWrite(G, 0); 
    analogWrite(B, 765 - mappedValue); 
  }
}