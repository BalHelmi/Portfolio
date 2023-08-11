#define outputAR 2
#define outputBR 3
#define outputAL 20
#define outputBL 21



int ARlaststate;
int BRlaststate;
int ALlaststate;
int BLlaststate;
int ARstate,ALstate;
float Rcmd=500,Rerror;

int Rdistance=0,Ldistance=0;
float distUnit=2*PI*40/600;

void rightDist(){
 if(BRlaststate!=ARlaststate)
    Rdistance+=distUnit;
  else
    Rdistance-=distUnit;
  Serial.print("Rdistance= ");
  Serial.println(Rdistance);
  ARlaststate=ARstate;
}
void leftDist(){
  ALstate=digitalRead(outputAL);
  if(ALstate!=ALlaststate)
    if(BLlaststate!=ALstate)
      Ldistance+=distUnit;
    else
      Ldistance-=distUnit;
  Serial.print("Ldistance= ");
  Serial.println(Rdistance);
  ALlaststate=ALstate;
}

void setup() {
Serial.begin(9600);
pinMode(outputAR, INPUT);
pinMode(outputBR, INPUT);
pinMode(outputAL, INPUT);
pinMode(outputBL, INPUT);

//attachInterrupt(0,rightDist(),RISING);
//attachInterrupt(1,,RISING);

ARlaststate=digitalRead(outputAR);
BRlaststate=digitalRead(outputBR);
ALlaststate=digitalRead(outputAL);
BLlaststate=digitalRead(outputBL); 
}
void loop() {
  rightDist();
  Rerror=Rcmd-Rdistance;
  Serial.print("Rerror= ");
  Serial.println(Rerror);
  
}
