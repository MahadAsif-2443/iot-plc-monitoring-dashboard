#include <ModbusMaster.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Define the RS485 direction control pin
#define RE_DE_PIN 4 

// Instantiate ModbusMaster object
ModbusMaster modbus;

// Instantiate AsyncWebServer on port 80
AsyncWebServer server(80);

// Array to store Modbus register values
uint16_t registerValues[5];

// Pre-transmission callback
void preTransmission() {
  digitalWrite(RE_DE_PIN, HIGH);
}

// Post-transmission callback
void postTransmission() {
  digitalWrite(RE_DE_PIN, LOW);
}

void setup() {
  Serial.begin(115200);

  // Initialize Modbus communication
  Serial2.begin(115200, SERIAL_8E1, 16, 17);
  pinMode(RE_DE_PIN, OUTPUT);
  digitalWrite(RE_DE_PIN, LOW);

  modbus.begin(1, Serial2);
  modbus.preTransmission(preTransmission);
  modbus.postTransmission(postTransmission);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // API endpoint to fetch Modbus data
  server.on("/modbus", HTTP_GET, [](AsyncWebServerRequest* request) {
    String json = "{";
    for (int i = 0; i < 5; i++) {
      json += "\"R" + String(i) + "\":" + String(registerValues[i]);
      if (i < 4) json += ",";
    }
    json += "}";
    request->send(200, "application/json", json);
  });

  // Start the server
  server.begin();
}

void loop() {
  // Read Modbus registers
  uint8_t modbusResult = modbus.readHoldingRegisters(0x0000, 5);
  if (modbusResult == modbus.ku8MBSuccess) {
     for (int i = 0; i < 5; i++) {
      registerValues[i] = modbus.getResponseBuffer(i); // Store in array
      Serial.print("Register R");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(registerValues[i]); // Print to Serial Monitor
    }
  } else {
    Serial.print("Modbus Error: ");
    Serial.println(modbusResult);
  }
  delay(1000); // Update data every second
}
