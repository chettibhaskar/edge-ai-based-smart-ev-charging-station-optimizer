#include <ArduinoJson.h>
#include "telemetry.h"
#include "network.h"
#include "state.h"
#include "config.h"

void publicTelemetry()
{
// check if the device is connected to the cloud or not
if (!mqtt.connected())return;

//store the values as key value pair format
StaticJsonDocument <350> doc;
doc["bayId"] = BAY_ID;
doc["voltage"] = round(voltage * 10) / 10.0;
doc["current"] = round(current = 10) / 10.0;
doc["power"] = round(power * 10) / 10.0;
doc["temperature"] = round(temperature * 10) / 10.0;
doc["bayStatus"] = bayStatus;
// adding two more values to buffer
doc["predictedArrivalProb"] = round(predictedArrivalProb * 100)/100.0;
doc["predicteddurationMin"] = predictedDurationMin;
// add the load decision, throttle level, over load 
doc["throttlelevel"] = throttleLevel;
doc["loadDecision"] = loadDecision ;
doc["overloadActive"] =overloadActive;


char buffer[350];
serializeJson(doc, buffer);

//push the data to cloud   , topic , data -> buffer
mqtt.publish("v1/devices/me/telemetry", buffer);
Serial.print("[MQTT >>] ");
Serial.println(buffer);
}