#define forwardR 8
#define backwardR 9
#define forwardL 10
#define backwardL 11
volatile boolean stopR=true, stopL=true;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(forwardR, OUTPUT);
  pinMode(backwardR, OUTPUT);
  pinMode(forwardL, OUTPUT);
  pinMode(backwardL, OUTPUT);

  stopR= !((digitalRead(forwardR) == HIGH) || (digitalRead(backwardR) == HIGH));
  stopL= !((digitalRead(forwardL) == HIGH) || (digitalRead(backwardL) == HIGH));
}
void motorR(volatile double vR) {
  if (vR > 0) {
    analogWrite(forwardR, vR);
    analogWrite(backwardR, 0);
  }
  else {
    analogWrite(forwardR, 0);
    analogWrite(backwardR, -vR);
  }
}
void motorL(volatile double vL) {
  if (vL > 0) {
    analogWrite(forwardL, vL);
    analogWrite(backwardL, 0);
  }
  else {
    analogWrite(forwardL, 0);
    analogWrite(backwardL, -vL);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  motorR(0);
  motorL(80);
   Serial.print("stopR= ");
   Serial.print(stopR);
   Serial.print("stopL= ");
   Serial.println(stopL);
}
