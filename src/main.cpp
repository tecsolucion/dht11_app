#include <Arduino.h>


#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>



/* Set these to your desired credentials. */
const char *ssid = "UACM-LIZB";  // nombre de red
const char *password = "12345678";  // Password de la red tienen que ser minimo 8



IPAddress ip(192, 168, 4, 6);
IPAddress netmask(255, 255, 255, 0);
const int port = 80; // Port
ESP8266WebServer server(port);

//AsyncWebServer server(80); 

// Acess Point


#define DHTPIN 4    // D2  GPIO4 what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

float c,f,h;
String dhtData;
boolean sensorError = false;



void handleRoot() {
  // Sending sample message if you try to open configured IP Address
  server.send(200, "text/html", "<h1>You are connected</h1>");
}


void sendDhtData() {
  server.send(200, "text/plain", dhtData);
}


void setup() {
  Serial.begin(9600);
  
 
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);



  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.on("/dht", sendDhtData);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("DHT11 Sensor");
  dht.begin();
}



void loop() {
  server.handleClient();
  c = dht.readTemperature();
  f = dht.readTemperature(true);
  h = dht.readHumidity();
  delay(500);
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(c) || isnan(h) || isnan(f)) {
    Serial.print("Sensor Not Connected");
    sensorError=true;
  } else {
    Serial.println("Temperature In Celcius: ");
    Serial.print(c);
    Serial.println(" *C");
    Serial.println("Temperature In Fahrenheit: ");
    Serial.println(f);
    Serial.println(" *F");
    Serial.println("Humidity: ");
    Serial.println(h);
    Serial.println(" %");
  }
  // If there is any issue in sensor connections, it will send 000 as String.
  if(sensorError) {
    dhtData = "sensorError";
  }
  else {
    dhtData = String(c) + ' ' + String(f) + ' ' + String(h);
  }
  delay(2000);
}