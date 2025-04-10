const int NB_LEDS = 8;          // Nombre de LED
const int LEDS[] = {2,3,4,5,6,7,8,9}; // Broches des LED
const int DELAI = 100;          // Délai entre les mouvements (ms)

void setup() {
  // Configuration des broches en sortie
  for (int i = 0; i < NB_LEDS; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  // Parcours des LED de gauche à droite
  for (int i = 0; i < NB_LEDS; i++) {
    digitalWrite(LEDS[i], HIGH); // Allume la LED
    delay(DELAI);
    digitalWrite(LEDS[i], LOW);  // Éteint la LED
  }
  
  for (int i = NB_LEDS - 1; i >= 0; i--) {
    digitalWrite(LEDS[i], HIGH);
    delay(DELAI);
    digitalWrite(LEDS[i], LOW);
  }
}