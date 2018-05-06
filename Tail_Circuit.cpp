/*

*******************
  Tail_Circuit
  1.0 beta

  2018/5/5
  R.Hirota

  Arduino Nano

  < Pin assign >
  D9 to rudder servo
  D10 to elevator servo
  D2 to switch right
  D3 to switch left
  A0 to slide volume
*******************

*/

#include <VarSpeedServo.h>
VarSpeedServo 
  rudder, 
  elevator;

// Pin assighn
int Rudder_Output_Pin = 9,
    Elevator_Output_Pin = 10,
    Switch_Input_right_Pin = 2,
    Switch_Input_left_Pin = 3,
    Analog_Input_Pin = 0;

// Values
bool val_right = 0,
      val_left = 0;
int val_elv = 0,
    get_analog = 0,
    n_steps = 5,
    bit_shift = 3;

// Paramaters
int MAX_RDR_ANDLE = 20,
    MAX_ELV_ANGLE = 30,
    CENTER_RDR_ANGLE = 90,
    CENTER_ELV_ANGLE = 90,
    SERVO_MIN_PULSE = 750,
    SERVO_MAX_PULSE = 2250,
    SERVO_ANGULAR_VEL = 150;

int rudder_angle = CENTER_RDR_ANGLE,
    elevator_angle = CENTER_ELV_ANGLE;

void setup() {
  Serial.begin(9600);

  rudder.attach(Rudder_Output_Pin, SERVO_MIN_PULSE,
                SERVO_MAX_PULSE);
  elevator.attach(Elevator_Output_Pin, SERVO_MIN_PULSE, 
                  SERVO_MAX_PULSE);

  pinMode(Switch_Input_right_Pin, INPUT);
  pinMode(Switch_Input_left_Pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

int and_check(bool val_right, bool val_left){
  bool check_mlt = val_right*val_left;
  return check_mlt;
}

void loop() {
  while(1){
    val_right = digitalRead(Switch_Input_right_Pin);
    val_left = digitalRead(Switch_Input_left_Pin);

    if(!and_check(val_right, val_left)){  // XOR
      rudder_angle = 
        CENTER_RDR_ANGLE - MAX_RDR_ANDLE*val_left + MAX_RDR_ANDLE*val_right;
      rudder.write(rudder_angle, SERVO_ANGULAR_VEL, false);
      delay(10);
    }else{
      rudder.write(rudder_angle, SERVO_ANGULAR_VELE, false);
    }

    val_elv = 0;  // reset

    for(int i = 0; i < n_steps; i++){
      get_analog = analogRead(Analog_Input_Pin);
      get_analog >>= bit_shift;
      val_elv += get_analog;
      delay(10);
    }

    val_elv = val_elv / n_steps;  // avarage
    val_elv = val_elv - ((1024 >>= bit_shift)*0.5);
    val_elv = val_elv / ((1024 >>= bit_shift)*0.5); // normarization

    Serial.println(val_elv, DEC);
    elevator_angle = CENTER_ELV_ANGLE + val_elv*MAX_ELV_ANGLE;
    elevator.write(elevator_angle, SERVO_ANGULAR_VEL, false);
  }
}
