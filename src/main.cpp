#include <WiFi.h>
#include "BluetoothSerial.h"
#define WIFI_SSID "motowifi"
#define WIFI_PASSWORD "imironman"
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
  

  delay(1000);  // Check every second
}


