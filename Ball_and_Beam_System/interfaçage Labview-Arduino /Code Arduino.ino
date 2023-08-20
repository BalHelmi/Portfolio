

#include <Servo.h>
// 130 --> 0 
//Pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int windowSize = 20;
int commandSmooth ; 
int distances[windowSize];
int Oldcommand = 1500 ; 
int currentIndex = 0;
int servoPin = 6 ; 
//Variables
long duration;          //Time it Took for Sound to Comeback
int distance;           //Location of Ball from Sensor
int pos = 0;          //Servo Position
double StartAngle = 1500;    //Angle of servo when beam is parallel to the ground
double pError,iError,dError;  //Error values associated with PID
double Output;          //Sum of error values multiplied by their corresponding gains
double prevError;         //Error from previous iteration
unsigned long prevTime,now;   //Previous time and current time
double dt;            //Change in time  
int receivedValue = 0;

int Setpoint = 25;         //Desired point on the beam 
//Gains
float Kp = 35;      //Proportional constant
float Ki = 0;       //Integration constant
float Kd = 5;  

Servo servo;            //Create servo object to control the servo

//Initial setup
void setup() {
  pinMode(trigPin, OUTPUT);   //Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    //Sets the echoPin as an Input

  servo.attach(servoPin);     //Attaches the servo on pin 13 to the servo object
  servo.write(StartAngle);    //Initiate the motor to the horizonal angle
    pinMode(13,OUTPUT);

  Serial.begin(9600);       //Starts the serial communication
}




void loop() {
  
 while (true) {
 long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration / 58  ;
  if (distance>35){
    distance = 35;
  }
  Serial.print(distance);
  Serial.print('\t'); 
  distances[currentIndex] = distance;
  currentIndex = (currentIndex + 1) % windowSize;
  float sum = 0;
  for (int i = 0; i < windowSize; i++) {
    sum += distances[i];
  }
  
  int filteredDistance = sum / windowSize;
  
  //Serial.print("Distance: ");
  //Serial.println(Setpoint);
  //Serial.print("Distance: ");
  Serial.println(filteredDistance);
  //Serial.print("\tSet Point: ");
  //Serial.println(10*Setpoint);
  

  now = millis();
  dt = (now - prevTime);
  pError = Setpoint - filteredDistance; // 25 - 10 = 15
  dError = (pError - prevError) / dt;
  iError = iError + (pError * dt);
  if (iError > 10) {
    iError = 10;        //Error greater than 10 are ignored; noise
  }

  Output = Kp * pError + Ki * iError + Kd * dError;
  //Serial output
  //Serial.print("PID: ");
  //Serial.println(Output);
  
  //Upper and Lower Bound of PID
  if (Output >800) {
    Output = 800;
  }
  if (Output <-500) {
    Output = -500;
  }

  servo.writeMicroseconds(StartAngle+Output);
  //Serial.println(StartAngle+Output);
  //Oldcommand = (StartAngle+Output) * 0.05;
  
  //Set previous error and time
  prevError = pError;
  prevTime = now;


 }
}
