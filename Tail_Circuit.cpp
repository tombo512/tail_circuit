/*

*******************
  Tail_Circuit
  1.1 beta

  2018/5/5
  R.Hirota

  assign pin D9 to rudder PWM
  assign pin D10 to elevator PWM
  assign pin D2 to switch right
  assign pin D3 to switch left
  assign pin A0 to slide volume
*******************

*/

#include <Servo.h>
Servo 
  rudder, 
  elevator;

// Pin assighn
int Rudder_Output_Pin = 9;
int Elevator_Output_Pin = 10;
int Switch_Input_right_Pin = 2;
int Switch_Input_left_Pin = 3;
int Analog_Input_Pin = 0;

// Values
bool val_right = 0;
bool val_left = 0;
int val_elv = 0;
int get_analog = 0;
int n_steps = 5;
int bit_shift = 3;

// Paramaters
int rudder_angle = 90;
int elevator_angle = 10;

int MAX_RDR_ANDLE = 20;
int MAX_ELV_ANGLE = 30;

void setup() {
  Serial.begin(9600);
  rudder.attach(Rudder_Output_Pin, 750, 2250);
  elevator.attach(Elevator_Output_Pin, 750, 2250);
  pinMode(Switch_Input_right_Pin, INPUT);
  pinMode(Switch_Input_left_Pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
}

int check(bool val_right, bool val_left){
  bool check_mlt = val_right*val_left;
  return check_mlt;
}

void loop() {
  while(1){
    /*
    digitalWrite(4, HIGH);
    delay(500);
    digitalWrite(4, LOW);
    delay(500);
    */
    val_right = digitalRead(Switch_Input_right_Pin);
    val_left = digitalRead(Switch_Input_left_Pin);
    if(!check(val_right, val_left)){
      rudder_angle = 
        90 - MAX_RDR_ANDLE*val_left + MAX_RDR_ANDLE*val_right;
      rudder.write(rudder_angle);
      delay(50);
    }else{
      rudder.write(rudder_angle); // keep
    }

   val_elv = 0;

   for(int i = 0; i < 5; i++){
     get_analog = analogRead(Analog_Input_Pin);
     get_analog >>= bit_shift;
     val_elv += get_analog;
     delay(10);
   }
   val_elv = val_elv / 5;
   Serial.println(val_elv, DEC);
   elevator_angle = 10 + val_elv;
   elevator.write(elevator_angle);
  }
}