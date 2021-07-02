// slave car 

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

#define FIREBASE_HOST "****" // HOST URL
#define FIREBASE_AUTH "****" // API KEY

String actualCommand;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

FirebaseData fbdo;

const char* ssid = "****"; // WIFI SSID
const char* password = "****"; // WIFI PASS

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
// Connecting WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
  
  Serial.println("");
  Serial.println("WiFi connected");

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
              actualCommand = fbdo.stringData();
            }
    
      } else {
        Serial.println(fbdo.errorReason());
      }
}

 void loop() {
   
      getCommand();
      analogWrite(ENA, 255);
      analogWrite(ENB, 255);
          
        if (actualCommand == "F")
        {
          startCar();
        }
        else if (actualCommand == "B")
        {
          backCar();
        }
        else if (actualCommand == "L")
        {
          leftTurn();
        }
        else if (actualCommand == "R")
        {
          rightTurn();
        }
         else if (actualCommand == "S")
        {
          stopCar();
        }
}
