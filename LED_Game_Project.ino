// define all of these keywords to mean certain pin numbers

#define GREEN4 2
#define GREEN3 3
#define GREEN2 4
#define GREEN1 5

#define LED1 A5
#define LED2 A4
#define LED3 A3
#define LED4 A2

#define BUTTON4 10
#define BUTTON3 11
#define BUTTON2 12
#define BUTTON1 13

#define BUZZER 9

// we need concepts like vectors in this program, so the Arduino-adapted version of the C++ STL is needed.
#include "Arduino_AVRSTL.h"

using namespace std;

// make arrays of similar objects (these are actually arrays of pins)
int buttons[] = {BUTTON1, BUTTON2, BUTTON3, BUTTON4};
int LEDs[] = {LED1, LED2, LED3, LED4};
int GREENs[] = {GREEN1, GREEN2, GREEN3, GREEN4};
int tones[] = {523, 622, 740, 880};

void startupSound() {
    tone(BUZZER, 523);
    delay(500);
    tone(BUZZER, 587);
    delay(500);
    tone(BUZZER, 784);
    delay(500);
    tone(BUZZER, 1000);
    delay(500);
    noTone(BUZZER);
}

void setup() {
  // put your setup code here, to run once:

  // all pins are given appropriate modes.
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(LEDs[i], OUTPUT);
    pinMode(GREENs[i], OUTPUT);
  }
  pinMode(BUZZER, OUTPUT);

  // Initialises serial (text) output with a given bitrate
  Serial.begin(9600);

  // We are using random numbers in this program, so we set a seed using an open analog pin.
  randomSeed(analogRead(A0));

  // Play start sound
  startupSound();
}

// this vector stores the sequence in which the user will press the buttons
vector<int> buttonSequence = {};

// this is the initial answer that game will set
vector<int> answer = {2, 3, 1, 0};


// whenever a user wins (guesses the right sequence), this function is called to make a new sequence
vector<int> genAnswer() {

  // initialise empty vector
  vector<int> answer = {};

  //get a random number from 0 to 3
  int rnd = random(4);

  // run a loop to fill the vector
  while (answer.size() < 4) {
    rnd = random(4);
    if (find(answer.begin(), answer.end(), rnd) == answer.end()) {
      // random number not in vector
      answer.push_back(rnd);
    }
  }

  // return the answer
  return answer;
}

// this function is called when a sequence has been entered
// it counts how many buttons were in the right order

int checkSequence(vector<int> seq, vector<int> ans) {
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (seq[i] == ans[i]) {
      count++;
    }
  }
  return count;
}

/*
 0 correct -> all red flash
 1 correct - > 1 green flash
 2 correct -> 2 green flash
 all correct -> all flash
 */

void turnOn(vector<int> v, bool all = false) {
  vector<int> w;
  if (all) {
    w = {0, 1, 2, 3, 4, 5, 6, 7};
  }
  else {
    w = v;
  }
  
  for (auto x:w) {
    if (x < 4) {
      digitalWrite(LEDs[x], HIGH);
    }
    else {
      analogWrite(GREENs[x-4], 255);
    }
  }
}

void turnAllOff(int del = 150) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDs[i], LOW);
    digitalWrite(GREENs[i], LOW);
  }
}

void playNote(int freq, int del = 150, int brk = 50) {
  tone(BUZZER, freq);
  delay(del);
  turnAllOff();
  noTone(BUZZER);
  delay(brk);
}

// this function is called after checking the inputed sequence
// it makes the required LEDs blink

void flashOutput(int correct) {
  switch (correct) {
    case 0:
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
          analogWrite(LEDs[j], 255);
        }
        tone(BUZZER, 500);
        delay(100);
        for (int k = 0; k < 4; k++) {
          analogWrite(LEDs[k], 0);
        }
        noTone(BUZZER);
        delay(100);
      }
      break;

     case 1:
      for (int i = 0; i < 10; i++) {
        digitalWrite(GREEN1, HIGH);
        tone(BUZZER, 700);
        delay(50);
        tone(BUZZER, 800);
        delay(50);
        digitalWrite(GREEN1, LOW);
        noTone(BUZZER);
        delay(100);
      }
      break;

     case 2:
      for (int i = 0; i < 5; i++) {
        digitalWrite(GREEN1, HIGH);
        digitalWrite(GREEN2, HIGH);
        tone(BUZZER, 600);
        delay(100);
        tone(BUZZER, 700);
        delay(50);
        tone(BUZZER, 800);
        delay(100);
        tone(BUZZER, 900);
        delay(50);
        digitalWrite(GREEN1, LOW);
        digitalWrite(GREEN2, LOW);
        noTone(BUZZER);
        delay(100);
      }
      break;

     case 4:
      // trying crazy frog
      // musical notes from C5

      int c = 523; int d = 587; int e = 659; int f = 698; int g = 784; int a = 880; int b_b = 932; int b = 988;
      int B = 50; int t = 150;

      turnOn({}, true);
      playNote(d, 150, 400);

      // Animation #1
      turnOn({1, 4});
      playNote(f);
      turnOn({2, 5});
      playNote(d);
      turnOn({3, 6});
      playNote(d);
      turnOn({4, 7});
      playNote(g);

      // Animation #2
      turnOn({3, 4});
      playNote(d);
      turnOn({3, 4});
      playNote(c);
      turnOn({2, 3, 4, 5});
      playNote(d, 150, 300);
      turnOn({2, 3, 4, 5});
      playNote(a);
      turnOn({1, 2, 3, 4, 5, 6});
      playNote(d);
      turnOn({1, 2, 3, 4, 5, 6});
      playNote(d);
      turnOn({0, 1, 2, 3, 4, 5, 6, 7});
      playNote(b_b);
      turnOn({0, 1, 2, 3, 4, 5, 6, 7});

      // Animation #3
      turnOn({0, 1, 6, 7});
      playNote(a);
      turnOn({0, 1, 6, 7});
      playNote(f);
      turnOn({2, 3, 4, 5});
      playNote(d);
      turnOn({2, 3, 4, 5});
      playNote(a);

      // Animation #4
      turnOn({0, 2, 4, 6});
      playNote(1175);
      turnOn({0, 2, 4, 6});
      playNote(d);
      turnOn({1, 3, 5, 7});
      playNote(c);
      turnOn({1, 3, 5, 7});
      playNote(c);

      turnOn({}, true);
      playNote(440);

      turnOn({}, true);
      playNote(e, 300, 150);
      turnOn({}, true);
      playNote(d, 800, 0);
      
      break;
    }
}

// this variable indicates if we need to make a new sequence, it is set to true when the user guesses correctly

bool newAns = false;

void loop() {
  // put your main code here, to run repeatedly:

  // if we do need a new answer, call genAnswer() and print the output (which the user won't see)

  if (newAns) {
    answer = genAnswer();
    Serial.println("Solved! New Code:\n\n");
    Serial.println(answer[0]);
    Serial.println(answer[1]);
    Serial.println(answer[2]);
    Serial.println(answer[3]);
    Serial.println("\n");
    newAns = false;
  }

  // run a for loop 4 times to see if any of our buttons have been pressed!
  
  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttons[i]) == HIGH) {
      
      // a button has been pressed, so light up the corresponding red LED
      analogWrite(LEDs[i], 255);

      // Play the sound
      tone(BUZZER, tones[i]);
      delay(50);
      
      // the Arduino senses that the button has been pressed multiple times because we hold it down
      // we only want to consider the first press and no repeats. so, if the buttonSequence vector
      // contains the vector already, we don't add it again.
      
      if (find(buttonSequence.begin(), buttonSequence.end(), i) != buttonSequence.end()) {
        // the vector has that button, don't do anything.
      }
      else {
        
        // a new button has been pressed, add it to the vector
        buttonSequence.push_back(i);

        // if 4 buttons have been pressed, evaluate the input
        if (buttonSequence.size() == 4) {
          delay(200);
          noTone(BUZZER);
          
          // see how many buttons were in the right place
          int correct = checkSequence(buttonSequence, answer);

          if (correct == 4) {
            // the user won! now we need a new answer.
            newAns = true;
          }

          // make the LEDs blink as we need.
          delay(50);
          analogWrite(LEDs[i], 0);
          flashOutput(correct);

          // reset the vector, and we're ready for a new input sequence.
          buttonSequence = {};
        }  
      }
    }
    else {
      // if a button hasn't been pressed, turn the corresponding LED off.
      analogWrite(LEDs[i], 0);
      noTone(BUZZER);
    }
  }
}
