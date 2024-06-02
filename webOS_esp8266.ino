// Author: B. Alin Gabriel
// Website: www.linecode.ro
// Developed with passion to bring innovative IoT solutions
// This code is under MIT License
// Building the future, one line of code at a time
// Follow us for more exciting projects and updates

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// Configurația WiFi
const char* ssid = "WiFi";
const char* password = "WiFi Password";

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
  Serial.begin(230400);
  Serial.println();
  Serial.println("LineCode WebOS System - www.linecode.ro");

  // Conectare la WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting on WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected ...");
  Serial.print("IP is: ");
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
  Serial.println("Server: Status OK");

  // Configurare WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket: Status OK  ");
}

void loop() {
  server.handleClient();
  webSocket.loop();
}

// Funcție pentru a genera pagina HTML
void handleRoot() {
    String html = "<!DOCTYPE html><html><head>";
  html += "<title>LineCode WebOS System Developed by LineCode</title>";
  html += "<style>";
  html += "body { font-family: Menlo, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; }";
  html += "h1 { color: #333; }";
  html += "#terminal { background-color: #000; color: #ccc; padding: 10px; width: 80%; height: 300px; overflow-y: scroll; border: 1px solid #333; white-space: pre-wrap; }";
  html += "#input { width: 80%; padding: 10px; font-size: 16px; margin-top: 10px; border: 1px solid #333; }";
  html += ".button-container { display: flex; flex-wrap: wrap; justify-content: center; margin-top: 10px; }";
  html += ".button { padding: 10px 20px; margin: 5px; background-color: #ccc; color: #008000; border: none; cursor: pointer; display: inline-block; }";
  html += ".button:hover { background-color: #0056b3; }";
  html += ".menu-container { display: flex; flex-wrap: wrap; justify-content: center; margin-top: 20px; }";
  html += ".menu-item { cursor: pointer; padding: 10px; background-color: #ccc; margin: 5px; display: inline-block; }";
  html += ".menu-item:hover { background-color: #0056b3; color: #fff; }";
  html += ".info-container { margin-top: 20px; padding: 20px; background-color: #fff; border: 1px solid #333; width: 80%; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Program Dezvoltat de LineCode - www.linecode.ro</h1>";
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
  html += "<div class='menu-container'>";
  html += "<div class='menu-item' onclick='showInfo(\"scan_wifi\")'>Scan WiFi Info</div>";
  html += "<div class='menu-item' onclick='showInfo(\"sysinfo\")'>SysInfo Info</div>";
  html += "<div class='menu-item' onclick='showInfo(\"restart\")'>Restart Info</div>";
  html += "<div class='menu-item' onclick='showInfo(\"wifi_status\")'>WiFi Status Info</div>";
  html += "<div class='menu-item' onclick='showInfo(\"memory_info\")'>Memory Info</div>";
  html += "<div class='menu-item' onclick='showInfo(\"uptime\")'>Uptime Info</div>";
  html += "</div>";
  html += "<div class='info-container' id='info'></div>";
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
  html += "function showInfo(command) {";
  html += "  var info = '';";
  html += "  if (command === 'scan_wifi') {";
  html += "    info = 'Scan WiFi: This command scans the nearby WiFi networks and lists them with their signal strengths.';";
  html += "  } else if (command === 'sysinfo') {";
  html += "    info = 'SysInfo: This command provides system information including firmware version, free heap memory, CPU frequency, flash chip size, and flash chip speed.';";
  html += "  } else if (command === 'restart') {";
  html += "    info = 'Restart: This command restarts the ESP8266 module.';";
  html += "  } else if (command === 'wifi_status') {";
  html += "    info = 'WiFi Status: This command displays the current WiFi SSID, IP address, and signal strength.';";
  html += "  } else if (command === 'memory_info') {";
  html += "    info = 'Memory Info: This command provides information about the free heap memory, heap fragmentation, and maximum free block size.';";
  html += "  } else if (command === 'uptime') {";
  html += "    info = 'Uptime: This command shows the total uptime of the ESP8266 module in seconds.';";
  html += "  }";
  html += "  document.getElementById('info').innerText = info;";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Funcție pentru a gestiona comanda "scan_wifi"
void handleScanWifi() {
  String networks = "LineCode Scanner V.1 : Scanning WiFi ...\n";
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == 0) {
    networks += "LineCode Scanner V.1 : No WiFi Found :(\n";
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
  String response = "System / Hardware Info:\n";
  response += "Firmware: " + String(ESP.getCoreVersion()) + "\n";
  response += "Free Heap: " + String(ESP.getFreeHeap()) + " bytes\n";
  response += "CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz\n";
  response += "Flash Chip Size: " + String(ESP.getFlashChipSize() / 1024) + " KB\n";
  response += "Flash Chip Speed: " + String(ESP.getFlashChipSpeed() / 1000000) + " MHz\n";
  webSocket.sendTXT(0, response); // Trimite informațiile despre placa către clientul WebSocket
  server.send(200, "text/plain", response); // Trimite informațiile și prin HTTP
}

// Funcție pentru a gestiona comanda "restart"
void handleRestart() {
  String response = "Restarting System Board ...\n";
  webSocket.sendTXT(0, response);
  server.send(200, "text/plain", response);
  delay(1000);
  ESP.restart();
}

// Funcție pentru a gestiona comanda "wifi_status"
void handleWifiStatus() {
  String response = "LineCode Scanner V.1 : Found WiFi :):\n";
  response += "The name of SSID: " + WiFi.SSID() + "\n";
  response += "IP is: " + WiFi.localIP().toString() + "\n";
  response += "Signal of WiFi: " + String(WiFi.RSSI()) + " dBm\n";
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
      String response = "LineCode[%]: -> Command not implemented or found: "+ message + "\n";
      webSocket.sendTXT(num, response);
    }
  }
}
