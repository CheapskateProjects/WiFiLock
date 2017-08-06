/*
  WiFiLock

  This project is wlan connected (ESP8266) lock that can be controlled if you have access to wlan

  Gpio2 pin is used to unlock the lock. You should build your electronics accordingly. 

  created   Aug 2017
  by CheapskateProjects

  ---------------------------
  The MIT License (MIT)

  Copyright (c) 2017 CheapskateProjects

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// WLAN configuration
const char* ssid = "XXX";
const char* password = "YYY";

// Constants that can be changed based on your hardware or preferences
const int solenoidPin = 2;

// status 
bool locked = true;

ESP8266WebServer server(80);

// Toggle lock status
void toggle()
{
  if(locked)
  {
    digitalWrite(solenoidPin, 1);
    locked = false;
  }
  else
  {
    digitalWrite(solenoidPin, 0);
    locked = true;
  }
  
  server.send(200, "text/html", "<html><head><meta http-equiv=\"refresh\" content=\"0; url=/\" /></head><body></body></html>");
}

// Handler for each url except toggle (just print the button)
void handlePrint()
{
  String html = "<html><head></head><body style=\"margin:10px;\"><a style=\"display: inline-block; height: 30%; background-color: grey; color: white; font-size: 25vmin; font-family: arial; width: 100%; text-align:center; text-decoration: none;\" href=\"/toggle\">";

  if(locked)
  {
    html += "Unlock";
  }
  else
  {
    html += "Lock";
  }

  html += "</a></body></html>";
  
  server.send(200, "text/html", html);
}

// Init output pin
void initOutput()
{
  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, 0);
}

// Init wlan connection
void initWLAN()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void setup(void)
{
  Serial.begin ( 115200 );
  initOutput();
  initWLAN();
  Serial.print ("Connected to ");
  Serial.println(ssid );
  Serial.print ("IP address: ");
  Serial.println (WiFi.localIP());
  
  server.on("/toggle", [](){toggle();});
  server.onNotFound(handlePrint);
  server.begin();
}

void loop(void)
{
  server.handleClient();
}
