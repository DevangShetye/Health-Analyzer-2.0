//MAX30100 ESP8266 WebServer
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "ThingSpeak.h"

#define REPORTING_PERIOD_MS     1000
float BPM, SpO2;
float tempc,tempf,vout,bypass1,bypass2,bypass3,bypass4;

const char* ssid = "Sky";  // Enter SSID here
const char* password = "devang2234";  //Enter Password here
unsigned long myChannelNumber = 1718283;
const char *myWriteAPIKey="GUHFDUECZB648WLB";
const char *server="api.thingspeak.com";
PulseOximeter pox;
uint32_t tsLastReport = 0;
WiFiClient client;
void updateThingSpeak(float bypass1, float bypass2,float bypass3,float bypass4){
    ThingSpeak.setField(1,bypass1);
    ThingSpeak.setField(2,bypass2);
    ThingSpeak.setField(3,bypass3);
    ThingSpeak.setField(4,bypass4);
    ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
    delay(5000);
}

void setup() {
   pinMode(16, OUTPUT);
   pinMode(A0,INPUT);
   Serial.begin(9600);
  ThingSpeak.begin(client);
  delay(2000);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);
  
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  Serial.print("Initializing pulse oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
    
  }
}
void loop() {
    vout=analogRead(A0);
    vout=(vout*500)/1024;
    
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    tempc=vout;
    tempf=(vout*1.8)+32;

    Serial.print("BPM: ");
    bypass1=BPM;
    ThingSpeak.setField(1,bypass1);
    Serial.println(bypass1);

    Serial.print("SpO2: ");
    bypass2=SpO2;
    ThingSpeak.setField(2,bypass2);
    Serial.print(bypass2);
    Serial.println("%");

    Serial.print("In DegreeC=");

  bypass3=tempc;
  ThingSpeak.setField(3,bypass3);
  Serial.print(bypass3);
  
  Serial.println();
  Serial.print("In F=");
  Serial.print("\t");
  bypass4=tempf-37;
  ThingSpeak.setField(4,bypass4);
  Serial.println(bypass4);
  Serial.println("***********");
  Serial.println();
  ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
  tsLastReport = millis();
  
  }
  
}
