LineCode WebOS System
This project provides a web-based operating system for the ESP8266 module, developed by LineCode. The system allows you to control and monitor various functions of the ESP8266 through a web interface and WebSocket communication.

Features
Web Interface: Control and monitor the ESP8266 through a user-friendly HTML interface.
WiFi Scanning: Scan for nearby WiFi networks and display their signal strengths.
System Information: Retrieve and display system and hardware information.
Restart: Remotely restart the ESP8266 module.
WiFi Status: Display the current WiFi SSID, IP address, and signal strength.
Memory Information: Provide details about free heap memory, heap fragmentation, and maximum free block size.
Uptime: Display the total uptime of the ESP8266 module in seconds.
GPIO Control: Control GPIO pins through WebSocket commands.
Getting Started
Prerequisites
ESP8266 module (e.g., NodeMCU, Wemos D1 Mini)
Arduino IDE with ESP8266 board support installed
WiFi network credentials
Installation
Clone the repository:

bash
Copy code
git clone https://github.com/bltaln/ESP8266.git
cd ESP8266
Open the project in Arduino IDE:

Launch the Arduino IDE.
Open the linecode-webos-system.ino file from the cloned repository.
Configure WiFi credentials:

In the webOS_esp8266.ino file, update the ssid and password variables with your WiFi network credentials:
cpp
Copy code
const char* ssid = "Your_SSID";
const char* password = "Your_Password";
Select the ESP8266 board:

Go to Tools > Board > ESP8266 Boards and select your ESP8266 module (e.g., NodeMCU 1.0, Wemos D1 Mini).
Upload the code:

Connect your ESP8266 to your computer via USB.
Select the appropriate COM port under Tools > Port.
Click the upload button in the Arduino IDE to flash the code to the ESP8266.
Usage
Connect to WiFi:

After uploading the code, open the Serial Monitor (set the baud rate to 230400) to view the connection process.
The ESP8266 will attempt to connect to the specified WiFi network.
Once connected, the IP address of the ESP8266 will be displayed in the Serial Monitor.
Access the Web Interface:

Open a web browser and navigate to the IP address displayed in the Serial Monitor.
The web interface will be loaded, displaying various control buttons and information sections.
Web Interface Commands:

Scan WiFi: Click the "Scan WiFi" button to scan for nearby WiFi networks.
SysInfo: Click the "SysInfo" button to retrieve and display system information.
Restart: Click the "Restart" button to restart the ESP8266 module.
WiFi Status: Click the "WiFi Status" button to display current WiFi details.
Memory Info: Click the "Memory Info" button to get information about memory usage.
Uptime: Click the "Uptime" button to view the total uptime of the module.
GPIO Control: Send a command in the format gpioX (where X is the GPIO pin number) to toggle the state of a GPIO pin.
Code Overview
The main components of the code include:

WiFi Configuration: Connects the ESP8266 to a specified WiFi network.
Web Server Initialization: Sets up the HTTP server to serve the web interface and handle various requests.
WebSocket Initialization: Sets up a WebSocket server to handle real-time communication between the web interface and the ESP8266.
Request Handlers: Functions to handle different commands such as scanning WiFi, retrieving system info, restarting the module, etc.
WebSocket Event Handling: Processes incoming WebSocket messages and executes corresponding functions.
Contributing
Contributions are welcome! Feel free to open issues or submit pull requests with improvements.

License
This project is licensed under the MIT License - see the LICENSE file for details.
