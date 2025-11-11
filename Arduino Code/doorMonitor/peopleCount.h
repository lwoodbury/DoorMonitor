// --- Pin configuration ---
const int PIN_B1 = 32;
const int PIN_B2 = 14;

// --- Timing constants ---
const unsigned long DEBOUNCE_MS = 40;   // ignore short glitches

// --- State tracking ---
int lastB1State = LOW; // LOW = unbroken, HIGH = broken
int lastB2State = LOW;

unsigned long lastB1Change = 0;
unsigned long lastB2Change = 0;

long peopleCount = 0;
long totalCount = 0;        // total count for the day

// --- Crossing states ---
enum CrossState { IDLE, B1_ACTIVE, B2_ACTIVE, ENTRY_PENDING, EXIT_PENDING };
CrossState state = IDLE;

/***************
Function helpers
*******************/
// Non-blocking debounce
bool isBeamBroken(int pin, int &lastState, unsigned long &lastChange) {
  int current = digitalRead(pin);
  unsigned long now = millis();

  if (current != lastState) {
    lastChange = now;
    lastState = current;
  }

  if (current == HIGH && (now - lastChange) >= DEBOUNCE_MS) {
    return true;
  }
  return false;
}

// Non-blocking beam released
bool isBeamClear(int pin, int &lastState, unsigned long &lastChange) {
  int current = digitalRead(pin);
  unsigned long now = millis();

  if (current != lastState) {
    lastChange = now;
    lastState = current;
  }

  if (current == LOW && (now - lastChange) >= DEBOUNCE_MS) {
    return true;
  }
  return false;
}


void updatePeopleCount() {
  bool B1 = isBeamBroken(PIN_B1, lastB1State, lastB1Change);
  bool B2 = isBeamBroken(PIN_B2, lastB2State, lastB2Change);
  bool B1_clear = isBeamClear(PIN_B1, lastB1State, lastB1Change);
  bool B2_clear = isBeamClear(PIN_B2, lastB2State, lastB2Change);

  switch (state) {
    case IDLE:
      if (B1 && !B2) state = B1_ACTIVE;
      if (B2 && !B1) state = B2_ACTIVE;
      break;

    case B1_ACTIVE:
      if (B2) state = ENTRY_PENDING;
      if (B1_clear) state = IDLE;
      break;

    case B2_ACTIVE:
      if (B1) state = EXIT_PENDING;
      if (B2_clear) state = IDLE;
      break;

    case ENTRY_PENDING:
      if (B1_clear && B2_clear) {
        peopleCount++;
        totalCount++;
        Serial.print("Entry detected. Current count: ");
        Serial.println(peopleCount);
        state = IDLE;
      }
      break;

    case EXIT_PENDING:
      if (B1_clear && B2_clear) {
        peopleCount = max(0L, peopleCount - 1);
        Serial.print("Exit detected. Current count: ");
        Serial.println(peopleCount);
        state = IDLE;
      }
      break;
  }
}