#include <Arduino.h>

constexpr uint8_t RADAR_OUT_PIN = 4;
constexpr uint8_t LORA_RX_PIN = 1;
constexpr uint8_t LORA_TX_PIN = 2;
constexpr uint32_t LORA_BAUD = 115200;

bool lastPresence = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RADAR_OUT_PIN, INPUT);
  Serial1.begin(LORA_BAUD, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  Serial.println("Radar + LoRa bridge started");
  Serial.println("Wiring: radar OUT -> GPIO4, LoRa UART -> GPIO1/GPIO2");
}

void loop() {
  const bool presence = digitalRead(RADAR_OUT_PIN) == HIGH;

  if (presence != lastPresence) {
    lastPresence = presence;

    Serial.print("Radar: ");
    Serial.println(presence ? "PRESENCE" : "CLEAR");

    Serial1.print("RADAR:");
    Serial1.println(presence ? "PRESENCE" : "CLEAR");
  }

  delay(200);
}
