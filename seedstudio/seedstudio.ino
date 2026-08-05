#include <Arduino.h>

// Translating your sequential physical pin request into internal ESP32-C6 GPIOs
#define LORA_GND_PIN 0   // XIAO Pin D0 (GPIO 0) -> Digital Ground
#define LORA_RX_PIN 1    // XIAO Pin D1 (GPIO 1) -> Connects to LoRa TXD
#define LORA_TX_PIN 2    // XIAO Pin D2 (GPIO 2) -> Connects to LoRa RXD
#define LORA_RST_PIN 21  // XIAO Pin D3 (GPIO 21) -> Connects to LoRa RST
#define LORA_VDD_PIN 22  // XIAO Pin D4 (GPIO 22) -> Connects to LoRa VDD

void setup() {
  // 1. Initialize native USB-C debug monitoring for your computer screen
  Serial.begin(115200);
  delay(1500);
  Serial.println("Initializing Custom Sequential Pinout for RYLR998...");

  // 2. Set up the custom logic power pin (D4 / GPIO22) to feed 3.3V to the module
  pinMode(LORA_VDD_PIN, OUTPUT);
  digitalWrite(LORA_VDD_PIN, HIGH);

  // 3. Set up your software Ground sink reference pin (D0 / GPIO0)
  pinMode(LORA_GND_PIN, OUTPUT);
  digitalWrite(LORA_GND_PIN, LOW);

  // 4. Initialize your custom hardware serial channel mapping
  // Syntax: Serial1.begin(BaudRate, Config, RX_Pin, TX_Pin);
  Serial1.begin(115200, SERIAL_8N1, LORA_RX_PIN, LORA_TX_PIN);

  // 5. Fire up the custom Reset line (D3 / GPIO21) to boot up the radio cleanly
  pinMode(LORA_RST_PIN, OUTPUT);
  digitalWrite(LORA_RST_PIN, HIGH);
  delay(100);
  Serial.println("Pin remapping execution complete.");
}

void loop() {
  // Query the module's target network address over your new sequence line
  Serial1.println("AT+ADDRESS?");
  Serial.println("Querying: AT+ADDRESS?");

  delay(300);

  // Echo any reply strings caught back from the radio out to your PC Monitor
  while (Serial1.available()) {
    String reply = Serial1.readStringUntil('\n');
    Serial.print("Radio Output: ");
    Serial.println(reply);
  }

  delay(4000);  // Test the serial pipeline again every 4 seconds
}
