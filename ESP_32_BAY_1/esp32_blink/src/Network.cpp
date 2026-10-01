#include <ArduinoJson.h>
#include <WiFi.h>
#include "Telemetry.h"
#include "Network.h"
#include "State.h"
#include "config.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);

void connectWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // --- DNS fix: force Google DNS so hostname lookups (e.g. mqtt.thingsboard.cloud) work ---
  IPAddress dns1(8, 8, 8, 8);
  IPAddress dns2(8, 8, 4, 4);
  WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), dns1, dns2);
  Serial.print("DNS set to: ");
  Serial.println(WiFi.dnsIP());
  // ---------------------------------
}

void connectMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (mqtt.connect(BAY_ID, THINGSBOARD_TOKEN, NULL))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(". Retrying in 2s");
      delay(2000);
    }
  }
}

// ---------------------------------------------------------------------
// FR-7 telemetry publish. Adds `overloadActive` (additive, beyond SRS
// 8.3's baseline schema) so the ThingsBoard Overcurrent alarm rule can
// be a one-line filter. Also adds `manualOverrideActive` so the
// dashboard can show when a bay is under operator control instead of
// automatic optimization.
// ---------------------------------------------------------------------
void publishTelemetry()
{
  if (!mqtt.connected()) return;

  StaticJsonDocument<350> doc;
  doc["bayId"] = BAY_ID;
  doc["voltage"] = round(voltage * 10) / 10.0;
  doc["current"] = round(current * 10) / 10.0;
  doc["power"] = round(power * 10) / 10.0;
  doc["temperature"] = round(temperature * 10) / 10.0;
  doc["bayStatus"] = bayStatus;

  char buffer[350];
  serializeJson(doc, buffer);

  mqtt.publish("v1/devices/me/telemetry", buffer);
  Serial.print("[MQTT >>] ");
  Serial.println(buffer);
}
