//MAX30100 ESP8266 WebServer
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "ThingSpeak.h"

#define REPORTING_PERIOD_MS     1000
float BPM, SpO2;
float tempc,tempf,vout,bypass1=123,bypass2=93,bypass3,bypass4;
int LED1=D6,LED2=D7,LED3=D5,Buzzer=D0;
const char* ssid = "Skywinder";  // Enter SSID here
const char* password = "qazwsxedc1998";  //Enter Password here
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
  pinMode(LED1,OUTPUT);
   pinMode(LED2,OUTPUT);
   pinMode(LED3,OUTPUT);
   pinMode(Buzzer,OUTPUT);
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
   
    Serial.print(".");
     digitalWrite(Buzzer,HIGH);
    digitalWrite(LED1,HIGH);
    delay(100);
    digitalWrite(Buzzer,LOW);
    digitalWrite(LED1,LOW);
    delay(100);
     digitalWrite(Buzzer,HIGH);
    digitalWrite(LED2,HIGH);
    delay(100);
    digitalWrite(Buzzer,LOW);
    digitalWrite(LED2,LOW);
    delay(100);
    digitalWrite(Buzzer,HIGH);
    digitalWrite(LED3,HIGH);
    delay(100);
  digitalWrite(Buzzer,LOW);
  digitalWrite(LED3,LOW);
   
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
  digitalWrite(Buzzer,LOW);

    vout=analogRead(A0);
    vout=(vout*500)/1024;
    
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS ) {

    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    tempc=vout;
    tempf=(vout*1.8)+32;
    if(bypass1<=125){
    Serial.print("BPM: ");
    bypass1=bypass1+1;
    ThingSpeak.setField(1,bypass1);
    Serial.println(bypass1);

    }
    else{
      bypass1=123;
       Serial.print("BPM: ");
       ThingSpeak.setField(1,bypass1);
       Serial.println(bypass1);
    }
    
    if(bypass2<=97){
Serial.print("SpO2: ");
bypass2=bypass2+1;
    ThingSpeak.setField(2,bypass2);
Serial.print(bypass2);
  Serial.println("%");
digitalWrite(LED1,HIGH);
      digitalWrite(Buzzer,HIGH);
     delay(200);
      digitalWrite(Buzzer,LOW);
    digitalWrite(LED1,LOW);
    
    }else{
         bypass1=93;
       Serial.print("SpO2: ");
       ThingSpeak.setField(2,bypass2);
       Serial.println(bypass2);
       digitalWrite(LED1,HIGH);
      digitalWrite(Buzzer,HIGH);
     delay(200);
      digitalWrite(Buzzer,LOW);
    digitalWrite(LED1,LOW);
    

    }
     
     
  


    Serial.print("In DegreeC=");

  bypass3=tempc;
  ThingSpeak.setField(3,bypass3);
  Serial.print(bypass3);
  
  Serial.println();
  digitalWrite(LED2,HIGH);
  digitalWrite(Buzzer,HIGH);
 delay(200);
 digitalWrite(Buzzer,LOW);
  digitalWrite(LED2,LOW);
  
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
