// Psychokinetic Energy Meter Arduino Control Code
// Programmer: Dylan Yaga
// 09.09.2024
// https://github.com/dylanyaga/PKEMeter_ArduinoNanoEvery


// Libraries
// https://github.com/DFRobot/DFRobotDFPlayerMini
#include <DFRobotDFPlayerMini.h>
// https://docs.arduino.cc/learn/built-in-libraries/software-serial/
#include <SoftwareSerial.h>
https://github.com/IPdotSetAF/EZButton
#include <EZButton.h>

// Pins - corresponds to Digital Output on Arduino Nano Every board
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BOARD_PIN 4  
#define LED_1_PIN 5
#define LED_2_PIN 6
#define LED_Y_PIN 7
#define LED_R_PIN 8
#define LED_G_PIN 9

// Set volume to max
#define VOLUME 30

// Define Sounds on SD Card
// NOTE: DFPlayer Mini does not care what your sound files are named
// It takes the order they are placed on the SD card - so place them
// on the SD card in the correct order (Mine are numbered)
#define START_SOUND 1
#define IDLE_SOUND 2
#define STATE_ACTIVE_1_SOUND 3
#define STATE_ACTIVE_2_SOUND 4

// EZButton Numbers
#define BTN_1 0
#define BTN_2 1

// PKE Meter States
#define STATE_IDLE 0
#define STATE_ACTIVE_1 1
#define STATE_ACTIVE_2 2

// Setup DFPlayer Mini
DFRobotDFPlayerMini myDFPlayer;
// Set the RX and TX for the DFPlayer Mini
SoftwareSerial mySoftwareSerial(10, 11); 


// Small struct to keep all of the data of the LEDs together; 
// If you are not replacing/rewiring the LEDs (the Red, Green and Yellow, and 2 additional Red)
// Then anything that uses this structure is not needed.
struct randomLED {
  int pin;
  int ledState;
  unsigned long previousMillis;
  long interval;
  bool blink;
};

// Setup some global variables to control state
// and LEDs
int currentState = STATE_IDLE;
randomLED led1;
randomLED led2;
randomLED ledY;
randomLED ledR;
randomLED ledG;

// EZButton Read Buttons function
void ReadButtons(bool* states, int num) {
  states[BTN_1] = !digitalRead(BUTTON1_PIN);
  states[BTN_2] = !digitalRead(BUTTON2_PIN);
}

// Setup EZButtons
// They can be expanded to accomodate
// long press functions as well; but I did not.
EZButton _ezb(2, ReadButtons, 1000, 200);

// Setup Serial connection for DFPlayer Mini
#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(4, 5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif


void setup() {
  // Begin Serial for DFPlayer
#if (defined ESP32)
  FPSerial.begin(9600, SERIAL_8N1, D3, D2);
#else
  FPSerial.begin(9600);
#endif
  // Begin Serial for Arduino
  Serial.begin(115200);

  Serial.println();
  Serial.println("Initializing Psychokinetic Energy Meter...");
  // Log some output for debugging if needed.
  if (!myDFPlayer.begin(FPSerial, true, true)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1. Please recheck the connection!"));
    Serial.println(F("2. Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println("Psychokinetic Energy Meter Warning Module Initialized...");

  // Setup Pins
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BOARD_PIN, OUTPUT);
  pinMode(LED_Y_PIN, OUTPUT);
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  // Write Initial Status
  digitalWrite(BOARD_PIN, HIGH);
  digitalWrite(LED_1_PIN, LOW);
  digitalWrite(LED_2_PIN, LOW);
  digitalWrite(LED_Y_PIN, LOW);
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);
  
  // Setup RandomLED structs
  led1 = { LED_1_PIN, LOW, 0, 1000, true };
  led2 = { LED_2_PIN, LOW, 0, 500, true };
  ledY = { LED_Y_PIN, LOW, 0, 500, true };
  ledR = { LED_R_PIN, LOW, 0, 250, true };
  ledG = { LED_G_PIN, LOW, 0, 1000, true };

  // Set DFPlayer Mini Volume
  myDFPlayer.volume(VOLUME);

  Serial.println("Begin Boot Sequence...");
  // Play PKE Boot Sound
  myDFPlayer.play(START_SOUND);
  // Wait a sec
  delay(1000); 

  // Loop the Idle State sound                 
  myDFPlayer.loop(IDLE_SOUND);  
  Serial.println("Boot Sequence Completed.");

  // Subscribe the EZButtons to their relased functions
  _ezb.Subscribe(BTN_1, Btn1Released, RELEASED);
  _ezb.Subscribe(BTN_2, Btn2Released, RELEASED);
  
  // Set current state
  currentState = STATE_IDLE;
}

void Btn1Released() {
  // Toggle the Idle state and Active State
  if (currentState == STATE_IDLE) {
    currentState = STATE_ACTIVE_1;
    myDFPlayer.loop(STATE_ACTIVE_1_SOUND);
  } else {
    currentState = STATE_IDLE;
    myDFPlayer.loop(IDLE_SOUND);
  }
  // Simulate a momentary button press to toggle the screen LEDs and Wings
  simulateButtonPress();
}

void Btn2Released() {
  // If the state is Idle; do nothing
  if (currentState == STATE_IDLE) {
    return;
  }
  // Toggle between the two active states
  if (currentState == STATE_ACTIVE_1) {
    currentState = STATE_ACTIVE_2;
    myDFPlayer.loop(STATE_ACTIVE_2_SOUND);
  } else if (currentState == STATE_ACTIVE_2) {
    currentState = STATE_ACTIVE_1;
    myDFPlayer.loop(STATE_ACTIVE_1_SOUND);
  }
}

void simulateButtonPress() {
  // Simulate momentary button switch
  Serial.println("Initializing Active Search Mode...");
  digitalWrite(BOARD_PIN, LOW);
  delay(500);
  digitalWrite(BOARD_PIN, HIGH);
  Serial.println("Active Search Mode Initialized.");
}

void BlinkLED(randomLED* led) {
  // Blink without delay
  if (led->blink == false) {
    digitalWrite(led->pin, HIGH);
  }
  unsigned long currentMillis = millis();

  if (currentMillis - led->previousMillis >= led->interval) {
    led->previousMillis = currentMillis;
    if (led->ledState == LOW) {
      led->ledState = HIGH;
    } else {
      led->ledState = LOW;
    }
    digitalWrite(led->pin, led->ledState);
  }
}

void TurnOffLED(randomLED* led) {
  // Just turns a LED off
  digitalWrite(led->pin, LOW);
}


void loop() {
  // EZButtons Looping to handle their presses
  _ezb.Loop();

  // Handle the various states of the PKE Meter
  if (currentState == STATE_IDLE) {
    led1.blink = true;
    led2.blink = true;
    ledG.blink = true;
    ledY.blink = true;
    BlinkLED(&led1);
    BlinkLED(&led2);
    BlinkLED(&ledG);
    TurnOffLED(&ledY);
    TurnOffLED(&ledR);
  } else if (currentState == STATE_ACTIVE_1) {
    led1.blink = false;
    led2.blink = true;
    ledG.blink = false;
    ledY.blink = true;
    BlinkLED(&led1);
    BlinkLED(&led2);
    BlinkLED(&ledG);
    BlinkLED(&ledY);
    TurnOffLED(&ledR);
  } else if (currentState == STATE_ACTIVE_2) {
    led1.blink = false;
    led2.blink = false;
    ledG.blink = false;
    ledY.blink = false;
    BlinkLED(&led1);
    BlinkLED(&led2);
    BlinkLED(&ledG);
    BlinkLED(&ledY);
    BlinkLED(&ledR);
  }
}
