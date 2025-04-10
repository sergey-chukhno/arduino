#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 6, 7, 8, 9); // Configuration des broches

// Caractère personnalisé (rectangle plein)
byte fullBlock[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

unsigned long previousMillis = 0;
const long interval = 3000; // Intervalle de 3 secondes
int progress = 0;           // Progression de la barre

void setup() {
  lcd.begin(16, 2);
  lcd.createChar(0, fullBlock); // Enregistre le caractère personnalisé
  
  lcd.print("Loading...");   // Message statique
}

void loop() {
  unsigned long currentMillis = millis();

  // Gestion du timing
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    lcd.setCursor(progress, 1); // Positionne sur la 2ème ligne
    
    if(progress < 16) { // Remplissage progressif
      lcd.write(byte(0)); // Affiche le bloc
      progress++;
    }
    else { // Réinitialisation
      lcd.clear();
      lcd.print("Loading...");
      progress = 0;
    }
  }
}