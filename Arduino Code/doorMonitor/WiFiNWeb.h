#include <WiFi.h>
#include <AsyncTCP.h> // by ESP32Async - https://github.com/ESP32Async/AsyncTCP
#include <ESPAsyncWebServer.h> // by ESP32Async - https://github.com/ESP32Async/ESPAsyncWebServer/releases

#include <webPage.h>
#include <SDcard.h>
void initSDCard();
void writeFile();
void appendFile();
void deleteFile();
void renameFile();

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
   /* // Handle the root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

  // Handle the download button
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/data.txt", String(), true);
  });

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

  // attach filesystem root at URL /fs
  //server.serveStatic("/fs", SPIFFS, "/");
  //server.serveStatic("/", SD, "/");
  */

  // Serve file browser page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", fileBrowserPage);
  });

  // Return file list as plain text: "filename,size"
  server.on("/list", HTTP_GET, [](AsyncWebServerRequest *request){
    File root = SD.open("/");
    if (!root || !root.isDirectory()) {
      request->send(500, "text/plain", "SD error");
      return;
    }

    File file = root.openNextFile();
    String output;

    while (file) {
      if (!file.isDirectory()) {
        output += String(file.name()) + "," + String(file.size()) + "\n";
      }
      file = root.openNextFile();
    }

    request->send(200, "text/plain", output);
  });

  // Download handler
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("file")) {
      request->send(400, "text/plain", "Missing ?file=");
      return;
    }

    String path = request->getParam("file")->value();

    // Ensure filename starts with "/"
    if (!path.startsWith("/")) {
      path = "/" + path;
    }

    if (!SD.exists(path)) {
      request->send(404, "text/plain", "File not found");
      return;
    }

    request->send(SD, path, String(), true);  // force download
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
  if (!request->hasParam("file")) {
    request->send(400, "text/plain", "Missing ?file=");
    return;
  }

  String path = request->getParam("file")->value();

  // ensure path starts with "/"
  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  if (!SD.exists(path)) {
    request->send(404, "text/plain", "File not found");
    return;
  }

  if (SD.remove(path)) {
    request->send(200, "text/plain", "Deleted: " + path);
  } else {
    request->send(500, "text/plain", "Failed to delete: " + path);
  }
});

  server.begin();
}
