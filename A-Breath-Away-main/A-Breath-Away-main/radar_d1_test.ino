#include <Arduino.h>

constexpr uint8_t RADAR_OUT_PIN = 1;  // D1 on the SeedStudio/XIAO board

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RADAR_OUT_PIN, INPUT);
  Serial.println("Radar input ready on D1");
}

void loop() {
  const bool present = digitalRead(RADAR_OUT_PIN) == HIGH;
  Serial.println(present ? "PRESENCE" : "CLEAR");
  delay(500);
}
