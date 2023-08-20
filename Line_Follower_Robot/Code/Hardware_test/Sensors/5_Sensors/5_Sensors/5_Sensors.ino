int sensor1 = 2;
int sensor2 = 3;
int sensor3 = 4;
int sensor4 = 5;   
int sensor5 = 13;// Right most sensor

int S1;
int S2;
int S3;
int S4;
int S5;

int error = 0;

void setup() {
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  Serial.begin(9600);                     
  delay(500);
}

void loop() {
  read_sensor_values();
  Serial.println(error);
  


  delay(1500);
   
  
}

void read_sensor_values()
{
  S1 = !digitalRead(sensor1);
  S2 = !digitalRead(sensor2);
  S3 = !digitalRead(sensor5);
  S4 = !digitalRead(sensor3);
  S5 = !digitalRead(sensor4);


  if ((S1 == 1) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 0)) error = 4;
  else if ((S1 == 1) && (S2 == 1) && (S3 == 0) && (S4 == 0) && (S5 == 0)) error = 3;
  else if ((S1 == 0) && (S2 == 1) && (S3 == 0) && (S4 == 0) && (S5 == 0)) error = 2;
  else if ((S1 == 0) && (S2 == 1) && (S3 == 1) && (S4 == 0) && (S5 == 0)) error = 1;
  else if ((S1 == 0) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 0)) error = 0;
  else if ((S1 == 0) && (S2 == 0) && (S3 == 1) && (S4 == 1) && (S5 == 0)) error = -1;
  else if ((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 1) && (S5 == 0)) error = -2;
  else if ((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 1) && (S5 == 1)) error = -3;
  else if ((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 1)) error = -4;

  else if ((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 0)) error = 100;
  else if ((S1 == 0) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 1)) error = 101;
  else if ((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 0)) error = 102;
  else if ((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 1)) error = 103;
} 
