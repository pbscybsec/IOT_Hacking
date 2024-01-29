#include <ESP8266WiFi.h>

const char* ssid = "Hellow World";
const char* password = "Helloworld";
int ledPin = 2; // GPIO2 for the built-in LED on most ESP8266 boards

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    delay(10);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Connect to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

void loop() {
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }

    // Wait until the client sends some data
    Serial.println("New client");
    while(!client.available()){
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\n');
    Serial.println(request);
    client.flush();

    // Match the request
    int value = LOW;
    if (request.indexOf("/LED=ON") != -1)  {
        digitalWrite(ledPin, HIGH);
        value = HIGH;
    }
    if (request.indexOf("/LED=OFF") != -1)  {
        digitalWrite(ledPin, LOW);
        value = LOW;
    }

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); // Do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.print("Led pin is now: ");
    if(value == HIGH) {
        client.print("On");
    } else {
        client.print("Off");
    }
    client.println("<br><br>");
    client.println("<a href=\"/LED=ON\"><button>Turn OFF </button></a><br/>");
    client.println("<a href=\"/LED=OFF\"><button>Turn ON </button></a><br />");  
    client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
}