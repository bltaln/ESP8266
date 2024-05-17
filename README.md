This code is created solely for EDUCATIONAL PURPOSES !!!


A simple scanner wifi to generate password for the ESP8266 board that find and connect to an available Wi-Fi network using a randomly generated password for each connection attempt. 
Here's how it works more exactly:

Initialization: At the beginning of the program, serial communication is configured to allow displaying debug messages. 
The ESP8266 module is also set to station mode (WIFI_STA) to enable connection to a Wi-Fi network.

Scanning for available Wi-Fi networks: All available Wi-Fi networks are searched for and displayed in the Serial Monitor. 
If no networks are detected, an appropriate message is displayed, and the program ends.

Connection attempts: If available Wi-Fi networks are detected, the program attempts to connect to each found network repeatedly. 

For each connection attempt:
A random password is generated for the respective network using the generatePassword() function.
An attempt is made to connect to the respective network using the generated SSID and password.
During the connection attempt, the built-in LED pulses to indicate activity.
Connection feedback: If a successful connection to a Wi-Fi network is established, a success message is displayed along with the IP address assigned by the router, 
and the built-in LED remains continuously lit. Otherwise, an error message is displayed, and a delay is applied before attempting to connect to the next available network.

Additional functions:

The generatePassword() function is responsible for generating random passwords, combining characters from a predefined set of special characters and alphanumeric characters.
The pulseLED() function is used to pulse the LED during connection attempts.
In essence, this script automates the process of connecting the ESP8266 board to a Wi-Fi network 
by attempting different combinations of randomly generated passwords until a successful connection is established or no more networks are available.


WiFi Scanner with automated password generator 
This program is developed by LineCode.Ro / Project_Luna.
Any modifications must be attributed to the author.
Suggestions for modifying this code are welcome.
This code is created solely for educational purposes.
Any illegal use will be subject to legal consequences in your country.






