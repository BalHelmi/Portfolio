

//************************   MACROS   ********************


#define SENSOR_1 2
#define SENSOR_2 3
#define SENSOR_3 4
#define SENSOR_4 5
#define SENSOR_5 13
#define BLACK 1
#define WHITE 0

#define PWM_LeftMotor 6
#define INPUT1_LeftMotor 8
#define INPUT2_LeftMotor 7
#define INPUT3_RightMotor 9
#define INPUT4_RightMotor 10
#define PWM_RightMotor 11

//************************  VARIABLES   **************************
static uint8_t ROBOT_LeftSensorState_u8 = 0,
               ROBOT_LeftCenterSensorState_u8 = 0,
               ROBOT_CenterSensorState_u8 = 0,
               ROBOT_RightCenterSensorState_u8 = 0,
               ROBOT_RightSensorState_u8 = 0;
static float ROBOT_Error = 0.0;
static float PID_P  = 0 ,
             PID_D  = 0 ,
             PID_I  = 0 ,
             PID_Value  = 0 ,
             PID_Previous_Error  = 0 ,
             Ki = 15 ,
             Kp = 0 ,
             Kd = 5 ;

//****************  STATE_MACHINE   ********************




//***************   FUNCTIONS DECLARATION  *********************

void ROBOT_vReadSensors();

void ROBOT_vForward();
void ROBOT_vBackward();
void ROBOT_vLeft();
void ROBOT_vRight();
void ROBOT_vStop();

void ROBOT_vCalculatePID();
void ROBOT_vMotorControl();


//***************   SETUP   *****************************
void setup() {
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);
  pinMode(SENSOR_5, INPUT);

  pinMode(INPUT1_LeftMotor, OUTPUT);
  pinMode(INPUT2_LeftMotor, OUTPUT);
  pinMode(INPUT3_RightMotor, OUTPUT);
  pinMode(INPUT4_RightMotor, OUTPUT);

  pinMode(PWM_LeftMotor, OUTPUT);
  pinMode(PWM_RightMotor, OUTPUT);

  Serial.begin(9600);
  delay(500);
}

//******************    LOOP    *************************

void loop() {
  ROBOT_vReadSensors();
  Serial.println(ROBOT_Error);
  delay(1000);
}

//************************* FUNCTIONS DEFINITION    ***********************




void ROBOT_vReadSensors () {
  ROBOT_LeftSensorState_u8 = !digitalRead(SENSOR_1);
  ROBOT_LeftCenterSensorState_u8 = !digitalRead(SENSOR_2);
  ROBOT_CenterSensorState_u8 = !digitalRead(SENSOR_5);
  ROBOT_RightCenterSensorState_u8 = !digitalRead(SENSOR_3);
  ROBOT_RightSensorState_u8 = !digitalRead(SENSOR_4);


  if ((ROBOT_LeftSensorState_u8 == BLACK) &&
      (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
      (ROBOT_CenterSensorState_u8 == WHITE) &&
      (ROBOT_RightCenterSensorState_u8 == WHITE) &&
      (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 4;
  }
  else if ((ROBOT_LeftSensorState_u8 == BLACK) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == WHITE) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 3;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == WHITE) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 2;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == WHITE) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 1;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == WHITE))
    ROBOT_Error = 0;
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = -1;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = -2;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == BLACK))
           {
            ROBOT_Error = -3;
           }
    
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == WHITE) &&
           (ROBOT_RightSensorState_u8 == BLACK))
  {
    ROBOT_Error = -4;
  }
  else if ((ROBOT_LeftSensorState_u8 == BLACK) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 100;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == BLACK))
  {
    ROBOT_Error = 101;
  }
  else if ((ROBOT_LeftSensorState_u8 == BLACK) &&
           (ROBOT_LeftCenterSensorState_u8 == BLACK) &&
           (ROBOT_CenterSensorState_u8 == BLACK) &&
           (ROBOT_RightCenterSensorState_u8 == BLACK) &&
           (ROBOT_RightSensorState_u8 == BLACK))
  {
    ROBOT_Error = 103;
  }
  else if ((ROBOT_LeftSensorState_u8 == WHITE) &&
           (ROBOT_LeftCenterSensorState_u8 == WHITE) &&
           (ROBOT_CenterSensorState_u8 == WHITE) &&
           (ROBOT_RightCenterSensorState_u8 == WHITE) &&
           (ROBOT_RightSensorState_u8 == WHITE))
  {
    ROBOT_Error = 102;
  }

}

void ROBOT_vCalculatePID() {
  PID_P  = ROBOT_Error;
  PID_I   = PID_I  + ROBOT_Error;
  PID_D  = ROBOT_Error - PID_Previous_Error ;

  PID_Value  = (Kp * PID_P ) + (Ki * PID_I ) + (Kd * PID_D );

  PID_Previous_Error  = ROBOT_Error;
}

void ROBOT_vMotorControl() {
  float left_motor_speed = 70 - PID_Value ;
  float right_motor_speed = 50 - PID_Value ;

  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);

  analogWrite(PWM_LeftMotor, left_motor_speed );
  analogWrite(PWM_RightMotor, right_motor_speed );

  ROBOT_vForward();

}

void ROBOT_vForward()
{
  digitalWrite(INPUT1_LeftMotor, LOW);
  digitalWrite(INPUT2_LeftMotor, HIGH);
  digitalWrite(INPUT3_RightMotor, LOW);
  digitalWrite(INPUT4_RightMotor, HIGH);
}
void ROBOT_vBackward()
{
  digitalWrite(INPUT1_LeftMotor, HIGH);
  digitalWrite(INPUT2_LeftMotor, LOW);
  digitalWrite(INPUT3_RightMotor, HIGH);
  digitalWrite(INPUT4_RightMotor, LOW);
}

void ROBOT_vRight() {
  digitalWrite(INPUT1_LeftMotor, LOW);
  digitalWrite(INPUT2_LeftMotor, HIGH);
  digitalWrite(INPUT3_RightMotor, HIGH);
  digitalWrite(INPUT4_RightMotor, LOW);
}
void ROBOT_vLeft() {
  digitalWrite(INPUT1_LeftMotor, HIGH);
  digitalWrite(INPUT2_LeftMotor, LOW);
  digitalWrite(INPUT3_RightMotor, LOW);
  digitalWrite(INPUT4_RightMotor, HIGH);
}
void ROBOT_vStop()
{
  digitalWrite(INPUT1_LeftMotor, HIGH);
  digitalWrite(INPUT2_LeftMotor, HIGH);
  digitalWrite(INPUT3_RightMotor, HIGH);
  digitalWrite(INPUT4_RightMotor, HIGH);
}
