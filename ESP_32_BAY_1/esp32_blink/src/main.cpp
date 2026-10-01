#include <Arduino.h>
#include <WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include  "optimization.h"



void setup()
{
  // initialise serial monitor
  Serial.begin(115200);
  // initialise sensor 
  dht.begin();
  //configure esp32 with real time
  configTime(0,0,"pool.ntp.org","time.nist.gov");

  //config peripherals pins
  pinMode(BTN_PLUGIN, INPUT_PULLUP);
  pinMode(BTN_PLUGOUT, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_GREEN , OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED , OUTPUT);

  //connect board to wifi
  connectWiFi();

  //configute to mqtt server
  mqtt.setServer(MQTT_SERVER, MQTT_PORT); //mqtt server addr of things and port number

  //connect board to cloud
  connectMQTT(); //TOKEN,device id
}


unsigned long now;
unsigned long last_print;

void loop()
{
    //listen to to the input request
    mqtt.loop();

    
   //push data every 5 seconds
   now = millis();
   if ((now - last_print) > 5000)
   {
     last_print = now;
     // read the data from the sensor //voltage, current, temperature, power, bay status
     sample_sensor();
     //publish the data
     publishTelemetry();
     // run AI to get prediction
     runEdgeAIInference();
     // decide load based on prection
     runOptimization();

   }
   plug_status();
   updateLeds();

   
}