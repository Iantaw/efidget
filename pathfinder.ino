#include <LinkedList.h>

int BUT1 = D0;
int BUT2 = D1;
int BUT3 = D2;
int BUT4 = D3;

int LED1 = D4;
int LED2 = D5;
int LED3 = D6;
int LED4 = D7;

int buttonPins[4] = {BUT1, BUT2, BUT3, BUT4};
int ledPins[4] = {LED1, LED2, LED3, LED4};

int ledToButtonMap[4] = {1, 0, 3, 2};

LinkedList<int> numList;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  int newNum = random(0, 4);
  numList.add(newNum);

  Serial.print("Round ");
  Serial.println(numList.size());

  playSequence();

  if (!checkUserInput()) {
    gameOver();
    return;
  }

  delay(800);
}

void playSequence() {
  for (int i = 0; i < numList.size(); i++) {
    int led = numList.get(i);
    digitalWrite(ledPins[led], HIGH);
    delay(500);
    digitalWrite(ledPins[led], LOW);
    delay(300);
  }
}

bool checkUserInput() {
  Serial.println("Repeat sequence...");
  
  for (int i = 0; i < numList.size(); i++) {
    int ledIndex = numList.get(i);
    int correctButton = ledToButtonMap[ledIndex];
    int pressed = waitForButtonPress();

    if (pressed != correctButton) {
      Serial.print("Expected BUTTON ");
      Serial.println(correctButton);
      Serial.print("But got BUTTON ");
      Serial.println(pressed);
      return false;
    }
  }
  Serial.println("Sequence correct!");
  return true;
}

int waitForButtonPress() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        while (digitalRead(buttonPins[i]) == LOW);
        digitalWrite(ledPins[i], HIGH);
        delay(150);
        digitalWrite(ledPins[i], LOW);
        return i;
      }
    }
  }
}

void gameOver() {
  for (int n = 0; n < 3; n++) {
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], HIGH);
    delay(200);
    for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
    delay(200);
  }

  Serial.println("You died! Resetting...");
  numList.clear();
  delay(1000);
}