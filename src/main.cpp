#include <WiFi.h>
#include <BluetoothSerial.h>

#define WIFI_SSID "motowifi"
#define WIFI_PASSWORD "imironman"
#define LED_PIN 2  // Built-in LED pin on ESP32

BluetoothSerial SerialBT;
const String device_name = "moto";

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is off initially

  // Initialize Bluetooth
  if (!SerialBT.begin(device_name)) {
    Serial.println("Bluetooth initialization failed");
  } else {
    Serial.println("Bluetooth initialized");
    SerialBT.end();  // Initially, turn off Bluetooth
  }
  //Priority-2 Initialize the wifi and connect to it
  // Initialize WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Turn on LED when WiFi is connected
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  bool wifiAvailable = (WiFi.status() == WL_CONNECTED);
  bool btAvailable = SerialBT.hasClient(); // Check if Bluetooth is connected

  if (wifiAvailable) {
    if (btAvailable) {
      // Priority 3: Both WiFi and Bluetooth are available
      SerialBT.end();  // Disconnect Bluetooth
      Serial.println("WiFi is connected, Bluetooth disconnected");
    }
    // Turn on LED when WiFi is connected
    digitalWrite(LED_PIN, HIGH);
  } else {
    if (btAvailable) {
      // Priority 1: Only Bluetooth is available
      Serial.println("Bluetooth is available, connected");
      // Ensure Bluetooth is on
    } else {
      // Neither WiFi nor Bluetooth is available
      Serial.println("Neither WiFi nor Bluetooth is available");
      // Turn off LED
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Manage Bluetooth connection based on WiFi availability
  if (!wifiAvailable && !btAvailable) {
    // If WiFi is not available and Bluetooth is not connected
    if (!SerialBT.isReady()) {
      SerialBT.begin(device_name);  // Turn on Bluetooth
      Serial.println("Bluetooth is now discoverable");
    }
    // Turn off LED when WiFi is disconnected
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);  // Check every second
}