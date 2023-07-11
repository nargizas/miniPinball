#include <LiquidCrystal_I2C.h>
#include <Servo.h> 
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo startServo;
Servo leftServo;
Servo rightServo;
#define START_SERVO 9
#define LEFT_SERVO 10
#define RIGHT_SERVO 11
#define INITIAL_POSITION 90

#define LEFT_JOY A0
#define LEFT_BUTTON 3
#define RIGHT_JOY A1
#define RIGHT_BUTTON 4

#define STOP_SENSOR 27
#define NUM_OF_HALL 5
int hallSensorPin[NUM_OF_HALL] = {22, 23, 24, 25, 26};        
int state[NUM_OF_HALL];          
int lastDetectTime = 0;
int detectDelay = 100;

int lastLeftButtonState = 0;
int lastRightButtonState = 0;
int lastJoyL = 0;
int lastJoyR = 0;

int ledState[NUM_OF_HALL] = {-1, -1, -1, -1, -1}; //this variable tracks the state of the sensor, negative if off, positive if on - tbh idfk

int score = 0;
bool gameOn = false;

void setup() {
  pinMode(STOP_SENSOR, INPUT);
  for(int i = 0; i < NUM_OF_HALL; i++){      
    pinMode(hallSensorPin[i], INPUT);
  }
  leftServo.attach(LEFT_SERVO);
  leftServo.write(INITIAL_POSITION);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);

  rightServo.attach(LEFT_SERVO);
  rightServo.write(INITIAL_POSITION);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  
  startServo.attach(START_SERVO);
  startServo.write(0);
  Serial.begin(9600);  
  lcd.init();                      // initialize the lcd 
  
}

void loop(){
  lcd.setCursor(2,1);
  lcd.print("Press joysticks");
  lcd.setCursor(9,2);
  lcd.print("to");
  lcd.setCursor(8,3);
  lcd.print("START!");
  
  if(digitalRead(LEFT_BUTTON)== LOW && digitalRead(RIGHT_BUTTON)== LOW){
    lcd.clear();
    lcd.backlight();
    startServo.write(120);
    gameOn = true;
  }
  
  while(gameOn == true){
    digitalWrite(LEFT_BUTTON, HIGH);
    digitalWrite(RIGHT_BUTTON, HIGH);
    //countScore();
    moveFlippers();  
    Serial.println(score);
    /*if(digitalRead(STOP_SENSOR) == LOW){
      gameOn = false;
      leftServo.detach();
      rightServo.detach();
      startServo.write(0);
      finalScreen();
      score = 0;
    }*/
  } 
}
int countScore(){
    for(int i = 0; i < NUM_OF_HALL; i++){
    state[i] = digitalRead(hallSensorPin[i]);
    if ( (millis() - lastDetectTime) > detectDelay) {

    //if the button has been pressed, lets toggle the LED from "off to on" or "on to off"
      if ( (state[i] == LOW) && (ledState[i] < 0) ) {
        ledState[i] = -ledState[i]; //now the LED is on, we need to change the state
        lastDetectTime = millis(); //set the current time
        score += 50;
      } else if ( (state[i] == HIGH) && (ledState[i] > 0) ) {
        ledState[i] = -ledState[i]; //now the LED is off, we need to change the state
        lastDetectTime = millis(); //set the current time
      }//close if/else
      displayScore();
    }
  }
}

void displayScore(){
      lcd.setCursor(0,0);
      lcd.print("Your score");
      lcd.setCursor(15,0);
      lcd.print(score);
}
void moveFlippers(){
  // read the state of the switch into a local variable:
  int leftButtonState = digitalRead(LEFT_BUTTON);
  int rightButtonState = digitalRead(RIGHT_BUTTON);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (leftButtonState != lastLeftButtonState) {
    // reset the debouncing timer
    lastJoyL = millis();
  }
  if (rightButtonState != lastRightButtonState) {
    // reset the debouncing timer
    lastJoyR = millis();
  }

  if ((millis() - lastJoyL) > 200) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (leftButtonState == LOW ) {
      leftServo.write(180);
    }
    lastJoyL = millis();
  }
  lastLeftButtonState = leftButtonState;

  if ((millis() - lastJoyR) > 200) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (rightButtonState == LOW ) {
      rightServo.write(0);
    }
    lastJoyR = millis();
  }
  lastRightButtonState = rightButtonState;
}

//display total score
void finalScreen(){
      lcd.clear();
      lcd.setCursor(8,1);
      lcd.println("GAME");
      lcd.setCursor(8,2);
      lcd.println("OVER");
      delay(2000);
      
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("Total score");
      int numOfChar = 0;
      int temp = score;
      while(temp > 10){
        temp /= 10;
        numOfChar++;
      }
      int startIndex = (21-numOfChar)/2;
      lcd.setCursor( startIndex,2);
      lcd.print(score);
      
      delay(2000);
      lcd.clear();
      lcd.noBacklight();
}
