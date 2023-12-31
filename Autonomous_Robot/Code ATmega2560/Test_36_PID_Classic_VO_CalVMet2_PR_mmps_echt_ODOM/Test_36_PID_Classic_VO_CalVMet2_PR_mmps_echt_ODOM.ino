#include <SimpleTimer.h>
#include <util/atomic.h>

//Variables temps
volatile double n = 0;
volatile unsigned long t0Ri = 0, t0Li = 0, t1Ri = 0, t1Li = 0;
volatile unsigned long t0R = 0, t0L = 0, t1R = 0, t1L = 0, t0 = 0, t1 = 0, t2 = 0;
volatile double dtRi = 0, dtLi = 0, t=0;
volatile double dtR = 0, dtL = 0, dt;
volatile double mindtL = (double) (88 / 1.0e6);
volatile double mindtR = (double) (68 / 1.0e6);
volatile double maxdtL = (double) (31620 / 1.0e6);
volatile double maxdtR = (double) (39072 / 1.0e6);
volatile double mindt = (double) (96 / 1.0e6);
volatile double maxdt = (double) (136884 / 1.0e6);
volatile double Te =8/1.0e3;
SimpleTimer timer;

//Variables stop
volatile boolean stopR = true, stopL = true;

//Variables encodeurs
#define outputAR 2
#define outputBR 3
#define outputAL 18
#define outputBL 19
volatile double xcmd=0;
volatile double ycmd=0;
volatile double anglecmd=0;
volatile double Oact = 0;
volatile double Rdist = 0;
volatile double Ldist = 0;
volatile double dist = 0;
volatile double dx = 0;
volatile double dy = 0;
volatile double x = 0;
volatile double y = 0;
volatile double Rdisti = 0;
volatile double Ldisti = 0;
volatile double prevRdist = 0;
volatile double prevLdist = 0;
volatile double prevRdisti = 0;
volatile double prevLdisti = 0;
volatile double prevdist = 0;
volatile double KR = (double) ((1024 * 2) / (PI *80));
volatile double KL = (double) ((600 * 2) / (PI *80));
volatile double distUnitR = (double)PI * 40 / 1024;
volatile double distUnitL = (double)PI * 40 / 600;

//Variables moteurs
#define forwardR 8
#define backwardR 9
#define forwardL 10
#define backwardL 11
volatile double pwmRcmd = 0;
volatile double pwmLcmd = 0;
volatile double pwmOcmd = 0;
volatile double vitesseLact = 0;
volatile double vitesseRact = 0;
volatile double vitesseOact = 0;
volatile double vitesseact = 0;
volatile double vitesseLactFilter = 0;
volatile double vitesseRactFilter = 0;
volatile double prevVRact = 0;
volatile double prevVLact = 0;
volatile double prevRact = 0;
volatile double prevLact = 0;


//Variables erreurs/commandes PIDposition
volatile double Rdistacc = 0;
volatile double Ldistacc = 0;
volatile double Rdistdec = 0;
volatile double Ldistdec = 0;

volatile double Rcmd = 0;
volatile double Lcmd = 0;
volatile double Ocmd = 0;
volatile double cmd = 0;
volatile double Rerror = 0;
volatile double Lerror = 0;
volatile double sommeRerror = 0;
volatile double sommeLerror = 0;

//Variables erreurs/commandes PIDvitesse
volatile double PIDVR = 0;
volatile double PIDVL = 0;
volatile double vitesseRcmd = 0;
volatile double vitesseLcmd = 0;
volatile double vitesseOcmd = 0;
volatile double vitessecmd = 0;
volatile double VRerror = 0;
volatile double VLerror = 0;
volatile double prevVRerror = 0;
volatile double prevVLerror = 0;
volatile double sommeVRerror = 0;
volatile double sommeVLerror = 0;
volatile double dVRerror = 0;
volatile double dVLerror = 0;


//Variables PID orientation
volatile double pwmO = 0 ;
volatile double Oerror = 0;
volatile double prevOerror = 0;
volatile double sommeOerror = 0;
volatile double dOerror = 0;

bool inRange(volatile double val, volatile double minimum, volatile double maximum) {
  return ((minimum <= val) && (val <= maximum));
}
double sign (volatile double val){
  if (val>=0)
    return 1;
  else
    return -1;
}
//Encoder functions
void ISRtrackAR() {
  Rdisti += (digitalRead(outputBR) == LOW) ? distUnitR : -distUnitR;
  t1Ri = micros();
  dtRi = ((double) ((t1Ri - t0Ri) / 1.0e6));
  t0Ri = t1Ri;
}
void ISRtrackBR() {
  Rdisti += (digitalRead(outputAR) == LOW) ? -distUnitR : distUnitR;
}

void ISRtrackAL() {
  Ldisti += ((digitalRead(outputBL) == LOW)) ? -distUnitL :  distUnitL;
  t1Li = micros();
  dtLi = ((double) ((t1Li - t0Li) / 1.0e6));
  t0Li = t1Li;
}

void ISRtrackBL() {
  Ldisti += ((digitalRead(outputAL) == LOW)) ? distUnitL :  -distUnitL;
}

//Motor functions
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
////////////////////////////////////////////////////////////////////////
//PID vitesse Parameters

volatile double kpVR = 5;
volatile double kpVL = 5;

volatile double kdVR = 0;
volatile double kdVL = 0;

volatile double kiVR = 0;
volatile double kiVL = 0;

//PID orientation Parameters

volatile double kpO = 1;
volatile double kdO = 0;
volatile double kiO = 0;

//Marges d'erreurs
//volatile double margePRdist = 10;
//volatile double margeNRdist = -10;
//volatile double margePLdist = 10;
//volatile double margeNLdist = -10;

volatile double margePVRerror = 1;
volatile double margeNVRerror = 1;
volatile double margePVLerror = 1;
volatile double margeNVLerror = 1;

volatile double margePRerror = 10;
volatile double margeNRerror = -10;
volatile double margePLerror = 10;
volatile double margeNLerror = -10;

volatile double margePOerror = 10;
volatile double margeNOerror = -10;

//Vitesses max min
volatile double pwmRcmdmax = 180;
volatile double pwmRcmdmin = 80;
volatile double pwmLcmdmax = 180;
volatile double pwmLcmdmin = 80;

//////////////////////////////////////////////////////
void PIDvitesseR() {
  if (!inRange(VRerror, margeNVRerror, margePVRerror)) {
    sommeVRerror += VRerror;
    dVRerror = (double) (VRerror - prevVRerror);
    prevVRerror = VRerror;
    PIDVR = (double) (kpVR * VRerror) + (kdVR * dVRerror) + kiVR * sommeVRerror;
    pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : PIDVR;
    pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : PIDVR;
  }
  else
    pwmRcmd = 0;
}

void PIDvitesseL() {
  if (!inRange(VLerror, margeNVLerror, margePVLerror)) {
    sommeVLerror += (double) VLerror;
    dVLerror = (double) (VLerror - prevVLerror);
    prevVLerror = VLerror;
    PIDVL = (double) (kpVL * VLerror) + (kdVL * dVLerror) + kiVL * sommeVLerror;
    pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : PIDVL;
    pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : PIDVL;
  }
  else
    pwmLcmd = 0;
}

void PIDorientation() {
//  Oerror = Ocmd - Oact;
  Oerror=Rdist-Ldist;
  if (!inRange(Oerror, margeNOerror, margePOerror)) {
    sommeOerror += (double) Oerror;
    dOerror = (double) (Oerror - prevOerror);
    prevOerror = Oerror;
    pwmOcmd = (double) (kpO * Oerror) + (kdO * dOerror) + kiO * sommeOerror;
  }
  else
    pwmOcmd = 0;
  pwmRcmd -= pwmOcmd;
  pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : pwmRcmd;
  pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : pwmRcmd;
  pwmLcmd += pwmOcmd;
  pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : pwmLcmd;
  pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : pwmLcmd;
}

void PIDposition() {

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      Rdist = Rdisti;
      Ldist = Ldisti;
      dtR = dtRi;
      dtL = dtLi;
    }
//    PIDorientation();
////////////////////////////////////////////////////////////////////////////////////////    
  //Right Wheel
    Rerror = Rcmd - Rdist;
    if (!inRange(dtR, mindtR, maxdtR)) {

      vitesseRact = 0;
    }
    else{
        vitesseRact = (double) ((Rdist - prevRdist)/ Te); // mm/s
        vitesseRactFilter = 0.854 * vitesseRactFilter + 0.0728 * vitesseRact + 0.0728 * prevVRact;
        vitesseRact = vitesseRactFilter;
    }
    stopR=(vitesseRact==0);
//////////////////////////////////////////////////////////////////////////////////////// 
  //Left Wheel
    Lerror = Lcmd - Ldist;
    if (!inRange(dtL, mindtL, maxdtL)) {

      vitesseLact = 0;
    }
    else{
        vitesseLact = (double) ((Ldist - prevLdist) / Te); // mm/s
        vitesseLactFilter = 0.854 * vitesseLactFilter + 0.0728 * vitesseLact + 0.0728 * prevVLact;
        vitesseLact = vitesseLactFilter;
    }
    stopL=(vitesseLact==0);
//////////////////////////////////////////////////////////////////////////////////////// 
    prevVRact = vitesseRact;
    prevVLact = vitesseLact;
    prevRdist=Rdist;
    prevLdist=Ldist;
    //Trapèze Right
    if (Rdist<Rdistacc){
      vitesseRcmd = ((double) (110 / Rdistacc)) * Rdist;
    }
    else if (inRange(Rdist,Rdistacc,Rcmd-Rdistdec)) {
      vitesseRcmd = 130;
    }
    else if (inRange(Rdist,Rcmd-Rdist,Rcmd)){
          vitesseRcmd=  ((double)(110 / Rcmd)) * Rerror;
    }
    else{
          vitesseRcmd = 0;
    }
    VRerror=vitesseRcmd-vitesseRact;
    PIDvitesseR();
//////////////////////////////////////////////////////////////////////////////////////// 
    //Trapèze Left
     if (Ldist<Ldistacc){
      vitesseLcmd = ((double) (110 / Ldistacc)) * Ldist;
    }
    else if (inRange(Ldist,Ldistacc,Lcmd-Ldistdec)) {
      vitesseLcmd = 110;
    }
    else if (inRange(Ldist,Lcmd-Ldist,Lcmd)){
      vitesseLcmd= ((double) (110 / Lcmd)) * Lerror;
    }
    else{
          vitesseLcmd = 0;
    }
    VLerror=vitesseLcmd-vitesseLact;
    PIDvitesseL();
//////////////////////////////////////////////////////////////////////////////////////// 
  //Odométrie
    dist=(double) ((Rdist+Ldist)/2);
    Oact+=(double) (Rdist-Ldist);
    vitesseact=dist-prevdist;
    prevdist=dist;
    dx=-vitesseact*sin(Oact);
    dy=vitesseact*cos(Oact);
    x+=dx;
    y+=dy;
//    Rcmd+=-sign(Rdist-prevRdist)*sqrt(dx*dx+dy*dy);
//    Lcmd+=-sign(Ldist-prevLdist)*sqrt(dx*dx+dy*dy);
}

void Go() {
//  if (stopR && stopL){
//    Rcmd=sqrt(xcmd*xcmd+ycmd*ycmd);
//    Lcmd=sqrt(xcmd*xcmd+ycmd*ycmd);

  Rcmd=ycmd;
  Lcmd=ycmd;
  Rdistacc = Rcmd * 0.15;
  Ldistacc = Lcmd * 0.15;
  Rdistdec = Rcmd * 0.25;
  Ldistdec = Lcmd * 0.25;
  PIDposition();
  motorR(pwmRcmd);
  motorL(pwmLcmd);
}

void setup() {
  Serial.begin(115200);
  
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

  timer.setInterval(Te, Go);
}
void loop() {
  timer.run();
  xcmd=0;
  ycmd=50;
  Ocmd=0;
  Serial.print("pwmRcmd= ");
  Serial.print(pwmRcmd);
  Serial.print(" vitesseRcmd= ");
  Serial.print(vitesseRcmd);
  Serial.print(" vitesseRact= ");
  Serial.print(vitesseRact);
  Serial.print(" VRerror= ");
  Serial.print(VRerror);
  Serial.print(" prevVRerror= ");
  Serial.print(prevVRerror);
//  Serial.print("\tvitesseLcmd= ");
//  Serial.print(vitesseLcmd);
//  Serial.print(" vitesseLact= ");
//  Serial.print(vitesseLact);
//  Serial.print(" VLerror= ");
//  Serial.print(VLerror);
//  Serial.print("\tOerror= ");
//  Serial.print(Oerror);
  Serial.print(" Rerror= ");
  Serial.println(Rerror);
//  Serial.print(" Lerror= ");
//  Serial.println(Lerror);
}
