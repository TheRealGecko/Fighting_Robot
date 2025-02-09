/*
  Names: Danya Cheng, Fatma Jadoon, Bethany Lum
  Teacher: Mr. Wong
  Due Date: Friday, March 8, 2024
  This is the program controlling our arduino circuit for
  our SumoBot.
*/


//DC Motor Pins
const int rightForward = 3;
const int rightBackward = 2;
const int leftForward = 5;
const int leftBackward = 4;
const int enA = 10; //Enabler pin 1
const int enB = 11; //Enabler pin 2


//LDR Sensor Values
int lightValue1 = 0; //Right
int lightValue2 = 0; //Back
int lightValue3 = 0; //Left
int prevLightValue1 = 0; //Right
int prevLightValue2 = 0; //Back
int prevLightValue3 = 0; //Left
//Difference in Light Sensor Value
int diff1 = abs(lightValue1 - prevLightValue1); //Right
int diff2 = abs(lightValue2 - prevLightValue2); //Back
int diff3 = abs(lightValue3 - prevLightValue3); //Left
int sensorTolerance = 20; //Tolerance of Sensor
int whiteNum = 250; //Light Value of White Border


//Measure Ultrasonic Sensor
long durationR;
long durationL;


//Ultrasonic Sensor Distance
int distanceCmR;
int distanceCmL;
int senseDistance = 60; //Maximum Distance for Sensing Opponent


//The Last Mode (Right, Left, or Spin)
char lastMode;


/*
  Setup function to setup the pins
*/
void setup()
{
  //Motors
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
  pinMode(enA , OUTPUT);
  pinMode(enB , OUTPUT);

  //LDRs
  pinMode(A0, INPUT);
  pinMode(A5, INPUT);
  pinMode(A2, INPUT);
  prevLightValue1 = analogRead(A0); //Right
  prevLightValue2 = analogRead(A5); //Back
  prevLightValue3 = analogRead(A2); //Left
  lightValue1 = analogRead(A0); //Right
  lightValue2 = analogRead(A5); //Back
  lightValue3 = analogRead(A2); //Left

  //Ultrasonic
  pinMode(6, OUTPUT);
  pinMode(18, OUTPUT);
 
  forward(255);
  delay(100);
  search('r');
  lastMode = 'r';
}


/*
    Main loop function to call all the necessary functions
*/
void loop()
{
  ultrasonicSensorR();
  ultrasonicSensorL();
  lightSensor();


  if(distanceCmR <= senseDistance && distanceCmL <= senseDistance ){
    forward(255);
  }
  else if(distanceCmR <= senseDistance && distanceCmL > senseDistance ){
    right(255);
    lastMode = 'r';
  }
  else if(distanceCmL <= senseDistance  && distanceCmR > senseDistance ){
    left(255);
    lastMode = 'l';      
  }
  else {
    search(lastMode);
  }
}


/*
    Function to search for other SumoBot
 */
void search(char lastMode) {
  if(distanceCmR >= senseDistance && distanceCmL >= senseDistance ){
    if(lastMode == 'r'){
      right(255);
    }
    else{
      left(255);
    }
  }
}


/*
  Function to manage the LDRs
*/
void lightSensor() {
  prevLightValue1 = lightValue1; //Right
  prevLightValue2 = lightValue2; //Back
  prevLightValue3 = lightValue3; //Left
  lightValue1 = analogRead(A0); //Right
  lightValue2 = analogRead(A5); //Back
  lightValue3 = analogRead(A2); //Left
  diff1 = lightValue1 - prevLightValue1; //Right
  diff2 = lightValue2 - prevLightValue2; //Back
  diff3 = lightValue3 - prevLightValue3; //Left
  if(lightValue1 >  whiteNum || lightValue2 >  whiteNum || lightValue3 >  whiteNum ){
    if (diff1 >  sensorTolerance && diff3 >  sensorTolerance) {
      backward(255);
      delay(400);
    }
    else if (diff3 >  sensorTolerance && diff1 <  sensorTolerance) {
      backward(255);
      delay(400);
      lastMode = 'r';
    }
    else if (diff1 >  sensorTolerance && (diff3 <  sensorTolerance)) {
      backward(255);
      delay(400);
      lastMode = 'l';
    }
    else if (diff2 >  sensorTolerance) {
      forward(255);
      delay(400);
    }
  }
}


/*
    Function to manage the right ultrasonic sensor
*/
void ultrasonicSensorR()
{
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  durationR = pulseIn(15, HIGH);
  distanceCmR = durationR / 29 / 2;
  return distanceCmR;
}


/*
    Function to manage the left ultrasonic sensor
*/
int ultrasonicSensorL()
{
  digitalWrite(18, LOW);
  delayMicroseconds(2);
  digitalWrite(18, HIGH);
  delayMicroseconds(10);
  digitalWrite(18, LOW);
  durationL = pulseIn(17, HIGH);
  distanceCmL = durationL / 29 / 2;
  return distanceCmL;
}


/*
    Function to move SumoBot forwards.
*/
void forward(int speed)
{
  lightSensor();
  analogWrite(enB , speed);
  digitalWrite(leftForward , HIGH);
  digitalWrite(leftBackward , LOW);
  analogWrite(enA , speed);
  digitalWrite(rightForward , HIGH);
  digitalWrite(rightBackward , LOW);
}


/*
    Function to move SumoBot backwards.
*/
void backward(int speed)
{
  lightSensor();
  analogWrite(enB , speed);
  digitalWrite(leftForward , LOW);
  digitalWrite(leftBackward , HIGH);
  analogWrite(enA , speed);
  digitalWrite(rightForward , LOW);
  digitalWrite(rightBackward , HIGH);
}


/*
    Function to move SumoBot left.
*/
void left(int speed)
{
  lightSensor();
  analogWrite(enB , speed);
  digitalWrite(leftForward , LOW);
  digitalWrite(leftBackward , LOW);
  analogWrite(enA , speed);
  digitalWrite(rightForward , HIGH);
  digitalWrite(rightBackward , LOW);
}


/*
    Function to move SumoBot right.
*/
void right(int speed)
{
  analogWrite(enB , speed);
  digitalWrite(leftForward , HIGH);
  digitalWrite(leftBackward , LOW);
  analogWrite(enA , speed);
  digitalWrite(rightForward , LOW);
  digitalWrite(rightBackward , LOW);
}
