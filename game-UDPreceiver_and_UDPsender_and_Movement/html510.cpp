/*
 * MEAM510 hacks for web interface
 * Jan 2021
 * Use at your own risk
 * 
 * Mark Yim
 * University of Pennsylvania
 * copyright (c) 2021 All Rights Reserved
 */
#include "html510.h"
  
void HTML510Server::begin(int port){
    server.begin(port);
}
  
void HTML510Server::attachHandler( String key, void (*handler)() ) {
  char lastchar = key[key.length()-1];
  if (lastchar != ' ' && lastchar != '=') 
    key = key + " ";
  handlerptrs[numHandler] = handler;
  handlerpars[numHandler] = "GET "+key;
  numHandler++;
}

//*** HTML text handling routines ****
void HTML510Server::sendhtml(String data) {
  String s = HTMLtext;
  s = s+data;
  client.print(s);
}

void HTML510Server::sendplain(String data) {
  String s = plaintext;
  s = s+data;
  client.print(s);
}

String HTML510Server::getText(){
  String txt = "";
  while (client.connected()) {
    if (client.available())  {
      char c = client.read();
      if (c<=' ') break;
      txt += c;
    }        
  }
  return txt;
}

int HTML510Server::getVal(){
  String num = "";
  while (client.connected()) {
    if (client.available())  {
      char d = client.read();
      num += d;
      if (d !='-' && (d<'0' || d>'9') ) break;
    }        
  }
  return num.toInt();
}
  
//*** web server hack ***
// Could use Webserver.h routines, but they are 100x  slower, so use a faster hack
// looks for key parameter at beginning of header, ignores everything else
void HTML510Server::serve() { 
  int breakflag=0; 
  client = server.available();
  if (client)  {
    String currentLine = "";

    while (client.connected()) {
      if (client.available())  {
        char c = client.read();
        //Serial.write(c);
        if (c== '\n') break; // only read 1st line, break & close connection 
        
        currentLine += c;
        for (int i=0;i<numHandler; i++){
          if(currentLine.startsWith(handlerpars[i])) {
            (*handlerptrs[i])(); 
            client.stop();    // found one, so close the connection
            return ;
          }
        }
      }
    }
    client.stop();    // Close the connection
  }
}
