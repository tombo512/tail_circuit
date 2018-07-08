/*

*******************
  Tail_Circuit
  1.2 beta

  2018/7/8
  R.Hirota

  pin D9  : rudder PWM
  pin D10 : elevator PWM 
  pin D2  : switch right
  pin D3  : switch left
  pin D4  : LED red
  pin D5  : LED yellow
  pin D6  : LED green
  pin A0  : slide volume
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

int LED_PIN_1 = 6; //green
int LED_PIN_2 = 5; //yellow
int LED_PIN_3 = 4; //red

int time_counter = 0;
bool val_right = 0;
bool val_left = 0;
float val_elv = 0;
int val_elv_sum = 0;
int get_analog = 0;
int n_steps = 5;
int bit_shift = 3;
int rudder_angle = 90;
int rudder_angle_former = 90;
int elevator_angle = 90;

int MAX_RDR_ANGLE = 15;
int CENTER_RDR_ANGLE = 90;
int MAX_ELV_ANGLE = 13;
int CENTER_ELV_ANGLE = 90;

void setup() {
  Serial.begin(9600);
  rudder.attach(Rudder_Output_Pin, 750, 2250);
  elevator.attach(Elevator_Output_Pin, 750, 2250);
  pinMode(Switch_Input_right_Pin, INPUT);
  pinMode(Switch_Input_left_Pin, INPUT);
  pinMode(LED_PIN_1, OUTPUT); 
  pinMode(LED_PIN_2, OUTPUT); 
  pinMode(LED_PIN_3, OUTPUT); 
}

int check(bool val_1, bool val_2){
  bool check_mlt = val_1 * val_2;
  return check_mlt;
}

int iterWrite(Servo servo, int former_angle, int angle){
  int dev = angle - former_angle;
  int dev_sgn = (dev >> 31) | 1; //calculate sgn
  int dev_abs = - dev_sgn * dev; //calculate abs
  for(int i = 0; i < dev_abs + 1; i++){
    servo.write(former_angle + i * dev_sgn);
    delay(20);
  }
  return 0;
}

void loop(){
  while(1){
    val_right = digitalRead(Switch_Input_right_Pin);
    val_left = digitalRead(Switch_Input_left_Pin);
    
    if(!check(val_right, val_left)){
      rudder_angle = 
        CENTER_RDR_ANGLE - MAX_RDR_ANGLE * val_left + MAX_RDR_ANGLE * val_right;
      digitalWrite(LED_PIN_1, val_right);
      digitalWrite(LED_PIN_3, val_left);
      if(rudder_angle == rudder_angle_former){
        rudder.write(rudder_angle); //keep
      }else{
        iterWrite(rudder, rudder_angle_former, rudder_angle);
      }
      delay(50);
    }else{
      rudder.write(rudder_angle); // keep
    }

   val_elv_sum = 0;
   
   for(int i = 0; i < 5; i++){
     get_analog = analogRead(Analog_Input_Pin);
     get_analog >>= bit_shift;
     val_elv_sum += get_analog;
     delay(10);
    }

   val_elv = val_elv_sum / 5;
   
   elevator_angle = CENTER_ELV_ANGLE + (val_elv - 63.5) / 63.5 * MAX_ELV_ANGLE;
   elevator.write(elevator_angle);

   if(time_counter % (elevator_angle - 76) == 0){
     digitalWrite(LED_PIN_2, HIGH);
     delay(5);
    }else{
     digitalWrite(LED_PIN_2, LOW);
    }
    
    time_counter++;
    rudder_angle_former = rudder_angle;
  }
}