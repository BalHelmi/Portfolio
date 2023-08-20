// IR Sensors
int sensor1 = 2;      // Left most sensor
int sensor2 = 3;
int sensor3 = 4;
int sensor4 = 5;
int sensor5 = 13;// Right most sensor


int S1;
int S2;
int S3;
int S4;
int S5;

// Motor Variables
int ENA = 6;
int motorInput1 = 8;
int motorInput2 = 7;
int motorInput3 = 9;
int motorInput4 = 10;
int ENB = 11;

//Initial Speed of Motor
unsigned int initial_motor_speed = 100;

// PID Constants
float Kp = 15;
float Ki = 0;
float Kd = 5;

float error = 0;

float P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;

int flag1 = 0;

void setup()
{
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);

  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
}
void loop()
{
  ligne_noir();
}


//********************************    LIGNE NOIR    ************************************************


void ligne_noir() {
  read_sensor_values_ligne_noir();
  if (error == 100) {

    while ( error != 0) {
      read_sensor_values_ligne_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
    }
    /*do {
      read_sensor_values_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
      } while (error == 0);*/
  }

  else if (error == 101) {
    while (error != 0) {
      read_sensor_values_ligne_noir();
      if (error == 100 || error == 103) break;
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpRightTurn();
    }
    /*do {
      read_sensor_values_noir();
      if (error == 100 || error == 103) break;
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpRightTurn();
      } while (error == 0);*/
  }

  else if (error == 102) {
    flag1 ++;

    while (error == 102) {
      read_sensor_values_ligne_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      forward();
      delay(200);
    }
    /*do {
      read_sensor_values_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      forward();
      delay(200);
      } while (error == 102);*/
  }

  else if (error == 103) {
    while (error != 0) {
      read_sensor_values_ligne_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
    }
    /*do {
      read_sensor_values_noir();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
      } while (error == 0);*/
  }

  else {
    calculate_pid();
    motor_control();
  }
}



void read_sensor_values_ligne_noir()
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

  else error = 0;


}

//*************************  FIN LIGNE NOIR *********************************


///////////////////////////////////////////////////////////////////////////////


//************************   LIGNE BLANCHE ***********************************

void ligne_blanche() {
  read_sensor_values_ligne_blanche();
  if (error == 100) {
    while (error != 0) {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
    }
    /*do {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
      } while (error == 0);*/
  }

  else if (error == 101) {
    while (error == 0) {
      read_sensor_values_ligne_blanche();
      if (error == 100 || error == 103) break;
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpRightTurn();
    }
    /*do {
      read_sensor_values_ligne_blanche();
      if (error == 100 || error == 103) break;
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpRightTurn();
      } while (error == 0);*/
  }

  else if (error == 102) {
    flag1 ++;
    while (error == 102) {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      forward();
      delay(200);
    }
    /*do {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      forward();
      delay(200);
      } while (error == 102);*/
  }

  else if (error == 103) {
    while (error != 0) {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
    }
    /*do {
      read_sensor_values_ligne_blanche();
      analogWrite(ENA, initial_motor_speed );
      analogWrite(ENB, 0.8 * initial_motor_speed);
      sharpLeftTurn();
    } while (error == 0);*/
  }

  else {
    calculate_pid();
    motor_control();
  }

}

void read_sensor_values_ligne_blanche() {
  S1 = digitalRead(sensor1);
  S2 = digitalRead(sensor2);
  S3 = digitalRead(sensor5);
  S4 = digitalRead(sensor3);
  S5 = digitalRead(sensor4);

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
  else error = 0;

}

//********************************    FIN LIGNE BLANCHE   ********************************



void calculate_pid()
{
  P = error;
  I = I + previous_error;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);

  previous_error = error;
}

void motor_control()
{
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = 0.8 * initial_motor_speed + PID_value;

  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  analogWrite(ENA, left_motor_speed);
  analogWrite(ENB, right_motor_speed);

  forward();
}

void forward()
{
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void reverse()
{
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void right()
{
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
void left()
{
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void sharpRightTurn() {
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void sharpLeftTurn() {
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void stop_bot()
{
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}
