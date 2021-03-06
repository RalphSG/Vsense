// constants / pin numbers
const int motor00 = 2;
const int motor01 = 45;
const int motor10 = 3;
const int motor11 = 44;
const int motor20 = 4;
const int motor21 = 9;
const int motor30 = 5;
const int motor31 = 11;
const int motor40 = 6;
const int motor41 = 12;
const int motor50 = 8;
const int motor51 = 13;

const int buttonPin = 48;

//const int HRsensor = 46;
const int buttonSensorSim = 52;

// variables
unsigned long timerDiff = 0;
unsigned long buttonTimer = 0; //ms timer: will store the value of millis() to check for pressing time
unsigned long longClickTime = 1000; //ms period: how long to wait for the long click event to trigger
bool buttonActive;
bool longPressActive = false;
bool brush1Active = false;
bool brush2Active = false;
bool brush3Active = false;
bool longClickMode = false;

int SOA = 95; //inter-stimulus onset asynchrony
int motorCycle = 160; //full time each motor will run
int addTime = 100; //additional time at the end of the wave
int PWM = 255; //intensity of the motor vibration. Range: 0 --> 255 (motors need at least 60 to start)


void setup() {
  Serial.begin(9600);
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
  pinMode(buttonPin, INPUT_PULLUP);
  //pinMode(HRsensor, INPUT);
  pinMode(buttonSensorSim, INPUT_PULLUP);
  
  buttonActive = false;
}

void loop() {
  Serial.println(digitalRead(buttonPin));
  
  if (digitalRead(buttonPin) == LOW) {
    Serial.print("Starting program... ");
    while(digitalRead(buttonPin) == LOW){
      Serial.println("Entering the while loop for checking longClick...");
      if(buttonActive == false){
        Serial.println("Setting buttonTimer and buttonActive...");
        buttonTimer = millis();
        buttonActive = true;
      }
      Serial.println("I escaped the if statement for setting bTimer and bActive...");
      timerDiff = millis() - buttonTimer;
      if (timerDiff > longClickTime){
      //do long click event
      Serial.print("millis() = ");
      Serial.println(millis());
      Serial.print("buttonTimer = ");
      Serial.println(buttonTimer);
      Serial.println("longClickEvent");
      longPressActive = true;
      buttonActive = false;
      longClickEvent();
      }
      Serial.println("I passed by the if statement for longClick...");
      delay(100);
    }
    if (longPressActive == true){
      Serial.println("Resetting buttonActive and longPressActive...");
      buttonActive = false;
      longPressActive = false;
    } else {
        Serial.println("Resetting buttonActive...");
        buttonActive = false;
        //do short click event
        Serial.println("shortClickEvent");
        shortClickEvent();
        Serial.print("ShortClickEvent is finished. ButtonActive: ");
        Serial.println(buttonActive);
    }
    delay(100);
  }
  delay(100);
}

void longClickEvent() {
 //notification for the long click input
  analogWrite(motor00, PWM);  // run first line of motor at 60% duty cycle --> 3V
  analogWrite(motor01, PWM);
  delay(1000);                // play for 0.5s
  analogWrite(motor00, 0);  // shut first line of motor
  analogWrite(motor01, 0);
  delay(1000);             // wait for 1s

  brush1Active = true;
  longClickMode = true;
  brush1();
  stopMotors();
  while (true) {
    Serial.println("Waiting for sensor...");
    if (digitalRead(buttonPin) == LOW){
      if (brush1Active == true){
        brush1Active = false;
        brush2Active = true;
        brush2();
        stopMotors();
      } else if (brush2Active == true){
        brush2Active = false;
        brush3Active = true;
        brush3();
        stopMotors();
      } else if (brush3Active == true){
        brush3Active = false;
        brush4();
        stopMotors();
      } else {
        brush1Active = true;
        brush1();
        stopMotors();
      }
      delay(100);
    }

    if (digitalRead(buttonSensorSim) == LOW){
      Serial.println("Entered the sensor simulation...");
      longClickMode = false;
      if (brush1Active == true){
        brush1();
      } else if (brush2Active == true){
        brush2();
      } else if (brush3Active == true){
        brush3();
      } else {
        brush4();
      }
      stopMotors();
      delay(500);
      longClickMode = true;
    }
//    while (digitalRead(HRsensor)>120){
//      if (brush1Active){
//        // change the parameters of the brush
//        // do the brush patter 1
//      } else {
//        
//      }
//    }
    delay(100);
  }
}

void shortClickEvent() {
  //notification for the short click input
  analogWrite(motor50, 90);  // run first line of motor at 60% duty cycle --> 3V  
  analogWrite(motor51, 90);
  delay(3000);                // play for 0.5s
  analogWrite(motor50, 0);  // shut first line of motor
  analogWrite(motor51, 0);
  delay(1000);             // wait for 1s

  //brush1();
  stopMotors();
  
  delay(1000);
}

void brush1() {
  do{
    Serial.println("Starting Brush1");
    analogWrite(motor00, PWM);  // run first line of motor at PWM (range: 0 --> 255)
    analogWrite(motor01, PWM);
    delay(SOA);                // play for *SOA* before starting the next row
    analogWrite(motor10, PWM);  // run second line of motor at PWM (range: 0 --> 255)
    analogWrite(motor11, PWM);
    delay(motorCycle-SOA);    // wait for the full motorCycle to finish before shutting down the first line
    analogWrite(motor00, 0);  // shut first line of motor
    analogWrite(motor01, 0);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(2*SOA-motorCycle);  // wait for the time in between 
    analogWrite(motor20, PWM);  // run third line of motor at PWM (range: 0 --> 255)
    analogWrite(motor21, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor10, 0);  // shut second line of motor
    analogWrite(motor11, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, PWM);  // run fourth line of motor at PWM (range: 0 --> 255)
    analogWrite(motor31, PWM);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);  // shut third line of motor
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor40, PWM);  // run fifth line of motor at PWM (range: 0 --> 255)
    analogWrite(motor41, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);  // shut fourth line of motor
    analogWrite(motor31, 0);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(2*SOA-motorCycle);
    analogWrite(motor50, PWM);  // run sixth line of motor at PWM (range: 0 --> 255)
    analogWrite(motor51, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor40, 0);  // shut fifth line of motor
    analogWrite(motor41, 0);
    delay(2*SOA-motorCycle+addTime); // end of the wave, start of the back firing wave
    analogWrite(motor40, PWM);
    analogWrite(motor41, PWM);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(motorCycle-SOA);
    analogWrite(motor50, 0);
    analogWrite(motor51, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, PWM);
    analogWrite(motor31, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor40, 0);
    analogWrite(motor41, 0);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(2*SOA-motorCycle);
    analogWrite(motor20, PWM);
    analogWrite(motor21, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);
    analogWrite(motor31, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor10, PWM);
    analogWrite(motor11, PWM);
    if (digitalRead(buttonPin)==LOW)
      break;
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor00, 1);
    analogWrite(motor01, 1);
    Serial.println("Finished Brush1");
  }while(longClickMode==false);
}

void brush2() {
  Serial.println("Starting Brush2");
  analogWrite(motor51, PWM);  // run the motor50 at PWM (range: 0 --> 153)
  delay(SOA);                 // play for *SOA* before starting the next row
  do{
    analogWrite(motor50, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor51, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor40, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor50, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor41, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor40, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor31, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor41, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor31, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor20, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor21, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor11, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor10, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor11, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor00, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor10, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor01, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor00, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor11, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor01, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor10, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor11, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor20, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor10, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor21, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor31, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor31, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor40, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor41, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor40, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor51, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
        break;
    analogWrite(motor41, 0);
    delay(2*SOA-motorCycle);
  }while(longClickMode==false);
  Serial.println("Finished Brush2");
}

void brush3(){
  Serial.println("Starting Brush3");
 analogWrite(motor00, PWM);
 delay(SOA);
 do {
   analogWrite(motor10, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor00, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor20, PWM);
   delay(motorCycle-SOA);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor10, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor30, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor20, 0);
   delay(2*SOA-motorCycle);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor40, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor30, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor50, PWM);
   delay(motorCycle-SOA);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor40, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor51, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor50, 0);
   delay(2*SOA-motorCycle);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor41, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor51, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor31, PWM);
   delay(motorCycle-SOA);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor41, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor21, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor31, 0);
   delay(2*SOA-motorCycle);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor11, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor21, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor01, PWM);
   delay(motorCycle-SOA);
   if (digitalRead(buttonPin)==LOW)
     break;
   analogWrite(motor11, 0);
   delay(2*SOA-motorCycle);
   analogWrite(motor00, PWM);
   delay(motorCycle-SOA);
   analogWrite(motor01, 0);
   delay(2*SOA-motorCycle);
   if (digitalRead(buttonPin)==LOW)
     break;
 }while(longClickMode==false);
 Serial.println("Finished Brush3");
}

void brush4(){
  Serial.println("Starting Brush4");
  analogWrite(motor00, PWM);
  delay(SOA);
  do {
    analogWrite(motor10, 165); // 204 was the corresponding number but since the other is not strong enough this had to be tempered
    analogWrite(motor11, 90); // 51 was the correct number to get a perfect hourlass shape with phantom actuators but the motor only start around 85
    delay(motorCycle-SOA);
    analogWrite(motor00, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor20, 137); // putting this one more in the middle to get a contrast with the previous one (correct number: 153)
    analogWrite(motor21, 117); // correct number: 102
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor10, 0);
    analogWrite(motor11, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, 117);
    analogWrite(motor31, 137);
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor40, 90);
    analogWrite(motor41, 165);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);
    analogWrite(motor31, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor51, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor40, 0);
    analogWrite(motor41, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor50, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor51, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor40, 165);
    analogWrite(motor41, 90);
    delay(motorCycle-SOA);
    analogWrite(motor50, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor30, 137);
    analogWrite(motor31, 117);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor40, 0);
    analogWrite(motor41, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor20, 117);
    analogWrite(motor21, 137);
    delay(motorCycle-SOA);
    analogWrite(motor30, 0);
    analogWrite(motor31, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor10, 90);
    analogWrite(motor11, 165);
    delay(motorCycle-SOA);
    analogWrite(motor20, 0);
    analogWrite(motor21, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor01, PWM);
    delay(motorCycle-SOA);
    if (digitalRead(buttonPin)==LOW)
     break;
    analogWrite(motor10, 0);
    analogWrite(motor11, 0);
    delay(2*SOA-motorCycle);
    analogWrite(motor00, PWM);
    delay(motorCycle-SOA);
    analogWrite(motor01, 0);
    delay(2*SOA-motorCycle);
    if (digitalRead(buttonPin)==LOW)
     break;
  }while(longClickMode == false);
  Serial.println("Finished Brush4");
}

void stopMotors() {
  Serial.println("Stopping all motors... ");
  analogWrite(motor00, 0);
  analogWrite(motor01, 0);
  analogWrite(motor10, 0);
  analogWrite(motor11, 0);
  analogWrite(motor20, 0);
  analogWrite(motor21, 0);
  analogWrite(motor30, 0);
  analogWrite(motor31, 0);
  analogWrite(motor40, 0);
  analogWrite(motor41, 0);
  analogWrite(motor50, 0);
  analogWrite(motor51, 0);
}

