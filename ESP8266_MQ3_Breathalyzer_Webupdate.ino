#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space

#include <CayenneDefines.h>
#include <BlynkSimpleEsp8266.h>
#include <CayenneWiFiClient.h>
#include <CayenneClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

// Cayenne authentication token. This is obtained from the Cayenne Dashboard.
const char* token = "rtibpy87tp";
const char* ssid = "Wu-Tang LAN";
const char* password = "allstream11";
const char* host = "esp8266-webupdate";

int reading = 0;
int highScore = 0;
int lastState = 0;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting Sketch...");
  pinMode(A0, INPUT);

  /* // Cayenne.begin takes care of this
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }
 */
  Cayenne.begin(token, ssid, password);

  MDNS.begin(host);
  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop()
{
  httpServer.handleClient();
  Cayenne.run();
}

  CAYENNE_OUT(V0)
{
  reading = analogRead(A0);
  if(reading > highScore){
    highScore = reading;
  }
  Cayenne.virtualWrite(V0, reading, DISTANCE, MILLIMETERS);
}

 CAYENNE_OUT(V1)
{
  Cayenne.virtualWrite(V1, highScore, DISTANCE, MILLIMETERS);
}

CAYENNE_OUT(V2)
{
  Cayenne.virtualWrite(V2, reading, DISTANCE, MILLIMETERS);
}
CAYENNE_IN(V3)
{
  int state = getValue.asInt();
  if(state != lastState){
    highScore = 0;
    lastState = state;
  }
  
}

