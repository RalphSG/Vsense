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

int SOA = 2000; //inter-stimulus onset asynchrony
int motorCycle = 3000; //full time each motor will run
int PWM = 150; //intensity of the motor vibration. Range: 0 --> 255 (motors need at least 60 to start)


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
  delay(5000);                // play for 0.5s
  analogWrite(motor00, 0);  // shut first line of motor
  analogWrite(motor01, 0);
  delay(1000);             // wait for 1s

  brush1();
  brush1Active = true;
  while (true) {
    Serial.println("Waiting for sensor...");
    if (digitalRead(buttonPin) == LOW){
      if (brush1Active == true){
        brush1Active = false;
        brush2();
      } else {
        brush1Active = true;
        brush1();
      }
      delay(100);
    }

    if (digitalRead(buttonSensorSim) == LOW){
      Serial.println("Entered the sensor simulation...");
      if (brush1Active == true){
        brush1();
      } else {
        brush2();
      }
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
  analogWrite(motor50, PWM);  // run first line of motor at 60% duty cycle --> 3V  
  analogWrite(motor51, PWM);
  delay(1000);                // play for 0.5s
  analogWrite(motor50, 0);  // shut first line of motor
  analogWrite(motor51, 0);
  delay(1000);             // wait for 1s

  brush1();
}

void brush1() {
  Serial.println("Starting Brush1");
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
  analogWrite(motor30, PWM);  // run fourth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor31, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor20, 0);  // shut third line of motor
  analogWrite(motor21, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor40, PWM);  // run fifth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor41, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor30, 0);  // shut fourth line of motor
  analogWrite(motor31, 0);
  delay(2*SOA-motorCycle);
  analogWrite(motor50, PWM);  // run sixth line of motor at PWM (range: 0 --> 153)
  analogWrite(motor51, PWM);
  delay(motorCycle-SOA);
  analogWrite(motor40, 0);  // shut fifth line of motor
  analogWrite(motor41, 0);
  delay(2*SOA-motorCycle);
  delay(motorCycle-SOA);
  analogWrite(motor50, 0);  // shut sixth line of motor
  analogWrite(motor51, 0);
  Serial.println("Finished Brush1");
  
}

void brush2() {
  Serial.println("Starting Brush2");
  analogWrite(motor51, PWM);  // run the motor50 at PWM (range: 0 --> 153)
  Serial.println("motor12 start");
  delay(SOA);                 // play for *SOA* before starting the next row
  analogWrite(motor50, PWM);
  Serial.println("motor11 start");
  delay(motorCycle-SOA);
  analogWrite(motor51, 0);
  Serial.println("motor12 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor40, PWM);
  Serial.println("motor9 start");
  delay(motorCycle-SOA);
  analogWrite(motor50, 0);
  Serial.println("motor11 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor41, PWM);
  Serial.println("motor10 start");
  delay(motorCycle-SOA);
  analogWrite(motor40, 0);
  Serial.println("motor9 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor31, PWM);
  Serial.println("motor8 start");
  delay(motorCycle-SOA);
  analogWrite(motor41, 0);
  Serial.println("motor10 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor30, PWM);
  Serial.println("motor7 start");
  delay(motorCycle-SOA);
  analogWrite(motor31, 0);
  Serial.println("motor8 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor20, PWM);
  Serial.println("motor5 start");
  delay(motorCycle-SOA);
  analogWrite(motor30, 0);
  Serial.println("motor7 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor21, PWM);
  Serial.println("motor6 start");
  delay(motorCycle-SOA);
  analogWrite(motor20, 0);
  Serial.println("motor5 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor11, PWM);
  Serial.println("motor4 start");
  delay(motorCycle-SOA);
  analogWrite(motor21, 0);
  Serial.println("motor6 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor10, PWM);
  Serial.println("motor3 start");
  delay(motorCycle-SOA);
  analogWrite(motor11, 0);
  Serial.println("motor4 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor00, PWM);
  Serial.println("motor1 start");
  delay(motorCycle-SOA);
  analogWrite(motor11, 0);
  Serial.println("motor4 stop");
  delay(2*SOA-motorCycle);
  analogWrite(motor01, PWM);
  Serial.println("motor2 start");
  delay(motorCycle-SOA);
  analogWrite(motor00, 0);
  Serial.println("motor1 stop");
  delay(SOA);
  analogWrite(motor01, 0);
  Serial.println("motor2 stop");
  Serial.println("Finished Brush2");
}

