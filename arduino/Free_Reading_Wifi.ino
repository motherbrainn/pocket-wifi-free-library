
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h> // Include the WebSocketsServer library
#include "./DNSServer.h"
#include <FS.h>

DNSServer dnsServer;
const byte DNS_PORT = 53;

ESP8266WebServer server(80);
WebSocketsServer webSocketServer = WebSocketsServer(81); // Create a WebSocket server on port 81

#ifndef STASSID
#define STASSID "POPUP FREE LIBRARY"
#endif

IPAddress apIP(192, 168, 4, 1);
const char* ssid = STASSID;

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Handle WebSocket events
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] Connected from IP %s\n", num, webSocketServer.remoteIP(num).toString().c_str());
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received text: %s\n", num, payload);

      // Broadcast the received message to all connected clients
      webSocketServer.broadcastTXT(payload, length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);

  dnsServer.start(DNS_PORT, "*", apIP);
  MDNS.begin("esp8266", WiFi.softAPIP());

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  SPIFFS.begin();

  // Serve web pages from SPIFFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.4.1/index.html\" /></head><body><p>redirecting...</p></body>";
      server.send(200, "text/html", metaRefreshStr);
    }
  });

  // CORS handling for WebSocket
  server.on("/sendMessage", HTTP_OPTIONS, [](){
    handleCORS();
    server.send(204);
  });

  // Start WebSocket server
  webSocketServer.begin();

  // Set the callback function for WebSocket events
  webSocketServer.onEvent(webSocketEvent);

  // Start HTTP server
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  webSocketServer.loop(); // Handle WebSocket events
  delay(50);
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".mp4")) return "video/mp4";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
