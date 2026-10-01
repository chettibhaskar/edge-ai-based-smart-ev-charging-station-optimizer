#include "Peripherals.h"
#include "config.h"
#include "State.h"

DHT dht(DHT_PIN, DHT_TYPE);

float mapfloat(long x, long inMin, long inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}

void sample_sensor(void)
{
  int raw_current = analogRead(CURRENT_PIN);  // 0 to 4095
  int raw_voltage = analogRead(VOLTAGE_PIN);  // 0 to 4095

  // map voltage 0 to 250 V
  voltage = mapfloat(raw_voltage, 0, 4095, 0, 250);

  if (bayStatus == "CHARGING")
  {
    // map current 0 to 32 A
    current = mapfloat(raw_current, 0, 4095, 0, 32);
  }
  else
  {
    current = 0;
  }

  // calculate power
  power = voltage * current;

  // read temperature in Celsius
  float t = dht.readTemperature();
  if (!isnan(t)) temperature = t;
}

float recentAvgCurrent(void)
{
  float sum = 0;
  // read recent 5 current values
  for (int i = 0; i < 5; i++)
  {
    sum = sum + current;
  }
  return sum / 5;
}

bool plugin_flag_once = 1;
bool plugout_flag_once = 1;

void plug_status(void)
{
  bool pluginReading = digitalRead(BTN_PLUGIN);
  // detect the switch is pressed
  if (pluginReading == LOW && plugin_flag_once)
  {
    // session start
    sessionStartMs = millis();
    // plug in switch is pressed
    plugin_flag_once = 0;
    // change bay_status free to charging
    if (bayStatus == "FREE")
    {
      bayStatus = "CHARGING";
      Serial.println("Bay1 plugin detected,bay is charging");
      // turn on the relay
      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  if (pluginReading == HIGH)
  {
    plugin_flag_once = 1;
  }

  // plug out switch is pressed
  bool plugoutReading = digitalRead(BTN_PLUGOUT);
  // detect the switch is pressed
  if (plugoutReading == LOW && plugout_flag_once)
  {
    plugout_flag_once = 0;
    // change bay_status charging to free
    if (bayStatus == "CHARGING")
    {
      bayStatus = "FREE";
      Serial.println("Bay1 plugout detected,Bay is FREE");
    }
  }

  if (plugoutReading == HIGH)
  {
    plugout_flag_once = 1;
  }
}

void update_led_status(void)
{
  // if bay_status FREE turn on green led
  if (bayStatus == "FREE")
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
  }
  else
  {
    // if bay_status charging turn on yellow led
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
  }
}