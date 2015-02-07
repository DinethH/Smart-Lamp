/**

  Smart Lamp
  
**/


/* How many milliseconds the nightlight should stay on for (1 minutes would be 1 x 60 x 1000ms = 60000 */
int nightLightTimeOut = 60000; 

#define RED 9
#define GREEN 11
#define BLUE 10
#define nightLight 13
#define trigger 2

unsigned long currentMillis = 0; // stores current time value 
unsigned long previousMillis = 0; // the value of the previous days millis(), so we can reset each day
unsigned long currentMinutes = 0; // just easier to work with
unsigned int minutesUntilSunrise; 
unsigned long nightLightTimeOff = 0; // A single number to store the time when we should deactive the current night light cycle
bool nightLightOn = false; // enable re-activation even when already activated

int LDR = 0;
int LDRValue = 0;
int Light_Sensitivity = 500;

int led = 13;                // the pin that the LED is atteched to
int sensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)


void setup() {
  pinMode(nightLight,OUTPUT);
  pinMode(trigger,INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);  
  
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  
  analogWrite(RED,0);
  analogWrite(GREEN,0);
  analogWrite(BLUE,0);
  
  // minutes until begin sunrise
  //minutesUntilSunrise = hoursUntilSunrise * 60;
  minutesUntilSunrise = 1;
  Serial.print("Minutes until sunrise:");
  Serial.println(minutesUntilSunrise);
}

void loop(){
  
  clock();
  
  sunrisealarm();
  
  motionTriggered();
 
  delay(100); 
    
}

void clock(){
  if(millis() >= previousMillis+86400000){
    // a full day has elapsed, reset the clock;
    previousMillis +=86400000;
  }
  currentMillis = millis() - previousMillis; // this keeps our currentMillis the same each day
  currentMinutes = (currentMillis/1000)/60;
  Serial.println(currentMinutes);
}

void sunrisealarm(){
   //each second during the 30 minite period should increase the colour value by:
  float increment = (float) 255/(1*60);
 
  if(currentMinutes >= minutesUntilSunrise){
     //sunrise begins! 
     float currentVal = (float)((currentMillis/1000) - (minutesUntilSunrise*60)) * increment;
     Serial.print("Current value for sunrise:");
     Serial.println(currentVal);
     //during ramp up, write the current value of minutes X brightness increment
     if(currentVal < 255){

         analogWrite(RED,currentVal);
         analogWrite(GREEN,currentVal);
         analogWrite(BLUE,currentVal);

     }
     else if(currentMinutes - minutesUntilSunrise < 4){

         analogWrite(RED,255);
         analogWrite(GREEN,255);
         analogWrite(BLUE,255);

     }
     else{
        analogWrite(RED,0);
        analogWrite(GREEN,0);
        analogWrite(BLUE,0);
      }
  }
  
  
  
  
}


void nightLamp () {
  LDRValue = analogRead(LDR);
  Serial.println(LDRValue);
  if(currentMinutes < minutesUntilSunrise){
    if (LDRValue > Light_Sensitivity) {

        analogWrite(BLUE,1);

      
    } else {
      analogWrite(BLUE,0);
    } 
  } else {
      //analogWrite(BLUE,0);
  }
}



void motionTriggered(){
  val = digitalRead(sensor);   // read sensor value
  if(currentMinutes < minutesUntilSunrise){
    if (val == HIGH) {           // check if the sensor is HIGH
      if (analogRead(LDR) > Light_Sensitivity) {
        analogWrite(RED,255);
        analogWrite(GREEN,255);
        analogWrite(BLUE,0);
      }
      
      if (state == LOW) {
        Serial.println("Motion detected!"); 
        state = HIGH;       // update variable state to HIGH
      }
    } 
    else {
        analogWrite(RED,0);
        analogWrite(GREEN,0);
        analogWrite(BLUE,0);
        nightLamp ();
        if (state == HIGH){
          Serial.println("Motion stopped!");
          state = LOW;       // update variable state to LOW
          
      }
    }
  }
}
