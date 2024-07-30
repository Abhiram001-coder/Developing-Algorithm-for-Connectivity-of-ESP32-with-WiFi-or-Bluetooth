<<<<<<< HEAD
#include <WiFi.h>
#include "BluetoothSerial.h"
#define WIFI_SSID "motowifi"
#define WIFI_PASSWORD "imironman"
//const int LED_PIN = 2; 
String device_name = "moto";
bool i=false;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void wifi_reinit() {
  // Check if WiFi is not connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reinitializing WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("WiFi Reinitialized");
  } else {
    Serial.println("WiFi is already connected");
  }
}

void bluetooth_reinit() {
  // Check if Bluetooth is not initialized or has no active client
  if (!SerialBT.isReady() || !SerialBT.hasClient()) {
    Serial.print("Reinitializing Bluetooth...");
    SerialBT.begin(device_name);
    Serial.println("Bluetooth Reinitialized");
  } else {
    Serial.println("Bluetooth is already initialized and connected");
  }
}

   

void setup(){
  Serial.begin(115200);
//  pinMode(LED_PIN,OUTPUT);
  //digitalWrite(LED_PIN,LOW);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected.");
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());
   SerialBT.begin(device_name);
}
void loop() {
  // Check the availability of WiFi and Bluetooth
  bool wifiAvailable = (WiFi.status() == WL_CONNECTED);
  bool btAvailable = SerialBT.hasClient(); // Check if Bluetooth is connected

  // Debugging outputs
  Serial.print("WiFi Available: ");
  Serial.println(wifiAvailable);
  Serial.print("Bluetooth Available: ");
  Serial.println(btAvailable);

  if (wifiAvailable && btAvailable) {
    // Case 1: Both WiFi and Bluetooth are available
    Serial.println("Both WiFi and Bluetooth are connected");
    SerialBT.end();
    Serial.println("WiFi is connected, bluetooth is disconnected, turning on led...");
   // digitalWrite(LED_PIN, HIGH);  // Turn on LED
  } else if (wifiAvailable){
    // Case 2: Either WiFi or Bluetooth is not available
    Serial.println("Only wifi is detected in this proximity and the device is connected to it");
    //digitalWrite(LED_PIN, LOW);  // Turn off LED
  }
  else if(btAvailable){
    Serial.println("Only bluetooth is detected in this proximity and the device is connected to it");
  }
  else if (!wifiAvailable && !btAvailable){
    Serial.println("No Connectivity...reinitializing");
    wifi_reinit();
    bluetooth_reinit();
    
    delay(5000);
    i=true;
   if(i){
    Serial.println("No connectivity..check you device had enough range to connect");
  }
  }
  //else if(i){
    //Serial.println("No connectivity..check you device had enough range to connect");
  //}

  delay(1000);  // Check every second
}

=======
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
>>>>>>> origin/main
