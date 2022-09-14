#include "html510.h"    
#include "Manual_Control.h"     // contains string "body" html code
HTML510Server h(80);

//NewPing and VL53L0X Libraries //

#include <NewPing.h>   // ultrasonic
#include "Adafruit_VL53L0X.h" // TOF

#define PIN_IR_SENSOR 37

#define TRIGGER_PIN_1 14
#define ECHO_PIN_1 34
#define TRIGGER_PIN_2 14
#define ECHO_PIN_2 38
#define MAX_DISTANCE 100

// one channel for each motor (PWM)
#define LEDC_CHANNEL1 0 // use first of 16  
#define LEDC_CHANNEL2 1 
#define LEDC_CHANNEL3 2 
#define LEDC_CHANNEL4 3 

#define LEDC_RESOLUTION_BITS 20 // up to 1M
#define LEDC_FREQ_HZ 20         // chosen frequency not stalling at high duty cycles
#define LEDC_RESOLUTION ((1<<LEDC_RESOLUTION_BITS)-1) 
// Motor 1
#define SPEED_PIN1 23
#define DIR_PIN1 4
// Motor 2
#define SPEED_PIN2 32
#define DIR_PIN2 9
// Motor 3
#define SPEED_PIN3 33
#define DIR_PIN3 13
// Motor 4
#define SPEED_PIN4 19
#define DIR_PIN4 10

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// global variables declaration
int initialtime, duration;
static int previoustime;
const char* ssid = "ESP32 Sof";
int duty;
int cal1 = 19;
int cal2 = 0;
int cal3 = 9;
int cal4 = 3;
int mode = 0;

void IRAM_ATTR handleSensorInterrupt() {  // IR interrupt
   for (int i=0; i<10; i++) {
      initialtime = micros();
   }
    if (previoustime != initialtime) {
        if (initialtime > previoustime) {           // calculate duration between two rises (period)
            duration = initialtime - previoustime; 
        }  
        if (initialtime < previoustime) {
            duration = 65536 + initialtime - previoustime;
        }
        previoustime = initialtime;
  }
}

void moveforward() {
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,HIGH); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255); 
}

void rotateCW() {
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255); 
}

void rotateCCW() {
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(DIR_PIN4,HIGH); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255); 
}


/*****************/
/* web handler   */
void handleRoot() {
  h.sendhtml(body);
}

// function triggered when we change the duty cycle
void handleSliderDuty(){
  String s2 = "Speed: ";
  duty = h.getVal();
  s2 = s2 + duty;
  h.sendplain(s2); 
}

// functions triggered when we press one of the buttons
void handleHit1(){            // forward left
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void handleHit2(){      // forward
  moveforward();
  h.sendplain(""); // acknowledge     
}

void handleHit3(){      // forward right
    digitalWrite(DIR_PIN1,HIGH); 
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255);  
  h.sendplain(""); // acknowledge     
}

void handleHit4(){      // left
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,LOW); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255);  
  h.sendplain(""); // acknowledge    
}

void handleHit5(){        // stop
    mode = 1;   // bypassing the wall following code
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void handleHit6(){      // right
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255);
  h.sendplain(""); // acknowledge     
}

void handleHit7(){      // backward left
    digitalWrite(DIR_PIN1,LOW); 
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,HIGH);
    digitalWrite(DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);  

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255); 
  h.sendplain(""); // acknowledge     
}
 
void handleHit8(){      // backwards
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(DIR_PIN2,LOW);
    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(DIR_PIN4,LOW);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty + cal1, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty + cal4, 255) * 2.55 / 255);
  h.sendplain(""); // acknowledge     
}

void handleHit9(){      // backward right
    digitalWrite(DIR_PIN1,HIGH);
    digitalWrite(DIR_PIN2,LOW); 
    digitalWrite(DIR_PIN3,LOW); 
    digitalWrite(DIR_PIN4,HIGH);

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty + cal2, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty + cal3, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);  
  h.sendplain(""); // acknowledge     
}

void handleHit10(){      // clockwise
    rotateCW(); 
    h.sendplain(""); // acknowledge     
}

void handleHit11(){      // counter clockwise
     rotateCCW();
    h.sendplain(""); // acknowledge     
}

void handleHit12(){      // manual mode
    digitalWrite(DIR_PIN1,LOW);
    digitalWrite(DIR_PIN2,HIGH);
    digitalWrite(DIR_PIN3,LOW);
    digitalWrite(DIR_PIN4,HIGH); 

    // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255); 

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);  
  h.sendplain(""); // acknowledge     
}

void handleHit13(){      // wall following
  mode = 2;
}

void handleHit14(){      // IR sensor
   mode = 3; 
}

void setup() {  
  Serial.begin(115200);

  // TIME OF FLIGHT
  /* from: https://www.esp32.com/viewtopic.php?t=12715
   *  The default I2C pins if you choose pico32 as the board in the platformio is
   *  IO22 - SCL (C)
   *  IO21 - SDA (D) */
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 

  // ports configurations
  pinMode(PIN_IR_SENSOR, INPUT);
  // interrupt for ir sensor
  attachInterrupt(digitalPinToInterrupt(PIN_IR_SENSOR), handleSensorInterrupt, RISING);
  
  // Attach LEDC channels to motor speed pins
  ledcAttachPin(SPEED_PIN1, LEDC_CHANNEL1);
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN2, LEDC_CHANNEL2);
  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN3, LEDC_CHANNEL3);
  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);

  ledcAttachPin(SPEED_PIN4, LEDC_CHANNEL4);
  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS); 
  
  // make direction pins output pins
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
  pinMode(DIR_PIN3, OUTPUT);
  pinMode(DIR_PIN4, OUTPUT); 
  
  // wifi configurations
  WiFi.softAP(ssid);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 136),  IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0)); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");  Serial.println(myIP);      
             
  h.begin();
  h.attachHandler("/ ",handleRoot);
  h.attachHandler("/slider?val2=",handleSliderDuty);
  h.attachHandler("/hit1",handleHit1);
  h.attachHandler("/hit2",handleHit2);
  h.attachHandler("/hit3",handleHit3);
  h.attachHandler("/hit4",handleHit4);
  h.attachHandler("/hit5",handleHit5);
  h.attachHandler("/hit6",handleHit6);
  h.attachHandler("/hit7",handleHit7);
  h.attachHandler("/hit8",handleHit8);
  h.attachHandler("/hit9",handleHit9);
  h.attachHandler("/hit10",handleHit10);
  h.attachHandler("/hit11",handleHit11);
  h.attachHandler("/hit12",handleHit12);
  h.attachHandler("/hit13",handleHit13);
  h.attachHandler("/hit14",handleHit14);
}

void loop(){
   h.serve();
  
  // ULTRASONICS
  unsigned int distance1 = sonar1.ping_cm();
  unsigned int distance2 = sonar2.ping_cm();  // NOT reliable
  
  /*Serial.print("Ultrasonic distance 1: ");
  Serial.println(distance1);
  Serial.print("Ultrasonic distance 2: ");
  Serial.println(distance2); */

  // TIME OF FLIGHT
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  /*if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("TOF distance: "); Serial.println(measure.RangeMilliMeter/10);
  } else {
    Serial.println(" out of range ");
  } */
  if (mode == 2) {
    // Wall following code
    if ((measure.RangeMilliMeter/10) > 20) {    // if wall is detected within 20cm
        if (distance1 > 20) {
          rotateCW();
          //Serial.println("rotate CW");
        } 
        else if ((distance1 < 5) || (distance2 > 20)) {
          rotateCCW();
          //Serial.println("rotate CCW");
        }
        else {
          moveforward();
          //Serial.println("forward");
        }
        delay(100);
    }
    else {
      rotateCCW();
      delay(1000);
    } 
  } 

  else if (mode == 3) {
    // IR sensor code
    //Serial.print("Duration: ");  Serial.println(duration);
 
    // counts = system frequency/(prescaler*desired frequency) 
    if ((duration < 1500) || (duration > 42000)) { // 23 Hz (43000 - 44000) OR 690-710 Hz (1400 - 1450)
      moveforward();
    }
    else {
       rotateCW();
    }
    delay(80);
  }
}
