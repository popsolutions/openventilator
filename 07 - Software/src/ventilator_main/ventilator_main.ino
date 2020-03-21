/*
  This is a software developed by Juan Luis Méndez for the project Open Ventilator, please follow the license GNU GPLv3
  Also notice that doc and development is going on and we need help
  
Connection to the IBT-2 board:

IBT-2 pin 1 (RPWM) to Arduino pin 5(PWM)
IBT-2 pin 2 (LPWM) to Arduino pin 6(PWM)
IBT-2 pins 3 (R_EN), 4 (L_EN), 7 (VCC) to Arduino 5V pin
IBT-2 pin 8 (GND) to Arduino GND
IBT-2 pins 5 (R_IS) and 6 (L_IS) not connected
*/
  
int SENSOR_RPM = A0; // center pin of the potentiometer, control expiratory frequency
int SENSOR_Vol = A1; // center pin of the potentiometer, expiratory volume control
int SENSOR_Re = A2; //  center pin of the potentiometer, Relationship between expiratory and ispiratory time
const int inputPin =12;//180º step point
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
float Re ; // Expiration time variable 
float RPm;//  Variable respiratory rate
float Vol;// Variable injected volume 

void setup() {
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
  pinMode(inputPin, INPUT);// declares entry logiga passage point 180º
}
  
void loop() {

  
  int sensorValue_RPM = analogRead(SENSOR_RPM);// sensor value is in the range 0 to 1023
  int sensorValue_Vol = analogRead(SENSOR_Vol);// sensor value is in the range 0 to 1023
  int sensorValue_Re = analogRead(SENSOR_Re);// sensor value is in the range 0 to 1023
  int val = digitalRead(inputPin); // reading of pin 12 passage through 180º 

//if (val == LOW) 

RPm = map (sensorValue_RPM,1,1023,5,30);// variables to be displayed 
Vol = map (sensorValue_Vol,1,1023,50,900);// variables to be displayed
Re = map (sensorValue_Re,1,1023,5,-5);//variables to be displayed.. variable that is also used to calculate the inspiratory / expiration ratio 
 
if (val == LOW) // establishes expiration portion when it passes 180º
      
  {
    // forward rotation
   int forward_RPM = (sensorValue_RPM/8 +(Re * 5)+43); 
    analogWrite(LPWM_Output, forward_RPM);
    analogWrite(RPWM_Output, 0);

  } else {
    // forward rotation
    int forward_RPM = (sensorValue_RPM/8 - (Re * 5)+43);
    analogWrite(LPWM_Output, forward_RPM);
    analogWrite(RPWM_Output, 0);
  }
}
