//Variables encodeurs
#define outputAR 2
#define outputBR 3
#define outputAL 18
#define outputBL 19
volatile float Rdist = 0;
volatile float Ldist = 0;
volatile float distUnitR = (float)PI * 40 / 1024 / 10;
volatile float distUnitL = (float)PI * 40 / 600 / 10;

//Variables moteurs
float vitesseR = 0 ;
float vitesseL = 0 ;
float vitesseO = 0 ;
#define forwardR 6
#define backwardR 7
#define forwardL 5
#define backwardL 4

//Variables erreurs/commandes position
volatile float Rcmd = 100; //110.3
volatile float Lcmd = 100; //110.2
volatile float Rerror;
volatile float Lerror;
float prevRerror = 0;
float prevLerror = 0;
float sommeRerror = 0;
float sommeLerror = 0;

//Variables erreurs vitesse
volatile float Oerror;
float prevOerror = 0;
float sommeOerror = 0;

//Encoder functions
void ISRtrackAR() {
  Rdist += (digitalRead(outputBR) == LOW) ? distUnitR : -distUnitR;
}
void ISRtrackBR() {
  Rdist += (digitalRead(outputAR) == LOW) ? -distUnitR : distUnitR;
}

void ISRtrackAL() {
  Ldist += (digitalRead(outputBL) == LOW) ? distUnitL : -distUnitL;
}

void ISRtrackBL() {
  Ldist += (digitalRead(outputAL) == LOW) ? -distUnitL : distUnitL;
}

//Motor functions
void motorR(float vR) {
  if (vR > 0) {
    analogWrite(forwardR, vR);
    analogWrite(backwardR, 0);
  }
  else {
    analogWrite(forwardR, 0);
    analogWrite(backwardR, -5.2 * vR); //5.2
  }
}

void motorL(float vL) {
  if (vL > 0) {
    analogWrite(forwardL, vL);
    analogWrite(backwardL, 0);
  }
  else {
    analogWrite(forwardL, 0);
    analogWrite(backwardL, -5.2 * vL); //5.2
  }
}

//PID Position Parameters

float kpR = 0.01 ; // 0.35 // 0.00001
float kpL = 0.01 ; // 0.35 // 0.00001

float kdR = 58; // 7
float kdL = 58; // 7

float kiR = 0 ; // 0.000000000001
float kiL = 0; 

//DONT TOUCH
float kpO = 10;
float kdO = 0; 
float kiO = 0.0000000000000000000001;

void PIDorientation() {
  Oerror = Rdist - Ldist;
  sommeOerror += Oerror;
  vitesseO = (kpO * Oerror) + (kdO * (Oerror - prevOerror)) + kiO * sommeOerror;
  prevOerror = Oerror;
}

void PIDposition() {

  Rerror = Rcmd - Rdist;
  sommeRerror +=  Rerror ;

  Lerror = Lcmd - Ldist;
  sommeLerror +=  Lerror ;
  
  PIDorientation();

  vitesseR = (kpR * Rerror) + (kdR * (Rerror - prevRerror)) + kiR * sommeRerror;
  vitesseL = (kpL * Lerror) + (kdL * (Lerror - prevLerror)) + kiL * sommeLerror;

  vitesseR = (vitesseR > 180) ? 180 : vitesseR;
  vitesseL = (vitesseL > 180) ? 180 : vitesseL;

  if (Rdist + Ldist < 20) {
    vitesseR = vitesseR / 10 * Rdist;
    vitesseL = vitesseL / 10 * Ldist;
  }

  vitesseR = ((vitesseR < 72) && (Rerror > 0)) ? 72 : vitesseR; // min 72
  vitesseL = ((vitesseL < 72) && (Lerror > 0)) ? 72 : vitesseL;// min 72

  vitesseR -= vitesseO;
  vitesseL += vitesseO;

  //Update errors
  prevRerror = Rerror;
  prevLerror = Lerror;

  motorR(vitesseR);
  motorL(vitesseL);
}
void AutoPID(){
    //RightPID
    if (Rdist<Rcmd){
      kpR+=0.25;
      PIDposition();
    }
    else if(Rdist>Rcmd*1.15){
        kdR+=0.25;
        PIDposition();
      }
      else if(Rdist>Rcmd*1.07){
        kiR+=0.0000000001;
        PIDposition();
      }    
      else          
        motorR(0);
      
     //LeftPID 
    if (Ldist<Lcmd){
      kpL+=0.25;
      PIDposition();
    }
    else if(Ldist>Lcmd*1.15){
        kdL+=0.25;
        PIDposition();
      }
      else if(Ldist>Lcmd*1.07){
        kiL+=0.0000000001;
        PIDposition();
      }    
      else          
        motorL(0);
}

void setup() {
  Serial.begin(9600);

  //Command Encoders
  pinMode(outputAR, INPUT_PULLUP);
  pinMode(outputBR, INPUT_PULLUP);
  pinMode(outputAL, INPUT_PULLUP);
  pinMode(outputBL, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(outputAR), ISRtrackAR, RISING);
  attachInterrupt(digitalPinToInterrupt(outputAL), ISRtrackAL, RISING);
  attachInterrupt(digitalPinToInterrupt(outputBR), ISRtrackBR, RISING);
  attachInterrupt(digitalPinToInterrupt(outputBL), ISRtrackBL, RISING);

  //Command Motors
  pinMode(forwardR, OUTPUT);
  pinMode(backwardR, OUTPUT);
  pinMode(forwardL, OUTPUT);
  pinMode(backwardL, OUTPUT);
}

void loop() {

  PIDposition();
  //AutoPID();
  /*Serial.print(Ldist);
  Serial.print(" ");
  Serial.print(Lerror);
  Serial.print(" ");
  Serial.print(Rdist);
  Serial.print(" ");
  Serial.println(Rerror);*/
  
  Serial.print("Ldist= ");
  Serial.print(Ldist);
  Serial.print("\tLerror= ");
  Serial.print(Lerror);
  Serial.print("\tRdist= ");
  Serial.print(Rdist);
  Serial.print("\tRerror= ");
  Serial.println(Rerror);
  /*Serial.print("\tLcmd= ");
  Serial.print(Lcmd);
  Serial.print("\tRcmd= ");
  Serial.println(Rcmd);*/

  /*if (Rerror>0 && Lerror>0){
    motorR(82);
    motorL(92);
    }
    else
     if(Rerror>0){
        motorR(82);
        motorL(0);
      }
      else
        if (Lerror>0){
          motorR(0);
          motorL(92);
        }
        else{
          motorR(0);
          motorL(0);
        }*/
}
