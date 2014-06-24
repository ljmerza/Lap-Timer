/*****************************************************************************************
Created by: Leonardo Merza
Version: 1.05
*****************************************************************************************/

/*****************************************************************************************
Variables
*****************************************************************************************/
// multiplexer pins
int muxS0Pin = 2;
int muxS1Pin = 3;
int muxS2Pin = 4;
int muxS3Pin = 5;
// digit pins
int digitZeroPin = 8;
int digitOnePin = 9;
int digitTwoPin = 10;
int digitThreePin = 11;
// led pins
int LED0 = 12;
int LED1 = 13;

// sonar sensor pins
int trigPin = 6;
int echoPin = 7;

unsigned long currentTime = 0; // current running time
unsigned long lastTime = 0; // last time the sensor was triggered
unsigned long delayTime = 5000; // time to delay stop timer for sensor

// digits to display
unsigned long  displayTimeDigit0 = 0;
unsigned long  displayTimeDigit1 = 0;
unsigned long  displayTimeDigit2 = 0;
unsigned long  displayTimeDigit3 = 0;

long distance = 0; // min distance to trigger sensor
long thresholdDistance = 10; // max distance to trigger sensor
boolean didCarPassSensor = false; // initial boolean for start of sensor
boolean didCarHitSensor = false; // boolean if sensor is triggered again after start
int timeToDelay = 5000; // time to delay sensor reading again
int lapCounter = 0; // counter of lap

// to setup multiplexer to show each number - next line shows 4bit number of each segment in led
//0000 bottom left, 1000 bottom, 0100 dot, 1100 bottom right, 0010 middle, 1010 top left, 0110 top right, 1110 top
int digitZero[] = {0,0,0,0, 1,0,0,0, 1,1,0,0, 1,0,1,0,  0,1,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1};
int digitOne[] = {0,1,1,0, 1,1,0,0, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int digitTwo[] = {0,0,0,0, 1,1,1,1, 1,0,0,0, 0,0,1,0, 0,1,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1};
int digitThree[] = {1,0,0,0, 1,1,0,0, 0,0,1,0, 1,1,1,0, 0,1,1,0, 0,0,0,1, 1,1,1,1, 1,1,1,1};
int digitFour[] = {1,1,0,0, 0,0,1,0, 1,0,1,0, 0,1,1,0, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int digitFive[] = {1,0,0,0, 1,1,0,0, 0,0,1,0, 1,0,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int digitSix[] = {0,0,0,0, 1,0,0,0, 1,1,0,0, 0,0,1,0, 1,0,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1};
int digitSeven[] = {1,1,0,0, 0,1,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
int digitEight[] = {0,0,0,0, 1,0,0,0, 1,1,0,0, 0,0,1,0, 1,0,1,0, 1,1,1,0, 0,1,1,0, 1,1,1,1};
int digitNine[] = {1,0,0,0, 1,1,0,0, 0,0,1,0, 1,0,1,0, 0,1,1,0, 1,1,1,0, 1,1,1,1, 1,1,1,1};
int digitDot[] = {0,1,0,0, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};

/*****************************************************************************************
Setup function. Activates all pins and serial port
*****************************************************************************************/
void setup(){
    
  Serial.begin(9600);
  
  //lap leds
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  
  // sonar sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  // make each pin an output pin
  pinMode(muxS0Pin,OUTPUT);
  pinMode(muxS1Pin,OUTPUT);
  pinMode(muxS2Pin,OUTPUT);
  pinMode(muxS3Pin,OUTPUT);
  
  pinMode(digitZeroPin,OUTPUT);
  pinMode(digitOnePin,OUTPUT);
  pinMode(digitTwoPin,OUTPUT);
  pinMode(digitThreePin,OUTPUT);
} //  void setup()

/*****************************************************************************************
Setup function. Activates all pins and serial port
*****************************************************************************************/
void loop(){
  currentTime = millis(); // get current time
  mainFunction(); // call main function
  
  // for debugging
  //divideTime();
  //writeTime(displayTimeDigit0, displayTimeDigit1, displayTimeDigit2, displayTimeDigit3);
} // void loop()

/*****************************************************************************************
Function senses when sonar sensor is activated then displayes lap time when sensor trigged.
*****************************************************************************************/
void mainFunction(){
  
 if(!(didCarHitSensor)){
   if(checkIfSonarIsHigh()){
     didCarHitSensor = true;
     lastTime = millis();
     lapCounter++;
   } // if sonar is high then go to next step
 } // if car hasnt hit sensor then only check sensor
 
 if(didCarHitSensor && !(didCarPassSensor)){
   currentTime = millis() - lastTime;
   if(currentTime > timeToDelay){
     didCarPassSensor = true;
   } // if time has passed start showing time
 } // if car has pass sensor then wait for certain time to display time
 
 if(didCarPassSensor){
   currentTime = millis() - lastTime;
   divideTime();
   if(checkIfSonarIsHigh()){
     didCarPassSensor=false;
     lastTime = millis();
     lapCounter++;
   } // if car has passed sensor again then update lap time
 } // if car has passed sensor display lap time
 
 
 writeTime(displayTimeDigit0, displayTimeDigit1, displayTimeDigit2, displayTimeDigit3);
 if(didCarHitSensor)
   displayLap(); 
} // void mainFunction()

/*****************************************************************************************
take current time and split it up for display.
*****************************************************************************************/
void divideTime(){ 
  displayTimeDigit0 = currentTime/10000;
  displayTimeDigit1 = (currentTime - displayTimeDigit0*10000)/1000;
  displayTimeDigit2 = (currentTime - displayTimeDigit0*10000 - displayTimeDigit1*1000)/100;
  displayTimeDigit3 = (currentTime - displayTimeDigit0*10000 - displayTimeDigit1*1000 - displayTimeDigit2*100)/10;
} // void divideTime()

/*****************************************************************************************
Recieves 4 digits that were split up and activates each digit on display one by one in
order to upload array to display number.
*****************************************************************************************/
void writeTime(int displayTimeDigit0Temp, int displayTimeDigit1Temp, int displayTimeDigit2Temp, int displayTimeDigit3Temp){
 
 //pin low for digital 0 - the rest high
 digitalWrite(digitOnePin,LOW);
 digitalWrite(digitTwoPin, LOW);
 digitalWrite(digitThreePin,LOW);
 digitalWrite(digitZeroPin,HIGH);
 getArray(displayTimeDigit0Temp);
 
 //pin low for digital 1 - the rest high
 digitalWrite(digitZeroPin,LOW);
 digitalWrite(digitTwoPin,LOW);
 digitalWrite(digitThreePin,LOW);
 digitalWrite(digitOnePin,HIGH);
 getArray(displayTimeDigit1Temp);

 getArray(10); // activates colon
 
 //pin low for digital 2 - the rest high
 digitalWrite(digitOnePin,LOW);
 digitalWrite(digitThreePin,LOW);
 digitalWrite(digitZeroPin,LOW);
 digitalWrite(digitTwoPin,HIGH);
 getArray(displayTimeDigit2Temp);
 
 //pin low for digital 3 - the rest high
 digitalWrite(digitZeroPin,LOW);
 digitalWrite(digitOnePin,LOW);
 digitalWrite(digitTwoPin,LOW);
 digitalWrite(digitThreePin,HIGH);
 getArray(displayTimeDigit3Temp);
} // oid writeTime()

/*****************************************************************************************
Receives digit to display and passes array representation down to multiplexer.
*****************************************************************************************/
void getArray(int numberToUse) {
 if(numberToUse == 0)
   displayNumber(digitZero);
 if(numberToUse == 1)
   displayNumber(digitOne);
 if(numberToUse == 2)
   displayNumber(digitTwo);
 if(numberToUse == 3)
   displayNumber(digitThree);
 if(numberToUse == 4)
   displayNumber(digitFour);
 if(numberToUse == 5)
   displayNumber(digitFive);
 if(numberToUse == 6)
   displayNumber(digitSix);
 if(numberToUse == 7)
   displayNumber(digitSeven);
 if(numberToUse == 8)
   displayNumber(digitEight);
 if(numberToUse == 9)
   displayNumber(digitNine);
 if(numberToUse == 10)
   displayNumber(digitDot);
} // void getArray(int numberToUse)

/*****************************************************************************************
Receives array representation of number and activates multiplexer pins.
*****************************************************************************************/
void displayNumber(int numberArray[]) {
  for(int i=0;i<32;i++){
    // mux has a capacitance that will carry over - wait to drain cap for next led
    delayMicroseconds(500);
    digitalWrite(muxS0Pin,numberArray[i]);
    i++;
    digitalWrite(muxS1Pin,numberArray[i]);
    i++;
    digitalWrite(muxS2Pin,numberArray[i]);
    i++;
    digitalWrite(muxS3Pin,numberArray[i]);
  } // for(int i=0;i<32;i++)
} // void displayNumber(int numberArray[], int whichDigital)

/*****************************************************************************************
Checks in sonar sensor is high and retrns boolean values if sensor is high or not.
*****************************************************************************************/
boolean checkIfSonarIsHigh(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  // Compute distance
  distance = pulseIn(echoPin, HIGH, 1000); 
  distance = distance/ 148;
  Serial.println(distance);
  
  if(distance > 0){
    return true;
  } else{
    return false;
  } // if sensor detects opject retuen true else retuen false
} // boolean checkIfSonarIsHigh()

/*****************************************************************************************
display led binary representation of current lap.
*****************************************************************************************/
void displayLap(){
 if(lapCounter == 1){digitalWrite(LED0, HIGH); digitalWrite(LED1, LOW);}
 else if(lapCounter==2){digitalWrite(LED1, HIGH); digitalWrite(LED0, LOW);}
 else if(lapCounter==3){digitalWrite(LED0, HIGH); digitalWrite(LED1, HIGH);}
 else{lapCounter=1;}
} // void displayLap()
