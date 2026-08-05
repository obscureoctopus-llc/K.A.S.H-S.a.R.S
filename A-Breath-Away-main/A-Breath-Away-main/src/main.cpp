#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>

#define KASH_IDENTIFIER "K.A.S.H"
#ifndef RADAR_UART_RX_PIN
#define RADAR_UART_RX_PIN 16
#endif
#ifndef RADAR_UART_TX_PIN
#define RADAR_UART_TX_PIN 17
#endif
#ifndef RADAR_UART_BAUD
#define RADAR_UART_BAUD 9600
#endif
#ifndef CROWPANEL_UART_RX_PIN
#define CROWPANEL_UART_RX_PIN -1
#endif
#ifndef CROWPANEL_UART_TX_PIN
#define CROWPANEL_UART_TX_PIN -1
#endif
#ifndef CROWPANEL_UART_BAUD
#define CROWPANEL_UART_BAUD 115200
#endif

constexpr size_t kLineBufferSize = 96;
char lineBuffer[kLineBufferSize];
size_t lineLength = 0;

struct RadarReport {
  bool valid;
  bool hasTarget;
  int values[6];
  size_t valueCount;
};

void publishToCrowPanel(const RadarReport& report, const char* rawLine) {
  const bool present = report.hasTarget;
  char payload[kLineBufferSize + 64];
  snprintf(payload, sizeof(payload), "%s CROWPANEL|state=%s|raw=%s\n",
           KASH_IDENTIFIER,
           present ? "PRESENCE" : "CLEAR",
           rawLine ? rawLine : "");

  Serial.print(payload);

  if (CROWPANEL_UART_TX_PIN >= 0 && CROWPANEL_UART_RX_PIN >= 0) {
    static bool crowPanelSerialReady = false;
    if (!crowPanelSerialReady) {
      Serial2.begin(CROWPANEL_UART_BAUD, SERIAL_8N1, CROWPANEL_UART_RX_PIN, CROWPANEL_UART_TX_PIN);
      pinMode(CROWPANEL_UART_RX_PIN, INPUT);
      pinMode(CROWPANEL_UART_TX_PIN, OUTPUT);
      crowPanelSerialReady = true;
    }
    Serial2.print(payload);
  }
}

RadarReport parseLine(const char* line) {
  RadarReport report = {false, false, {0, 0, 0, 0, 0, 0}, 0};
  if (line == nullptr || *line == '\0') {
    return report;
  }

  char temp[kLineBufferSize];
  strlcpy(temp, line, sizeof(temp));

  char* token = strtok(temp, ",");
  if (token != nullptr && !isdigit(static_cast<unsigned char>(token[0])) && token[0] != '-' && token[0] != '+') {
    token = strtok(nullptr, ",");
  }

  while (token != nullptr && report.valueCount < 6) {
    report.values[report.valueCount++] = atoi(token);
    token = strtok(nullptr, ",");
  }

  if (report.valueCount == 0) {
    return report;
  }

  report.valid = true;
  report.hasTarget = false;
  for (size_t i = 0; i < report.valueCount; ++i) {
    if (report.values[i] > 0) {
      report.hasTarget = true;
      break;
    }
  }

  return report;
}

void resetLineBuffer() {
  lineLength = 0;
  lineBuffer[0] = '\0';
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(RADAR_UART_BAUD, SERIAL_8N1, RADAR_UART_RX_PIN, RADAR_UART_TX_PIN);
  pinMode(RADAR_UART_RX_PIN, INPUT);
  pinMode(RADAR_UART_TX_PIN, OUTPUT);
  resetLineBuffer();
  Serial.println(KASH_IDENTIFIER);
  Serial.println("MR60 UART parser active");
  Serial.println("Crow panel payloads will be emitted as CROWPANEL|state=...|raw=...");
}

void loop() {
  while (Serial1.available()) {
    const char c = static_cast<char>(Serial1.read());

    if (c == '\n' || c == '\r') {
      if (lineLength > 0) {
        lineBuffer[lineLength] = '\0';
        const RadarReport report = parseLine(lineBuffer);
        if (report.valid) {
          publishToCrowPanel(report, lineBuffer);
        }
        resetLineBuffer();
      }
    } else if (lineLength < (kLineBufferSize - 1)) {
      lineBuffer[lineLength++] = c;
    } else {
      resetLineBuffer();
    }
  }

  delay(10);
}
