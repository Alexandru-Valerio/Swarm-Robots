/*
Code Name: Arduino Wifi Control Car 
Code URI: https://circuitbest.com/category/arduino-projects/
Additional Board Manager URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json 
Install: ESP8266 by ESP8266 Community
Author: Make DIY
Author URI: https://circuitbest.com/author/admin/
Description: This program is used to control a robot using a app
that communicates with Arduino through a ESP8266 Module.
App URI: https://drive.google.com/file/d/1pvtWsTeXhcJdpHMfGlFmbGDuH7eoLfoP/view?usp=sharing
Version: 2.0
License: Remixing or Changing this Thing is allowed. Commercial use is not allowed.
*/


#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Rightx          GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#include "FirebaseESP8266.h"
#include <FirebaseFS.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#define FIREBASE_HOST "*******" // HOST URL
#define FIREBASE_AUTH "*******" // API KEY

String FirebaseCommand, actualCommand;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

FirebaseData fbdo;

const char* ssidAP = "Wifi CAR";

const char* ssid = "****"; // WIFI SSID
const char* password = "****"; //WIFI PASS


ESP8266WebServer server(80);


void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
// Connecting WiFi

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssidAP);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
  
  Serial.println("");
  Serial.println("WiFi connected");

 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());
 // Connecting to FireBase

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  Firebase.reconnectWiFi(true);
  fbdo.setBSSLBufferSize(1024, 1024);
  fbdo.setResponseSize(1024);

}

void startCar(){
  digitalWrite(IN_1, HIGH); //7,6,5,4
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
}
void backCar(){
  digitalWrite(IN_1, LOW); //7,6,5,4
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
}
void stopCar(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
}
void rightTurn(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
}
void leftTurn(){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
} 


void getCommand()
{
        if (Firebase.getString(fbdo, "/Command")) {
      
            if (fbdo.dataType() == "string") {
              FirebaseCommand = fbdo.stringData();
            }
    
      } else {
        Serial.println(fbdo.errorReason());
      }
}

void sendCommand(String actualCommand, String FirebaseCommand){
  if(actualCommand != FirebaseCommand){
    if(Firebase.setString(fbdo, "/Command", actualCommand)){
          Serial.println("DONE! "+ actualCommand); 
      }
      else
          Serial.println("FAILED!");
  }
  
}
void loop(){
  server.handleClient();
  getCommand();
  actualCommand = server.arg("State");
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  if (actualCommand == "F")
  {
    startCar();
    sendCommand(actualCommand,FirebaseCommand);
  }
  else if (actualCommand == "B")
  {
    backCar();
    sendCommand(actualCommand,FirebaseCommand);
  }
  else if (actualCommand == "L")
  {
    leftTurn();
    sendCommand(actualCommand,FirebaseCommand);
  }
  else if (actualCommand == "R")
  {
    rightTurn();
    sendCommand(actualCommand,FirebaseCommand);
  }
   else if (actualCommand == "S")
  {
    stopCar();
    sendCommand(actualCommand,FirebaseCommand);
  }
}
