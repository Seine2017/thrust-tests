/*
/   Author: Dominic Heaton - dhh1g15@soton.ac.uk
/   Explanation: This code is designed to allow a host PC to set the 
/     throttle for all four motors at one time by outputting a certain
/     PWM signal pulse width between 1ms and 2ms. This code is designed
/     to allow the operator to establish the duty cycle at which their 
/     quadcopter will leave the ground - the threshold throttle
/   How to use: Enter a value between 0 and 180 to set the level of the
/     PWM signal, where 0 is minimum (1ms) and 180 is the maximum (2ms)
*/
#include <Servo.h> 
 
//4 ESC objects to be controlled
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;
 
//PWM pins connectef to the ESCs
int escPin1 = 6;
int escPin2 = 9;
int escPin3 = 10;
int escPin4 = 11;

//Min and max pulse rates
int minPulseRate        = 1000; //1000us = 1ms
int maxPulseRate        = 2000; //2000us = 2ms
int throttleChangeDelay = 50;
 
void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(500);

  initEscs();
}

void loop() 
{
  // Wait for some input
  if (Serial.available() > 0) 
  {
    
    // Read the new throttle value
    int throttle = normalizeThrottle(Serial.parseInt());
    
    // Print it out
    Serial.print("Setting throttle to: ");
    Serial.println(throttle);
    
    // Change throttle to the new value
    changeThrottle(throttle);
  }
 
}
 
//Change throttle value
void changeThrottle(int throttle)
{
  int currentThrottle = readThrottle();
  
  int step = 1;
  if(throttle < currentThrottle) 
  {
    step = -1;
  }
  
  //Incremental changes to ramp throttle
  while(currentThrottle != throttle) 
  {
    writeTo4Escs(currentThrottle + step);
    
    currentThrottle = readThrottle();
    delay(throttleChangeDelay);
  }
}

//Checking throttle levels
int readThrottle() 
{
  int throttle = esc1.read();
  
  Serial.print("Current throttle is: ");
  Serial.print(throttle);
  Serial.println();
  
  return throttle;
}

//Change velocity of the 4 escs at the same time
void writeTo4Escs(int throttle) 
{
  //int normalisedThrottle = map(throttle, 0, 100, 0, 180);
  esc1.write(throttle);
  esc2.write(throttle);
  esc3.write(throttle);
  esc4.write(throttle);
}

//Initialisation of the ESCs
void initEscs() 
{
  esc1.attach(escPin1, minPulseRate, maxPulseRate);
  esc2.attach(escPin2, minPulseRate, maxPulseRate);
  esc3.attach(escPin3, minPulseRate, maxPulseRate);
  esc4.attach(escPin4, minPulseRate, maxPulseRate);
  

  writeTo4Escs(0);
  Serial.println("ESCs Initialised. Throttle Set to 0.");
  Serial.println("Value of 28 corresponds to 5.7% duty cycle (Motor Switch On Point)");
  Serial.println("Max value is 180");
}

//Start the motors
void startUpMotors() 
{
  writeTo4Escs(50);
}
 
// Ensure the throttle value is between 0 - 180
int normalizeThrottle(int value) 
{
  if(value < 0) {
    return 0;
    
  } else if(value > 180) {
    return 180;
  }
  
  return value;
}
