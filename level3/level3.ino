#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "rajesh@vianet"; 
const char* password = "Aryan@1337"; 

WebServer server(80);

const int gpio1_pin = 14; 
const int gpio2_pin = 18;
const int gpio3_pin = 19; 


bool is_authentified(){
  Serial.println("Enter is authentified");
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin(){
  String msg;
  if (server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")){
    Serial.println("Disconnection");
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.sendHeader("Set-Cookie","ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")){
    if (server.arg("PASSWORD") == "t$o$o$r$" ) // enter ur username and password you want
    {
      server.sendHeader("Location","/");
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;

      }

  msg = "Wrong username/password! try again.";
  Serial.println("Log in Failed");
  }
  String content = "<html><script type='text/javascript'>function encodeMyHtml(){var htmlToEncode = document.getElementById('password').value;var str = String(htmlToEncode) ;var newString = '';for (var i = str.length - 1; i >= 0; i--) { newString += str[i]+'$';}var encodedHtml = escape(htmlToEncode);document.getElementById('password').value=newString;return true;} </script>";
  content += "<body style='background-color:MediumAquaMarine'><form id='form1' action='/login' method='POST'><p  align ='center' style='font-size:300%;'><u><b><i>  Log In  </i></b></u></p><br>";
  content += " <p   align ='center' style='font-size:160%'><b> UserName:<input type='text' name='USERNAME' placeholder='user name' required></b></p><br>";
  content += "<p  align ='center' style='font-size:160%'><b>Password:<input type='password' id='password' name='PASSWORD' placeholder='password' required></b></p><br>";
  content += "<p  align ='center' style='font-size:160%'><input type='submit' name='SUBMIT' onclick='return encodeMyHtml()' value='Submit'></form>" + msg + "</p><br> </body></html>";
  server.send(200, "text/html", content); 
}

//root page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()){
    server.sendHeader("Location","/login");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
    return;
  }
  String content =  "<body style='background: #f58f25'><h1 align ='center'><b><u><i><strong>HOME AUTOMATION</strong></i></u></b></h1><br><p align ='center
  content += "<br><p align='center'>Developed by <b>Presidential Cybersecurity Club</b></p>";
  content += "<br><br><br><br></body>"; 
  
   if (server.hasHeader("User-Agent")){
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
    
    
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
}

//no need authentification
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void setup() {
    pinMode(gpio1_pin, OUTPUT);
    digitalWrite(gpio1_pin, LOW);
    pinMode(gpio2_pin, OUTPUT);
    digitalWrite(gpio2_pin, LOW);
    pinMode(gpio3_pin, OUTPUT);
    digitalWrite(gpio3_pin, LOW);

    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp32")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/login", handleLogin);
    server.onNotFound(handleNotFound);

    const char* headerkeys[] = {"User-Agent", "Cookie"};
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
    server.collectHeaders(headerkeys, headerkeyssize);

    server.on("/switch1On", []() {
        if (is_authentified()) {
            digitalWrite(gpio1_pin, HIGH);
            server.send(200, "text/plain", "Switch 1 turned ON");
        } else {
            server.send(401, "text/plain", "Authentication Required");
        }
    });

    server.on("/switch1Off", []() {
        if (is_authentified()) {
            digitalWrite(gpio1_pin, LOW);
            server.send(200, "text/plain", "Switch 1 turned OFF");
        } else {
            server.send(401, "text/plain", "Authentication Required");
        }
    });

    server.on("/switch2On", []() {
        digitalWrite(gpio2_pin, HIGH);
        server.send(200, "text/plain", "Switch 2 turned ON");
    });

    server.on("/switch2Off", []() {
        digitalWrite(gpio2_pin, LOW);
        server.send(200, "text/plain", "Switch 2 turned OFF");
    });

    server.on("/switch3On", []() {
        digitalWrite(gpio3_pin, HIGH);
        server.send(200, "text/plain", "Switch 3 turned ON");
    });

    server.on("/switch3Off", []() {
        digitalWrite(gpio3_pin, LOW);
        server.send(200, "text/plain", "Switch 3 turned OFF");
    });

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
