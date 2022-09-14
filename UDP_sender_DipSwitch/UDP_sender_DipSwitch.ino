#include <WiFi.h>
#include <WiFiUdp.h>
#include "vive510.h"

#define SWITCH_PIN_1 21   //dip switch pin 1
#define SWITCH_PIN_2 22   //dip switch pin 2
#define SIGNALPIN1 26

Vive510 vive1(SIGNALPIN1);

const char* ssid     = "TP-Link_E0C8";
const char* password = "52665134";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2510; // port for cans is 1510, port for robots is 2510
IPAddress ipTarget(192, 168, 1, 255); // 255 is a broadcast address to everyone at 192.168.1.xxx
IPAddress ipLocal(192, 168, 1, 178);  // replace with your IP address

void fncUdpSend(char *datastr, int len)
{
  UDPTestServer.beginPacket(ipTarget, UDPPort);
  UDPTestServer.write((uint8_t *)datastr, len);
  UDPTestServer.endPacket();
}

void setup() {
  Serial.begin(115200);
  vive1.begin();
  Serial.println("Vive trackers started");
  
  //pinMode(SWITCH_PIN_1,INPUT_PULLUP);
  //pinMode(SWITCH_PIN_2,INPUT_PULLUP);
  Serial.print("Connecting to ");  Serial.println(ssid);
 
  WiFi.config(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  UDPTestServer.begin(UDPPort); // strange bug needs to come after WiFi.begin but before connect

}
                                                
void loop() {  
  char s[13];
  int x, y; // x,y of vive detection
  if (vive1.status() == VIVE_LOCKEDON) {
    if(digitalRead(SWITCH_PIN_1) == HIGH && digitalRead(SWITCH_PIN_2) == LOW){
      x = vive1.xCoord();
      y = vive1.yCoord();
      sprintf(s,"%1d:%4d,%4d",1, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s \n",s);
      delay(100); 
    }
    else if(digitalRead(SWITCH_PIN_1) == LOW && digitalRead(SWITCH_PIN_2) == HIGH){
      x = vive1.xCoord();
      y = vive1.yCoord();
      sprintf(s,"%1d:%4d,%4d",2, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s \n",s);
      delay(100); 
    }
    else if(digitalRead(SWITCH_PIN_1) == HIGH && digitalRead(SWITCH_PIN_2) == HIGH){
      x = vive1.xCoord();
      y = vive1.yCoord();
      sprintf(s,"%1d:%4d,%4d",3, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s \n",s);
      delay(100); 
    }
    else if(digitalRead(SWITCH_PIN_1) == LOW && digitalRead(SWITCH_PIN_2) == LOW){
      x = vive1.xCoord();
      y = vive1.yCoord();
      sprintf(s,"%1d:%4d,%4d",4, x, y); 
      fncUdpSend(s,13);
      Serial.printf("sending data: %s \n",s);
      delay(100); 
    }
  }
  else 
     {
      vive1.sync(15);
     }
  delay(100);
}
