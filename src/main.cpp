
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

int index1 = 0;
char character = ' ';

String data = String("");


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");


void configureEvents() {
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client connections. Id: %u\n", client->lastId());
    }
    Serial.print("hello?");
    // and set reconnect delay to 1 second
    client->send("hello from ESP8266",NULL,millis(),1000);
  });
  server.addHandler(&events);
}

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
  
  WiFi.begin(ssid, password);
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
  server.on("/assets/index-146e7c80.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "assets/index-146e7c80.js", "text/javascript");
  });

  
  configureEvents();
  // Start server
  server.begin();
}
 
int counter = 0;
void loop(){  
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      Serial.print("Received packet '");

      // read packet
      while (LoRa.available()) {
        character = (char)LoRa.read();
        Serial.println(character);
        data = String(data + character);
      }

      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }else if(data != ""){
      Serial.print("Full message: ");
      // String potato = String('['+ String((counter*2 + 50)) + ',' + ' ');
      // potato = String(potato  + String((counter + 30)) + ']');
      data = String('['+data+']');
      events.send(data.c_str());
      Serial.println(data);
      data.clear();
      index1 = 0;
      counter++;
    }
}