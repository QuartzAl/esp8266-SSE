
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>

#define frequency 433E6

#define slaveSelect 15
#define reset 4
#define dio0 5


const char* ssid     = "BIZNET";
const char* password = "manyar33";

String data = String("");


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
    if(!LittleFS.begin()){
      Serial.println("An Error has occurred while mounting LittleFS");
      return;
    }

    Serial.println("LoRa Receiver");

      // LoRa.setPins(ss, reset, dio0);
      LoRa.setPins(slaveSelect, reset, dio0);
      if (!LoRa.begin(frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
      }
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "css/style.css", "text/css");
  });
  server.on("/leaflet.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "css/leaflet.css", "text/css");
  });
  server.on("/leaflet.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "js/leaflet.js", "text/javascript");
  });

  
  events.onConnect([](AsyncEventSourceClient *client){
      if(client->lastId()){
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("hello!", NULL, millis(), 10000);
  });
    server.addHandler(&events);


  // Start server
  server.begin();
}
 
void loop(){  
    int packetSize = LoRa.parsePacket();

    int index = 0;
    if (packetSize) {
      // received a packet
      Serial.print("Received packet '");

      // read packet
      while (LoRa.available()) {
        char character = (char)LoRa.read();
        Serial.print(data);
        data[index++] = character;
      }

      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }else{
      Serial.print("Full message: ");
      Serial.println(data);
      data.clear();
      index = 0;
    }
}