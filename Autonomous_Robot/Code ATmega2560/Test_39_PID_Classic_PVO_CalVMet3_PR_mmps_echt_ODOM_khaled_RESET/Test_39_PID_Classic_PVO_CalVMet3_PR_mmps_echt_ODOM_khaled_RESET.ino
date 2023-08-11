#include <SimpleTimer.h>
#include <util/atomic.h>

//Variables temps
volatile double n = 0;
int compteur =0;
volatile double Te =15/1.0e3;
SimpleTimer timer;

//Variables stop
volatile boolean stopR = true, stopL = true;

//Variables encodeurs
#define outputAR 2
#define outputBR 3
#define outputAL 18
#define outputBL 19
volatile double Rdist = 0;
volatile double Ldist = 0;
volatile double dist = 0;
volatile double Rdisti = 0;
volatile double Ldisti = 0;
volatile double prevRdist = 0;
volatile double prevLdist = 0;
volatile double prevRdisti = 0;
volatile double prevLdisti = 0;
volatile double prevdist = 0;
volatile double KR = (double) ((1024 * 2) / (PI *80));
volatile double KL = (double) ((600 * 2) / (PI *80));
volatile double distUnitR = (double)PI * 40 / 600;
volatile double distUnitL = (double)PI * 40 / 1024;

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
volatile double cmd  = 0;
volatile double Rerror = 0;
volatile double Lerror = 0;
volatile double sommeRerror = 0;
volatile double sommeLerror = 0;
volatile double prevRerror = 0;
volatile double prevLerror = 0;

//Variables erreurs/commandes PIDvitesse
volatile double PIDVR = 0;
volatile double PIDVL = 0;
volatile double vitesseRcmd = 0;
volatile double vitesseLcmd = 0;
volatile double prevvitesseRcmd = 0;
volatile double prevvitesseLcmd = 0;
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
}
void ISRtrackBR() {
  Rdisti += (digitalRead(outputAR) == LOW) ? -distUnitR : distUnitR;
}

void ISRtrackAL() {
  Ldisti += ((digitalRead(outputBL) == LOW)) ? distUnitL :  -distUnitL;
}

void ISRtrackBL() {
  Ldisti += ((digitalRead(outputAL) == LOW)) ? -distUnitL :  distUnitL;
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
// PID Position Parameters

#define kpR 1.9
#define kpL 1.9//  5.43

#define kdR 0
#define kdL 0

#define kiR 0
#define kiL 0

//PID vitesse Parameters

#define kpVR 21
#define kpVL 13.75 // 150

#define kdVR 11
#define kdVL 3.7

#define kiVR 0.012
#define kiVL 0.005065

//PID orientation Parameters

#define kpO 13// 3
#define kdO 0// 2.5
#define kiO 0

//Marges d'erreurs

#define margePVRerror 1
#define margeNVRerror -1
#define margePVLerror 1
#define margeNVLerror -1

#define margePRerror 10
#define margeNRerror -10
#define margePLerror 10
#define margeNLerror -10

#define margePOerror 10
#define margeNOerror -10

//Vitesses/Acc max min
#define pwmRcmdmax 240
#define pwmRcmdmin -80
#define pwmLcmdmax 240
#define pwmLcmdmin -80

#define vitesseRcmdmax 7  // mm/s
#define vitesseRcmdmin -7 // mm/s  
#define vitesseLcmdmax 7// mm/s
#define vitesseLcmdmin -7 // mm/s

#define ARcmdmax 3.7    // mm/s²
#define ARcmdmin -3.7    // mm/s²
#define ALcmdmax 3.7    // mm/s²
#define ALcmdmin -3.7    // mm/s²

//////////////////////////////////////////////////////
void PIDvitesseR() {
    vitesseRact = (double) (Rdist - prevRdist); // mm/s
//    vitesseRactFilter = 0.854 * vitesseRactFilter + 0.0728 * vitesseRact + 0.0728 * prevVRact;
//    vitesseRact = vitesseRactFilter;
    stopR=(vitesseRact==0);
    prevRdist=Rdist;
    prevVRact = vitesseRact;
    prevVRerror=VRerror;
    VRerror=vitesseRcmd-vitesseRact;
    sommeVRerror += VRerror;
    prevVRerror = VRerror;
    PIDVR = (double) (kpVR * VRerror) + (kdVR * (VRerror - prevVRerror)) + kiVR * sommeVRerror;
    pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : PIDVR;
    pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : PIDVR;
}

void PIDvitesseL() {
    vitesseLact = (double) (Ldist - prevLdist); // mm/s
//    vitesseLactFilter = 0.854 * vitesseLactFilter + 0.0728 * vitesseLact + 0.0728 * prevVLact;
//    vitesseLact = vitesseLactFilter;
    stopL=(vitesseLact==0);
    prevLdist=Ldist;
    prevVLact = vitesseLact;
    prevVLerror=VLerror;
    VLerror=vitesseLcmd-vitesseLact;
    sommeVLerror += (double) VLerror;
    prevVLerror = VLerror;
    PIDVL = (double) (kpVL * VLerror) + (kdVL * (VLerror - prevVLerror)) + kiVL * sommeVLerror;
    pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : PIDVL;
    pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : PIDVL;
}

void PIDorientation() {
//  Oerror = Ocmd - Oact;
  Oerror=Rdist-Ldist;
  sommeOerror += (double) Oerror;
  prevOerror = Oerror;
  pwmOcmd = (double) (kpO * Oerror) + (kdO * (Oerror - prevOerror)) + kiO * sommeOerror;
  pwmRcmd -= pwmOcmd;
  pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : pwmRcmd;
  pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : pwmRcmd;
  pwmLcmd += pwmOcmd;
  pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : pwmLcmd;
  pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : pwmLcmd;
}
void PIDorientationDoura() {
//  Oerror = Ocmd - Oact;
  Oerror=abs(Rdist)- abs(Ldist);
  sommeOerror += (double) Oerror;
  prevOerror = Oerror;
  pwmOcmd = (double) (kpO * Oerror) + (kdO * (Oerror - prevOerror)) + kiO * sommeOerror;
  if(Lcmd<0){
  pwmRcmd -= pwmOcmd;
  pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : pwmRcmd;
  pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : pwmRcmd;
  pwmLcmd -= pwmOcmd;
  pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : pwmLcmd;
  pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : pwmLcmd;
  }
  else{
  pwmRcmd += pwmOcmd;
  pwmRcmd = (pwmRcmd<pwmRcmdmin) ? pwmRcmdmin : pwmRcmd;
  pwmRcmd = (pwmRcmd>pwmRcmdmax) ? pwmRcmdmax : pwmRcmd;
  pwmLcmd += pwmOcmd;
  pwmLcmd = (pwmLcmd<pwmLcmdmin) ? pwmLcmdmin : pwmLcmd;
  pwmLcmd = (pwmLcmd>pwmLcmdmax) ? pwmLcmdmax : pwmLcmd;
  }
}

void PIDposition() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      Rdist = Rdisti;
      Ldist = Ldisti;
    }
//PID Position Right
    prevvitesseRcmd=vitesseRcmd;
    prevRerror=Rerror;
    Rerror = Rcmd - Rdist;
    sommeRerror+=Rerror;
    vitesseRcmd=(double) (kpR*Rerror) + (kdR*(Rerror-prevRerror)) + (kiR*sommeRerror);
    vitesseRcmd=(vitesseRcmd>vitesseRcmdmax) ? vitesseRcmdmax : vitesseRcmd;
    vitesseRcmd=(vitesseRcmd<vitesseRcmdmin) ? vitesseRcmdmin : vitesseRcmd;
    vitesseRcmd=(vitesseRcmd-prevvitesseRcmd>ARcmdmax) ? prevvitesseRcmd+ARcmdmax : vitesseRcmd;
    vitesseRcmd=(vitesseRcmd-prevvitesseRcmd<ARcmdmin) ? prevvitesseRcmd+ARcmdmin : vitesseRcmd;
    PIDvitesseR();
//PID Position Left
    prevvitesseLcmd=vitesseLcmd;
    prevLerror=Lerror;
    Lerror = Lcmd - Ldist;
    sommeLerror+=Rerror;
    vitesseLcmd=(double) (kpL*Lerror) + (kdL*(Lerror-prevLerror)) + (kiL*sommeLerror);
    vitesseLcmd=(vitesseLcmd>vitesseLcmdmax) ? vitesseLcmdmax : vitesseLcmd;
    vitesseLcmd=(vitesseLcmd<vitesseLcmdmin) ? vitesseLcmdmin : vitesseLcmd;
    vitesseLcmd=(vitesseLcmd-prevvitesseLcmd>ALcmdmax) ? prevvitesseLcmd+ALcmdmax : vitesseLcmd;
    vitesseLcmd=(vitesseLcmd-prevvitesseLcmd<ALcmdmin) ? prevvitesseLcmd+ALcmdmin : vitesseLcmd;
    PIDvitesseL();
  //Odométrie
//    dist=(double) ((Rdist+Ldist)/2);
//    Oact+=(double) (Rdist-Ldist);
//    vitesseact=dist-prevdist;
//    prevdist=dist;
//    dx=-vitesseact*sin(Oact);
//    dy=vitesseact*cos(Oact);
//    x+=dx;
//    y+=dy;
//    Rcmd+=-sign(Rdist-prevRdist)*sqrt(dx*dx+dy*dy);
//    Lcmd+=-sign(Ldist-prevLdist)*sqrt(dx*dx+dy*dy);
if(Rcmd*Lcmd >= 0)
    PIDorientation();
    else
    PIDorientationDoura();
}

void Go() {
//  if (stopR && stopL)
  PIDposition();
  motorR(pwmRcmd);
  motorL(pwmLcmd);
}
void reset(){
  Lcmd =0;
  Rcmd = 0;
  Ldist =0;
  Rdist =0;
  Rdisti = 0;
 Ldisti = 0;
 prevRdist = 0;
 prevLdist = 0;
 prevRdisti = 0;
 prevLdisti = 0;
 prevdist = 0;
 pwmRcmd = 0;
  pwmLcmd = 0;
  pwmOcmd = 0;
  vitesseLact = 0;
  vitesseRact = 0;
  vitesseOact = 0;
  vitesseact = 0;
  prevVRact = 0;
  prevVLact = 0;
  prevRact = 0;
  prevLact = 0;
 
 Rdistacc = 0;
 Ldistacc = 0;
 Rdistdec = 0;
 Ldistdec = 0;

 Rcmd = 0;
 Lcmd = 0;
 Ocmd = 0;
 cmd  = 0;
 Rerror = 0;
 Lerror = 0;
 sommeRerror = 0;
 sommeLerror = 0;
prevRerror = 0;
 prevLerror = 0;

//Variables erreurs/commandes PIDvitesse
 PIDVR = 0;
 PIDVL = 0;
 vitesseRcmd = 0;
vitesseLcmd = 0;
 prevvitesseRcmd = 0;
 prevvitesseLcmd = 0;
 vitesseOcmd = 0;
 vitessecmd = 0;
 VRerror = 0;
VLerror = 0;
 prevVRerror = 0;
 prevVLerror = 0;
sommeVRerror = 0;
 sommeVLerror = 0;
dVRerror = 0;
 dVLerror = 0;


//Variables PID orientation
 pwmO = 0 ;
 Oerror = 0;
 prevOerror = 0;
 sommeOerror = 0;
 dOerror = 0;
  
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

  timer.setInterval(Te,Go);
}
void loop() {
  timer.run();

//  if(compteur == 0){
//    
//  
//  Lcmd= 800;
//  Rcmd=800;
//  if(vitesseRact<=0.5 && vitesseRact>=-0.5 && vitesseLact<=0.5 && vitesseLact>=-0.5 && Rdist > 100)
//  compteur++;
//  }
//  if(compteur == 1){
//    reset();
//  compteur++;
//  }
//    if(compteur == 2){
//      while(!(vitesseRact<=0.5 && vitesseRact>=-0.5 && vitesseLact<=0.5 && vitesseLact>=-0.5 && Rdist > 20)){
//   Lcmd= - (((PI)*93));
//  Rcmd=(((PI)*93));
//      }
////  compteur++;
//    }

  
  Serial.print("Lerror= ");
  Serial.print(Lerror);
  Serial.print(" vitesseLcmd= ");
  Serial.print(vitesseLcmd);
  Serial.print(" vitesseLact= ");
  Serial.print(vitesseLact);
  Serial.print(" VLerror= ");
  Serial.print(VLerror);
  Serial.print("\t\tRerror= ");
  Serial.print(Rerror);
  Serial.print(" vitesseRcmd= ");
  Serial.print(vitesseRcmd);
  Serial.print(" vitesseRact= ");
  Serial.print(vitesseRact);
  Serial.print(" VRerror= ");
  Serial.print(VRerror);
  
   Serial.print(" compteur= ");
  Serial.println(compteur);
}
