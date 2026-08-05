# 1 "C:\\Users\\austi\\AppData\\Local\\Temp\\tmpcha021g7"
#include <Arduino.h>
# 1 "C:/work/vpserial/VirtualPilotSerial/VirtualPilotSerial.ino"
# 22 "C:/work/vpserial/VirtualPilotSerial/VirtualPilotSerial.ino"
#include <Arduino.h>

#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator mode is not selected in Tools->Zigbee mode"
#endif

#include <Zigbee.h>
#include <ZigbeePilotWireControl.h>

const uint16_t ZbeeEndPoint = 1;
const uint8_t button = BOOT_PIN;


ZigbeePilotWireControl zbPilot (ZbeeEndPoint);
void
setPilotWire (ZigbeePilotWireMode mode);
void setup();
void loop();
#line 38 "C:/work/vpserial/VirtualPilotSerial/VirtualPilotSerial.ino"
void
setPilotWire (ZigbeePilotWireMode mode) {

  static const char *const modeNames[] = {
    "OFF",
    "COMFORT",
    "ECO",
    "FROST",
    "COMFORT-1",
    "COMFORT-2"
  };
  Serial.printf ("Pilot Wire Mode: %s\n", modeNames[mode]);
}

void setup() {
  Serial.begin (115200);



  while (!Serial) {

    delay (100);
  }
  delay (2000);

  Serial.println ("Zigbee Virtual Pilot Wire Control starting...");


  pinMode (button, INPUT_PULLUP);


  zbPilot.onPilotWireModeChange (setPilotWire);


  zbPilot.begin ();
  zbPilot.enableNvs (true);


  Serial.println ("Adding ZigbeePilotWireControl endpoint to Zigbee Core");
  Zigbee.addEndpoint (&zbPilot);


  if (!Zigbee.begin (ZIGBEE_ROUTER)) {
    Serial.println ("Zigbee failed to start! Rebooting...");
    ESP.restart();
  }

  Serial.print ("Connecting to network");
  while (!Zigbee.connected()) {

    Serial.print (".");
    delay (500);
  }
  Serial.println ("\nZigbee connected to network.");

  if (zbPilot.reportAttributes()) {

    Serial.println ("Pilot Wire attributes reported");
  }
  else {

    Serial.println ("Failed to report Pilot Wire attributes");
  }
}

void loop() {


  if (digitalRead (button) == LOW) {

    delay (100);
    unsigned long t = millis();
    while (digitalRead (button) == LOW) {
      delay (50);
      if ( (millis() - t) > 3000) {

        Serial.println ("Resetting Zigbee to factory and rebooting in 1s.");
        delay (1000);
        Zigbee.factoryReset();
      }
    }

    t = millis() - t;
    Serial.printf ("Button pressed for %lu ms\n", t);
    if (t < 3000) {
      uint8_t mode = zbPilot.pilotWireMode();

      mode = (mode + 1) % PILOTWIRE_MODE_COUNT;
      zbPilot.setPilotWireMode (static_cast<ZigbeePilotWireMode> (mode));
    }
  }
  delay (100);
}