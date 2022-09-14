/*
 * receives packets  (plus any other broadcasters)
 * 
 */
#include <WiFi.h>
#include <WiFiUdp.h>
#include "vive510.h"
#include "html510.h"    
#include "Coordinates.h"
HTML510Server h(80);

#define SWITCH_PIN_1 21   //dip switch pin 1
#define SWITCH_PIN_2 22   //dip switch pin 2
#define SIGNALPIN1 26

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

int duty = 40;
int V1_x = 0, V1_y = 0;

Vive510 vive1(SIGNALPIN1);

const char* ssid     = "TP-Link_E0C8";
const char* password = "52665134";
int x_goal, y_goal;
int x = 0;
int y = 0;

WiFiUDP canUDPServer;
WiFiUDP UDPTestServer;

IPAddress myIPaddress(192, 168, 0, 136); // change to your IP
unsigned int UDPPort = 2510; // port for cans is 1510, port for robots is 2510
IPAddress ipTarget(192, 168, 1, 255); // 255 is a broadcast address to everyone at 192.168.1.xxx

/*****************/
/* web handler   */
void handleRoot() {
  h.sendhtml(body);
}

// function triggered when we change the duty cycle

void handleSliderDuty1(){
  String s1 = "X goal: ";
  x_goal = h.getVal();
  s1 = s1+ x_goal;
  h.sendplain(s1); 
}

void handleSliderDuty2(){
  String s2 = "Y goal: ";
  y_goal = h.getVal();
  s2 = s2+ y_goal;
  h.sendplain(s2); 
}

// functions triggered when we press one of the buttons
void handleHit1(){            // forward left
    x = x_goal;
    y = y_goal;
    h.sendplain(""); // acknowledge     
}


void stoprob() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);
}

void moveforward() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void movebackward () {
  digitalWrite(DIR_PIN1, LOW);

  digitalWrite(DIR_PIN2, LOW);

  digitalWrite(DIR_PIN3, LOW);

  digitalWrite(DIR_PIN4, LOW);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void moveleft() {
  digitalWrite(DIR_PIN1, LOW);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, LOW);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void moveright() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, LOW);

  digitalWrite(DIR_PIN3, LOW);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void moveforwardleft() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);
}

void moveforwardright() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void movebackwardleft() {
  digitalWrite(DIR_PIN1, LOW);

  digitalWrite(DIR_PIN2, HIGH);

  digitalWrite(DIR_PIN3, HIGH);

  digitalWrite(DIR_PIN4, LOW);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, 0);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, 0);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);
}

void movebackwardright() {
  digitalWrite(DIR_PIN1, HIGH);

  digitalWrite(DIR_PIN2, LOW);

  digitalWrite(DIR_PIN3, LOW);

  digitalWrite(DIR_PIN4, HIGH);

  // write duty to motors
  ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL1, 0);

  ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL2, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL3, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL3, LEDC_RESOLUTION * min(duty, 255) * 2.55 / 255);

  ledcSetup(LEDC_CHANNEL4, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcWrite(LEDC_CHANNEL4, 0);
}

void fncUdpSend(char *datastr, int len)
{
  UDPTestServer.beginPacket(ipTarget, UDPPort);
  UDPTestServer.write((uint8_t *)datastr, len);
  UDPTestServer.endPacket();
}

void handleXY() {
    char s[13];
    if (vive1.status() == VIVE_LOCKEDON) {
      //if (digitalRead(SWITCH_PIN_1) == HIGH && digitalRead(SWITCH_PIN_2) == HIGH) {
        V1_x = vive1.xCoord();
        V1_y = vive1.yCoord();    
        
        if ((abs(V1_y - y) < 100) && ((x - V1_x) > 50)) {
          moveforward();
        }
        else if ((abs(V1_y - y) < 100) && ((V1_x - x) > 50)) {
          movebackward();
        }
        else if ((abs(V1_x - x) < 100) && ((y - V1_y) > 50)) {
          moveright();
        }
        else if ((abs(V1_x - x) < 100) && ((V1_y - y) > 50)) {
          moveleft();
        }
        else if ((((x - V1_x) > 50) && ((y - V1_y) > 50))) {
         moveforwardright();
        }
        else if (((V1_x - x) > 50) && ((V1_y - y) > 50)) {
          movebackwardleft();
        }
        else if (((V1_x - x) > 50) && ((y - V1_y) > 50)) {
         movebackwardright();
        }
        else if (((x - V1_x) > 50) && ((V1_y - y) > 50)) {
          moveforwardleft();
        }
        else {
          stoprob();
        }
        
        sprintf(s, "%1d:%4d,%4d", 1, V1_x, V1_y);
        fncUdpSend(s, 13);
        Serial.printf("sending data: %s \n",s);
        delay(10);
      //}
    }
    else {
      vive1.sync(15);
    }
  }

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");  Serial.println(ssid);

  // wifi configurations
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192, 168, 0, 136),
              IPAddress(192, 168, 0, 1),
              IPAddress(255, 255, 255, 0));
              
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500);
    Serial.print("."); 
  }
  Serial.println("WiFi connected"); 
  
  h.begin();
  h.attachHandler("/ ",handleRoot);
  h.attachHandler("/slider?val1=",handleSliderDuty1);
  h.attachHandler("/slider?val2=",handleSliderDuty2);
  h.attachHandler("/hit1",handleHit1);
  

  // Sender
  vive1.begin();
  Serial.println("Vive trackers started");
  
  //pinMode(SWITCH_PIN_1,INPUT_PULLUP);
  //pinMode(SWITCH_PIN_2,INPUT_PULLUP);
  Serial.print("Connecting to ");  Serial.println(ssid);
 
  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  UDPTestServer.begin(UDPPort); // strange bug needs to come after WiFi.begin but before connect

  // Receiver 
  canUDPServer.begin(1510); // can port 1510

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 

   //pwm channel setup
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
  stoprob();
}

void loop() {
    handleXY();
    h.serve();
    //Serial.println(x);
    //Serial.println(y);
    /*if(digitalRead(SWITCH_PIN_1) == HIGH && digitalRead(SWITCH_PIN_2) == LOW){
      x = vive1.xCoord();
      y = vive1.yCoord();
      sprintf(s,"%1d:%4d,%4d",1, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s \n",s);
      delay(100); 
    }
    else if(digitalRead(SWITCH_PIN_1) == LOW && digitalRead(SWITCH_PIN_2) == HIGH){
      sprintf(s,"%1d:%4d,%4d",2, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s",s);
      delay(100); 
    }
    if(digitalRead(SWITCH_PIN_1) == HIGH && digitalRead(SWITCH_PIN_2) == HIGH){
      sprintf(s,"%1d:%4d,%4d",3, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s",s);
      delay(100); 
    }
    else if(digitalRead(SWITCH_PIN_1) == LOW && digitalRead(SWITCH_PIN_2) == LOW){
      sprintf(s,"%1d:%4d,%4d",4, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s",s);
      delay(100); 
    }*/
  delay(100);
}
