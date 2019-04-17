// constants / pin numbers
const int motor00 = 2;
const int motor01 = 3;
const int motor10 = 4;
const int motor11 = 5;
const int motor20 = 6;
const int motor21 = 7;
const int motor30 = 8;
const int motor31 = 9;
const int motor40 = 11;
const int motor41 = 12;
const int motor50 = 44;
const int motor51 = 45;

const int buttonPin = 22;

const int HRsensor = 46;

// variables
int buttonTimer = 0; //ms timer: will store the value of millis() to check for pressing time
int longClickTime = 1000; //ms period: how long to wait for the long click event to trigger
bool buttonActive = false;
bool longPressActive = false;
bool brush1Active = false;

int SOA = 800; //inter-stimulus onset asynchrony
int motorCycle = 1000; //full time each motor will run
int PWM = 153; //intensity of the motor vibration. Range: 0 --> 153 (pins go up to 255 but we only want to use 3V max in the transistor ==> 3V/5V*255 = 153)


void setup() {
  pinMode(motor00, OUTPUT);
  pinMode(motor01, OUTPUT);
  pinMode(motor10, OUTPUT);
  pinMode(motor11, OUTPUT);
  pinMode(motor20, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor30, OUTPUT);
  pinMode(motor31, OUTPUT);
  pinMode(motor40, OUTPUT);
  pinMode(motor41, OUTPUT);
  pinMode(motor50, OUTPUT);
  pinMode(motor51, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(HRsensor, INPUT);
}

void loop() {

  if (digitalRead(buttonPin) == HIGH) {
    if (buttonActive == false){
      buttonActive = true;
      buttonTimer = millis();  
    }

    if ((millis() - buttonTimer > longClickTime) && (longPressActive == false)){
      longPressActive = true;
      //do long click event
      longClickEvent();
    }
  } else {
    if (buttonActive == true){
      if (longPressActive == true){
        longPressActive = false;
      } else {
        //do short click event
        shortClickEvent();
      }
      buttonActive = false;
    }
  }

}

void longClickEvent() {
  
}

void shortClickEvent() {
  //notification for the short click input
  analogWrite(motor00, 153);  // run first line of motor at 60% duty cycle --> 3V
  analogWrite(motor01, 153);
  delay(500);                // play for 0.5s
  analogWrite(motor00, 0);  // shut first line of motor
  analogWrite(motor01, 0);
  delay(1000);             // wait for 1s

  brush1();
  brush1Active = true;
  while (true) {
    if (digitalRead(buttonPin) == HIGH){
      if (brush1Active == true){
        brush1Active = false;
        brush2();
      } else {
        brush1Active = true;
        brush1();
      }
    }
    while (digitalRead(HRsensor)>120){
      if (brush1Active){
        // change the parameters of the brush
        // do the brush patter 1
      } else {
        
      }
    }
  }
}

void brush1() {
  analogWrite(motor00, PWM);  // run first line of motor at PWM (range: 0 --> 153)
  analogWrite(motor01, PWM);
  delay(SOA);                // play for *SOA* before starting the next row
  analogWrite(motor10, PWM);  // run second line of motor at PWM (range: 0 --> 153)
  analogWrite(motor11, PWM);
  delay(motorCycle-SOA);    // wait for the full motorCycle to finish before shutting down the first line
  analogWrite(motor00, 0);  // shut first line of motor
  analogWrite(motor01, 0);
  delay(2*SOA-motorCycle);  // wait for the time in between 
  analogWrite(motor20, PWM);  // run third line of motor at PWM (range: 0 --> 153)
  analogWrite(motor21, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor10, 0);  // shut second line of motor
  analogWrite(motor11, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor20, PWM);  // run fourth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor21, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor10, 0);  // shut third line of motor
  analogWrite(motor11, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor20, PWM);  // run fifth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor21, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor10, 0);  // shut fourth line of motor
  analogWrite(motor11, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor20, PWM);  // run sixth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor21, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor10, 0);  // shut fifth line of motor
  analogWrite(motor11, 0);
  delay(2*SOA-motorCycle);
  delay(motorCycle-SOA);
  analogWrite(motor10, 0);  // shut sixth line of motor
  analogWrite(motor11, 0);
  
  
}

void brush2() {
  analogWrite(motor50, PWM);  // run the motor50 at PWM (range: 0 --> 153)
  delay(SOA);                 // play for *SOA* before starting the next row
  analogWrite(motor51, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor50, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor41, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor51, 0);
  delay(motorCycle-SOA);
  analogWrite(motor50, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor41, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor50, 0);

  
}

