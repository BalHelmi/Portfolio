int sensor1 = 2;
int sensor2 = 3;
int sensor3 = 4;
int sensor4 = 5;      

int S1;
int S2;
int S3;
int S4;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);

  Serial.begin(9600);                     
  delay(500);
}

void loop() {
  S1 = !digitalRead(sensor1);//S1 = !digitalRead(sensor1);
  S2 = !digitalRead(sensor2);//S2 = !digitalRead(sensor2);
  S3 = !digitalRead(sensor3);//S3 = !digitalRead(sensor1);
  S4 = !digitalRead(sensor4);//S4 = !digitalRead(sensor4);
  
  /*Serial.print("S1 = ");
  Serial.println(S1);
  Serial.print("S2 = ");
  Serial.println(S2);
  Serial.print("S3 = ");
  Serial.println(S3);
  Serial.print("S4 = ");
  Serial.println(S4);*/


  delay(1000);
   
  
}
