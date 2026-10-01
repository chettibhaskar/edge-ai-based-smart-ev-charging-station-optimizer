#pragma once

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""
#define MQTT_SERVER "thingsboard.cloud"
#define MQTT_PORT 1883
#define BAY_ID "BAY_02"
#define THINGSBOARD_TOKEN "yRNlkSeSDynBHOaapdJR"

// Pins
#define VOLTAGE_POT_PIN 34
#define CURRENT_POT_PIN 35
#define PLUG_BUTTON_PIN 32 // START
#define STOP_BUTTON_PIN 33 // STOP
#define RED_LED_PIN 21 // OVERLOAD
#define YELLOW_LED_PIN 19 // CHARGING
#define GREEN_LED_PIN 18 // FREE
#define RELAY_PIN 26
#define DHT_PIN 15

// Thresholds
#define VOLTAGE_OVERLOAD 260.0
#define CURRENT_OVERLOAD 32.0

// Names the .cpp files use
#define VOLTAGE_PIN  VOLTAGE_POT_PIN
#define CURRENT_PIN  CURRENT_POT_PIN
#define BTN_PLUGIN   PLUG_BUTTON_PIN
#define BTN_PLUGOUT  STOP_BUTTON_PIN
#define LED_RED      RED_LED_PIN
#define LED_YELLOW   YELLOW_LED_PIN
#define LED_GREEN    GREEN_LED_PIN

#define DHT_TYPE     DHT22

// Used by optimization.cpp
const float overloadCurrentA = 28.0;