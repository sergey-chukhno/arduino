#include <LiquidCrystal.h>

// Hardware Configuration
const int LED_PINS[] = {2, 3, 4, 5, 6};      // LED pins (5 colors)
const int BUTTON_PINS[] = {A0, A1, A2, A3, A4}; // Button pins
const int BUZZER_PIN = 13;
const int DIFFICULTY_POT = A5;               // Difficulty potentiometer

// LCD Connections (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 7, 8, 9, 10);

// Game State Variables
int sequence[100];            // Stores the light sequence
int currentLevel = 1;         // Current level number
int playerScore = 0;          // Total score
int difficultySetting = 1;    // 1=Easy, 2=Medium, 3=Hard
int sequenceLength = 1;       // Current sequence length
unsigned long lastInputTime = 0;

// Difficulty Parameters
const struct {
  int startLength;
  int maxLength;
  char* name;
} DIFFICULTY_LEVELS[3] = {
  {1, 3, "Easy"},   // Easy: 1-3 sequence
  {4, 6, "Medium"},  // Medium: 4-6 sequence
  {7, 10, "Hard"}    // Hard: 7-10 sequence
};

void setup() {
  // Initialize hardware
  initializeHardware();
  
  // Seed random number generator
  randomSeed(analogRead(0));
  
  // Show startup screen
  showMainMenu();
}

void loop() {
  static bool gameActive = false;

  if (!gameActive) {
    handleDifficultySelection();
    initializeNewGame();
    gameActive = true;
  }

  updateGameDisplay();
  playCurrentSequence();
  
  if (checkPlayerInput()) {
    handleSuccessfulRound();
  } else {
    handleGameOver();
    gameActive = false;
  }
}

// Hardware initialization
void initializeHardware() {
  // Set up LEDs and buttons
  for (int i = 0; i < 5; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  
  // Set up buzzer and LCD
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.begin(16, 2);
}

// Display main menu
void showMainMenu() {
  lcd.clear();
  lcd.print(" Memory Game");
  lcd.setCursor(0, 1);
  delay(2000);
}

// Handle difficulty selection
void handleDifficultySelection() {
  lcd.clear();
  lcd.print("Select Difficulty:");
  delay(500);

  // Read stabilized potentiometer value
  int potValue = readStabilizedPotentiometer();
  
  // Map to difficulty levels
  difficultySetting = map(potValue, 0, 1023, 1, 4);
  difficultySetting = constrain(difficultySetting, 1, 3);

  // Show selected difficulty
  lcd.clear();
  lcd.print(DIFFICULTY_LEVELS[difficultySetting-1].name);
  lcd.setCursor(0, 1);
  lcd.print("Length: ");
  lcd.print(DIFFICULTY_LEVELS[difficultySetting-1].startLength);
  delay(1500);
}

// Initialize new game parameters
void initializeNewGame() {
  currentLevel = 1;
  playerScore = 0;
  sequenceLength = DIFFICULTY_LEVELS[difficultySetting-1].startLength;
  generateNewSequence();
}

// Generate new light sequence
void generateNewSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    sequence[i] = random(0, 5); // Random LEDs 0-4
  }
}

// Display game information
void updateGameDisplay() {
  lcd.clear();
  lcd.print("Lv:");
  lcd.print(currentLevel);
  lcd.print(" Sc:");
  lcd.print(playerScore);
  lcd.setCursor(0, 1);
  lcd.print(sequenceLength);
}

// Play the current sequence
void playCurrentSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    activateLED(sequence[i]);
    delay(calculateSpeed());
    deactivateLEDs();
    delay(200);
  }
}

// Check player input against sequence
bool checkPlayerInput() {
  for (int i = 0; i < sequenceLength; i++) {
    int expectedButton = sequence[i];
    int pressedButton = waitForButtonPress();
    
    if (pressedButton != expectedButton) {
      return false;
    }
  }
  return true;
}

// Handle successful round
void handleSuccessfulRound() {
  playerScore++;
  currentLevel++;
  
  // Increase sequence length if under max
  if (sequenceLength < DIFFICULTY_LEVELS[difficultySetting-1].maxLength) {
    sequenceLength++;
  }
  
  generateNewSequence();
  playSuccessFeedback();
}

// Handle game over
void handleGameOver() {
  playFailureFeedback();
  lcd.clear();
  lcd.print("Game Over! Score:");
  lcd.setCursor(0, 1);
  lcd.print(playerScore);
  delay(3000);
}

// Helper Functions -------------------------------------------------

int readStabilizedPotentiometer() {
  // Average 10 readings for stability
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(DIFFICULTY_POT);
    delay(10);
  }
  return total / 10;
}

int calculateSpeed() {
  // Speed decreases with higher difficulty
  return 500 - (difficultySetting * 150);
}

void activateLED(int ledIndex) {
  digitalWrite(LED_PINS[ledIndex], HIGH);
  tone(BUZZER_PIN, 500 + (ledIndex * 200), 100);
}

void deactivateLEDs() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

int waitForButtonPress() {
  while (true) {
    for (int i = 0; i < 5; i++) {
      if (digitalRead(BUTTON_PINS[i]) == LOW) {  // Check for pressed button
        // Valid button press detected
        activateLED(i);
        delay(300);
        deactivateLEDs();
        return i;
      }
    }
    delay(50);
  }
}

void playSuccessFeedback() {
  tone(BUZZER_PIN, 1000, 200);
  lcd.clear();
  lcd.print("Correct!");
  delay(1000);
}

void playFailureFeedback() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 300, 200);
    delay(300);
  }
}