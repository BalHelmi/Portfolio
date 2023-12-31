//Variables encodeurs
#define outputAR 2
#define outputBR 3
#define outputAL 18
#define outputBL 19
volatile unsigned int temp=20;
volatile float Rdist=0.0;
volatile float Ldist=0.0;
volatile float distUnitR=(float)PI*40/1024/10;
volatile float distUnitL=(float)PI*40/600/10;

//Variables moteurs
float vitesseR = 0 ;
float vitesseL = 0 ;
float vitesseO = 0;
#define forwardR 6
#define backwardR 7
#define forwardL 5
#define backwardL 4

//Variables erreurs/commandes position
volatile float Rcmd=100;
volatile float Lcmd=100;
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
void ISRtrackAR(){
  Rdist+= (digitalRead(outputBR)==LOW) ? distUnitR : -distUnitR;
}
void ISRtrackBR(){
  Rdist+= (digitalRead(outputAR)==LOW) ? -distUnitR : distUnitR;
}

void ISRtrackAL(){
  Ldist+= (digitalRead(outputBL)==LOW) ? distUnitL : -distUnitL;
}

void ISRtrackBL(){
  Ldist+= (digitalRead(outputAL)==LOW) ? -distUnitL : distUnitL;
}

//Motor functions
void motorR(float vR) {
  if (vR>0){
    analogWrite(forwardR,vR);
    analogWrite(backwardR,0);
  }
  else{
    analogWrite(forwardR,0);
    analogWrite(backwardR,-vR);
  }
}

void motorL(float vL) {
  if (vL>0){
    analogWrite(forwardL,vL);
    analogWrite(backwardL,0);
  }
  else{
    analogWrite(forwardL,0);
    analogWrite(backwardL,-vL);
  }
}

//PID Position Parameters
/*float k1 = 0.04 ;
float k2 = 0.9;

float kpR = 2.3 ; 
float kpL = 2.5 ;

float kdR = 4.5 ;
float kdL = 5.7 ;

float kiR = 0 ; 
float kiL = 0 ;*/


float k1 = 0.01 ;
float k2 = 0.95;

float kpR = 2.1 ; 
float kpL = 2.19 ;

float kdR = 4.5 ;
float kdL = 4.5 ;

float kiR = 0 ; 
float kiL = 0 ;

float kpO = 1;
float kdO = 0;
float kiO = 0;

void PIDorientation(){
  Oerror=Rdist-Ldist;
  sommeOerror+=Oerror;

  vitesseO = (kpO * Oerror) + (kdO * (Oerror - prevOerror)) + kiO * sommeOerror;

  prevOerror = Oerror; 
}

//PID Position + Asservissement Vitesse
void PIDposition() {
  
  PIDorientation();
  
  vitesseR = (kpR * Rerror) + (kdR * (Rerror - prevRerror)) + kiR * sommeRerror-vitesseO;
  vitesseL = (kpL * Lerror) + (kdL * (Lerror - prevLerror)) + kiL * sommeLerror+vitesseO;

  vitesseR = (vitesseR > 255) ? vitesseR * k1 * Rdist : vitesseR;
  vitesseL = (vitesseL > 255) ? vitesseL * k2 * Ldist : vitesseL;

  //Update errors
  prevRerror = Rerror;
  prevLerror = Lerror;

  motorR(vitesseR);
  motorL(vitesseL);
}

void setup() {
  Serial.begin(9600);
  
  //Command Encoders
  pinMode(outputAR, INPUT_PULLUP);
  pinMode(outputBR, INPUT_PULLUP);
  pinMode(outputAL, INPUT_PULLUP);
  pinMode(outputBL, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(outputAR),ISRtrackAR,RISING);
  attachInterrupt(digitalPinToInterrupt(outputAL),ISRtrackAL,RISING);
  attachInterrupt(digitalPinToInterrupt(outputBR),ISRtrackBR,RISING);
  attachInterrupt(digitalPinToInterrupt(outputBL),ISRtrackBL,RISING);

  //Command Motors
  pinMode(forwardR,OUTPUT);
  pinMode(backwardR, OUTPUT);
  pinMode(forwardL, OUTPUT);
  pinMode(backwardL, OUTPUT);
}

void loop() {
  Rerror = Rcmd - Rdist;
  sommeRerror +=  Rerror ;
  
  Lerror = Lcmd - Ldist;
  sommeLerror +=  Lerror ;

  PIDposition();
  Serial.print(Rcmd);
  Serial.print(" ");
  Serial.print(Rdist);
  Serial.print(" ");
  Serial.print(vitesseR);
  Serial.print(" ");
  Serial.print(Lcmd);
  Serial.print(" ");
  Serial.print(Ldist);
  Serial.print(" ");
  Serial.println(vitesseR);
  
  /*Serial.print("Ldist= ");
  Serial.print(Ldist);
  Serial.print("\tLerror= ");
  Serial.print(Lerror);
  Serial.print("\t\tRdist= ");
  Serial.print(Rdist);
  Serial.print("\tRerror= ");
  Serial.println(Rerror);*/

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
