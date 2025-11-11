#include <WiFi.h>
#include <AsyncTCP.h> // by ESP32Async - https://github.com/ESP32Async/AsyncTCP
#include <ESPAsyncWebServer.h> // by ESP32Async - https://github.com/ESP32Async/ESPAsyncWebServer/releases

#include <SDcard.h>
void initSDCard();
void writeFile();
void appendFile();
void deleteFile();

// Replace with your network credentials
const char* ssid     = "DoorMonitor-AP";
const char* password = "42069247";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

void WiFisetup(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void webServerSetup(){
    // Handle the root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

  // Handle the download button
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/data.txt", String(), true);
  });

/*
  // Handle the download button
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/var.txt", String(), true);
  });
*/ 

  // Handle the View Data button
  server.on("/view-data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/data.txt", "text/plain", false);
  });

  // Handle the delete button
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    deleteFile(SD, dataPath);
    request->send(200, "text/plain", "data.txt was deleted.");
  });

  // Uncomment the following line if you need to serve more static files like CSS and javascript or favicon
  //server.serveStatic("/", SD, "/");
  
  server.begin();
  }