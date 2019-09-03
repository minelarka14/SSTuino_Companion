/*
  Adafruit IO with MQTT

  For the SSTuino boards.

  This example sketch subscribes to Adafruit OP using MQTT.
  This can be a foundation to use for automation over the Internet.

  This example code is in the public domain.

  https://d3lta-v.github.io/SSTuino/
*/

#include "SSTuino_Companion.h"

#define SSID          "SSID GOES HERE"
#define PASSWORD      "WIFI PASSWORD GOES HERE"
#define IO_USERNAME   "AIO USERNAME GOES HERE"
#define IO_KEY        "AIO KEY GOES HERE"
#define FEED_KEY      "FEED KEY GOES HERE"

SSTuino wifi = SSTuino();

String receivedRawData = "    ";

void setup()
{
  Serial.begin(9600);

  // Open the link between the two devices
  wifi.openLink();

  // Verify that the link is ok between the two devices
  if (!wifi.smokeTest()) {
    Serial.println(F("Unable to establish link with Wi-Fi chip. Halting.")); 
    while (true){};
  }

  wifiConnect();

  setupMQTT();

  /*
    Insert your setup code here
  */
}

void loop()
{
  /*
    Insert your loop code here, and you can do more processing with receivedRawData
  */

  if (wifi.mqttNewDataArrived(F(IO_USERNAME "/feeds/" FEED_KEY))) {
    receivedRawData = wifi.mqttGetSubcriptionData(F(IO_USERNAME "/feeds/" FEED_KEY));
    Serial.print(F("New data available: "));
    Serial.println(receivedRawData);
  } else {
    Serial.print(F("No new data available, current data is: "));
    Serial.println(receivedRawData);
  }

  delay(500); // half second interval
}

void wifiConnect(void)
{
  // Connects to Wifi and displays connection state
  wifi.connectToWifi(F(SSID), F(PASSWORD));
  Serial.println(F("Connecting to Wi-Fi..."));

  delay(10000); // 10 seconds optimal for wifi connection to fully establish

  Status wifiStatus = wifi.getWifiStatus();
  if (wifiStatus != SUCCESSFUL) {
    Serial.println(F("Failed to connect to Wi-Fi"));
    while (true){};
  } else {
    Serial.println(F("Wi-Fi connected"));
  }
}

void setupMQTT(void)
{
  // Setup MQTT
  Serial.println(F("Setting up MQTT..."));
  bool mqttSuccess = wifi.enableMQTT(F("io.adafruit.com"), true, IO_USERNAME, IO_KEY);
  if (!mqttSuccess) {
    Serial.println(F("Failed to enable MQTT. Halting."));
    while (true){};
  }
  delay(10000); // Wait for MQTT to fully connect

  // Check if MQTT is connected
  if (!wifi.isMQTTConnected()) {
    Serial.println(F("MQTT did not connect successfully!"));
    while (true){};
  } else {
    Serial.println(F("MQTT connected!"));
  }

  // Setup MQTT subscribe
  if (wifi.mqttSubscribe(F(IO_USERNAME "/feeds/" FEED_KEY))) {
    Serial.println(F("Successfully subscribed!"));
  } else {
    Serial.println(F("Failed to subscribe. Halting."));
    while (true){};
  }
}
