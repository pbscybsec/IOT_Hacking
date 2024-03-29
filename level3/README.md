# Level 3 Attack
In level 3, we will raise the level of security by encrypting user credentials, preventing attackers from exploiting them. However, in this case, the attacker will utilize the Fiddler tool for packet capturing and has the ability to replay the captured packet; specifically, the attacker will repeat the packet used to control the smart led.
This project sets up a web server on an ESP32 device for controlling GPIO pins via a web interface with authentication. It's ideal for simple home automation tasks, allowing users to control devices connected to the ESP32's GPIO pins remotely within a local network.

## Software Dependencies

- `WiFi.h` for connecting to the internet
- `WebServer.h` for creating the web server
- `ESPmDNS.h` for the MDNS responder

## Setup Instructions

1. **Prepare the ESP32 Board:**
   - Connect your devices (e.g., LEDs or relays) to the GPIO pins 14, 18, and 19 on the ESP32.

2. **Configure WiFi:**
   - Update the `ssid` and `password` in the code to match your WiFi network details.

3. **Upload the Code:**
   - Use the Arduino IDE or your preferred IDE to upload the code to your ESP32 board.

4. **Access the Web Server:**
   - Once the ESP32 is connected to your WiFi network, use a web browser to navigate to the ESP32's IP address.

## Code Overview
### Authentication

- The `is_authentified()` function checks for a specific cookie to verify if the user is authenticated.
- The `handleLogin()` function presents a login page and sets a cookie upon successful authentication.

### GPIO Control

- Functions `handleRoot()`, `handleLogin()`, and `handleNotFound()` manage the web server's responses to different URIs.
- GPIO pins are controlled via specific URIs (e.g., `/switch1On`, `/switch1Off`), which call functions to toggle the pins.

### Web Server and MDNS

- Initializes a `WebServer` object on port 80.
- Uses MDNS responder to allow easy access through `http://esp32.local` if supported by the client device.

## Example Usage

After logging in, users can control connected devices via the web interface. Each device corresponds to a button on the web page that sends a request to the ESP32 to toggle the state of the connected GPIO pin.
