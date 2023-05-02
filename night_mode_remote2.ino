
// Arduino code for esp32 implementing switching day and night mode by a toggle button, for mapfactor navigator, using its remote control features

// Include the WiFi library
#include <WiFi.h>

// Define the WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Define the server IP and port
const char* serverIP; // this will be obtained from DHCP server
const int serverPort = 4242; // change this to your server port

// Define the pin for the toggle button
const int buttonPin = 2;

// Define a variable to store the button state
int buttonState = 0;

// Define a variable to store the previous button state
int prevButtonState = 0;

// Define a variable to store the mode (0 for day, 1 for night)
int mode = 0;

// Define a WiFi client object
WiFiClient client;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the button pin as input with pullup resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Connect to WiFi network
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Get the gateway address from DHCP server
  serverIP = WiFi.gatewayIP().toString();
  
  // Connect to server
  Serial.println("Connecting to server...");
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected!");
  }
  else {
    Serial.println("Connection failed!");
    while (1); // stop the program
  }
}

void loop() {
  // Read the button state
  buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != prevButtonState) {
    // Check if the button was pressed (low state)
    if (buttonState == LOW) {
      // Toggle the mode
      mode = !mode;

      // Send a command to mapfactor navigator to switch mode
      if (mode == 0) {
        // Send a command to switch to day mode
        client.write("$set_mode=day\r\n");
        Serial.println("Switching to day mode");
      }
      else {
        // Send a command to switch to night mode
        client.write("$set_mode=night\r\n");
        Serial.println("Switching to night mode");
      }
    }
    // Update the previous button state
    prevButtonState = buttonState;
  }

  // Check if there is any data available from the server
  if (client.available()) {
    // Read and print the data
    String data = client.readStringUntil('\n');
    Serial.print("Received from server: ");
    Serial.println(data);
  }
}


//Source: Conversation with Bing, 5/2/2023
//(1) How to get current gateway IP address on ESP8266/ESP32. https://bing.com/search?q=how+to+get+gateway+address+from+dhcp+server+with+esp32.
//(2) How to get current gateway IP address on ESP8266/ESP32. https://techoverflow.net/2021/02/07/how-to-get-current-gateway-ip-address-on-esp8266-esp32/.
//(3) Arduino ESP32 DHCP server - Stack Overflow. https://stackoverflow.com/questions/56486774/arduino-esp32-dhcp-server.
//(4) [answered] how to get my IP address? - ESP32 Forum. https://www.esp32.com/viewtopic.php?t=5496.
