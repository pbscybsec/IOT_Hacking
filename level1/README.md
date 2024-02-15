# Level 1 Attack
This project demonstrates how to create a basic web server with an ESP8266 module to remotely control an LED through a web interface. Level 1 device just uses get operation to control the led and can be easily hacked by extracting out the Get headers using tcpdump &amp; netcat tool.

## Code Breakdown

### Including Libraries

```cpp
#include <ESP8266WiFi.h>
```
This line includes the ESP8266WiFi library, which provides the necessary functions to connect to a WiFi network and create a web server.
### Configuration Variables
```cpp
const char* ssid = "ssid";
const char* password = "pass";
int ledPin = 5; // GPIO5 (D1 on NodeMCU board)
```
- ssid and password are used to connect to your WiFi network. Replace "ssid" and "pass" with your actual WiFi credentials.
- ledPin is set to 5, corresponding to GPIO5 on the ESP8266, commonly labeled as D1 on NodeMCU boards. This is where you'll connect the LED.

### Setting Up the Server
```cpp
WiFiServer server(80);
```
This line initializes a WiFiServer object to listen on port 80, which is the default HTTP port.
### Setup Function
```cpp
void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    server.begin();
    Serial.print("Use this URL to connect: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}
```
- Initializes serial communication for debugging.
- Sets the LED pin as an output and ensures the LED is off initially.
- Connects to the specified WiFi network.
- Starts the web server and prints the ESP8266's IP address to the Serial Monitor.
### Loop Function
```cpp
void loop() {
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    String request = client.readStringUntil('\n');
    if (request.indexOf("/LED=ON") != -1)  {
        digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("/LED=OFF") != -1) {
        digitalWrite(ledPin, LOW);
    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("<!DOCTYPE HTML><html>");
    client.print("Led pin is now: ");
    client.print(digitalRead(ledPin) ? "On" : "Off");
    client.println("<br><br>");
    client.println("<a href=\"/LED=ON\"><button>Turn ON </button></a>");
    client.println("<a href=\"/LED=OFF\"><button>Turn OFF </button></a>");  
    client.println("</html>");
}
```
- Checks for a client connection and reads their HTTP request.
- Parses the request. If it contains /LED=ON, the LED is turned on. If it contains /LED=OFF, the LED is turned off.
- Sends an HTTP response back to the client with the current state of the LED and buttons to control it.
### Usage

After uploading this code to an ESP8266, connect an LED to GPIO5 (D1) and GND. Use any web browser within the same WiFi network to navigate to the ESP8266's IP address. You'll be presented with a simple web page with buttons to turn the LED on and off.
