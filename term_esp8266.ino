#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// Configurația WiFi
const char* ssid = "WirLab Gabriel";
const char* password = "29101986";

// Inițializare server web pe portul 80
ESP8266WebServer server(80);
// Inițializare WebSocket pe portul 81
WebSocketsServer webSocket = WebSocketsServer(81);

void handleRoot();
void handleScanWifi();
void handleSysInfo();
void handleRestart();
void handleWifiStatus();
void handleMemoryInfo();
void handleGpioControl(String message);
void handleUptime();

void setup() {
  // Inițializare consola serială
  Serial.begin(115200);
  Serial.println();
  Serial.println("LineCode / Project_Luna");

  // Conectare la WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected on WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configurare server web
  server.on("/", handleRoot);
  server.on("/scan_wifi", handleScanWifi);
  server.on("/sysinfo", handleSysInfo);
  server.on("/restart", handleRestart);
  server.on("/wifi_status", handleWifiStatus);
  server.on("/memory_info", handleMemoryInfo);
  server.on("/uptime", handleUptime);
  server.begin();
  Serial.println("Server Online...");

  // Configurare WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket Online...");
}

void loop() {
  server.handleClient();
  webSocket.loop();
}

// Funcție pentru a genera pagina HTML
void handleRoot() {
    String html = "<!DOCTYPE html><html><head>";
  html += "<title>ESP8266 Terminal</title>";
  html += "<style>";
  html += "body { font-family: Menlo, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; }";
  html += "h1 { color: #333; }";
  html += "#terminal { background-color: #000; color: #ccc; padding: 10px; width: 80%; height: 300px; overflow-y: scroll; border: 1px solid #333; white-space: pre-wrap; }";
  html += "#input { width: 80%; padding: 10px; font-size: 16px; margin-top: 10px; border: 1px solid #333; }";
  html += ".button-container { display: flex; flex-wrap: wrap; justify-content: center; margin-top: 10px; }";
  html += ".button { padding: 10px 20px; margin: 5px; background-color: #ccc; color: #008000; border: none; cursor: pointer; display: inline-block; }";
  html += ".button:hover { background-color: #0056b3; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>ESP8266 Terminal</h1>";
  html += "<div id='terminal'></div>";
  html += "<input type='text' id='input' autofocus placeholder='Enter command here'>";
  html += "<div class='button-container'>";
  html += "<button class='button' onclick='sendCommand(\"scan_wifi\")'>Scan WiFi</button>";
  html += "<button class='button' onclick='sendCommand(\"sysinfo\")'>SysInfo</button>";
  html += "<button class='button' onclick='sendCommand(\"restart\")'>Restart</button>";
  html += "<button class='button' onclick='sendCommand(\"wifi_status\")'>WiFi Status</button>";
  html += "<button class='button' onclick='sendCommand(\"memory_info\")'>Memory Info</button>";
  html += "<button class='button' onclick='sendCommand(\"uptime\")'>Uptime</button>";
  html += "</div>";
  html += "<script>";
  html += "var gateway = `ws://${window.location.hostname}:81/`;";
  html += "var websocket;";
  html += "window.addEventListener('load', onLoad);";
  html += "function onLoad(event) {";
  html += "  initWebSocket();";
  html += "  document.getElementById('input').addEventListener('keydown', function(e) {";
  html += "    if (e.key === 'Enter') {";
  html += "      sendMessage(e.target.value);";
  html += "      e.target.value = '';"; 
  html += "    }";
  html += "  });";
  html += "}";
  html += "function initWebSocket() {";
  html += "  websocket = new WebSocket(gateway);";
  html += "  websocket.onopen = onOpen;";
  html += "  websocket.onclose = onClose;";
  html += "  websocket.onmessage = onMessage;";
  html += "}";
  html += "function onOpen(event) { console.log('Connection opened'); }";
  html += "function onClose(event) { console.log('Connection closed'); setTimeout(initWebSocket, 2000); }";
  html += "function onMessage(event) { document.getElementById('terminal').innerHTML += event.data + '\\n'; }";
  html += "function sendMessage(message) { websocket.send(message); }";
  html += "function sendCommand(command) { websocket.send(command); }";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Funcție pentru a gestiona comanda "scan_wifi"
void handleScanWifi() {
  String networks = "Available WiFi networks:\n";
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    networks += "No WiFi networks found.\n";
  } else {
    for (int i = 0; i < numNetworks; ++i) {
      networks += String(i+1) + ". " + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + " dBm)\n";
    }
  }
  webSocket.sendTXT(0, networks); // Trimite rezultatul scanării rețelelor WiFi către clientul WebSocket
  server.send(200, "text/plain", networks); // Trimite rezultatul și prin HTTP
}

// Funcție pentru a gestiona comanda "sysinfo"
void handleSysInfo() {
  String response = "ESP8266 Board Information:\n";
  response += "Firmware: " + String(ESP.getCoreVersion()) + "\n";
  response += "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n";
  response += "CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz\n";
  response += "Flash Chip Size: " + String(ESP.getFlashChipSize() / 1024) + " KB\n";
  response += "Flash Chip Speed: " + String(ESP.getFlashChipSpeed() / 1000000) + " MHz\n";
  webSocket.sendTXT(0, response); // Trimite informațiile despre placa ESP8266 către clientul WebSocket
  server.send(200, "text/plain", response); // Trimite informațiile și prin HTTP
}

// Funcție pentru a gestiona comanda "restart"
void handleRestart() {
  String response = "Restarting ESP8266 Board ...\n";
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
  delay(1000);
  ESP.restart();
}

// Funcție pentru a gestiona comanda "wifi_status"
void handleWifiStatus() {
  String response = "WiFi Status:\n";
  response += "SSID: " + WiFi.SSID() + "\n";
  response += "IP Address: " + WiFi.localIP().toString() + "\n";
  response += "Signal Strength: " + String(WiFi.RSSI()) + " dBm\n";
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
}

// Funcție pentru a gestiona comanda "memory_info"
void handleMemoryInfo() {
  String response = "Memory Info:\n";
  response += "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n";
  response += "Heap Fragmentation: " + String(ESP.getHeapFragmentation()) + "%\n";
  response += "Max Free Block Size: " + String(ESP.getMaxFreeBlockSize()) + " bytes\n";
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
}

// Funcție pentru a gestiona comanda "uptime"
void handleUptime() {
  String response = "Uptime: " + String(millis() / 1000) + " seconds\n";
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
}

// Funcție pentru a gestiona controlul GPIO
void handleGpioControl(String message) {
  int gpioPin = message.substring(5).toInt();
  String response = "Toggling GPIO " + String(gpioPin) + "\n";
  pinMode(gpioPin, OUTPUT);
  digitalWrite(gpioPin, !digitalRead(gpioPin)); // Toggle pin state
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*) payload);
    Serial.println(message);
    if (message == "scan_wifi") {
      handleScanWifi();
    } else if (message == "sysinfo") {
      handleSysInfo();
    } else if (message == "restart") {
      handleRestart();
    } else if (message == "wifi_status") {
      handleWifiStatus();
    } else if (message == "memory_info") {
      handleMemoryInfo();
    } else if (message.startsWith("gpio")) {
      handleGpioControl(message);
    } else if (message == "uptime") {
      handleUptime();
    } else {
      String response = "Comanda primită: " + message + "\n";
      webSocket.sendTXT(num, response);
    }
  }
}
