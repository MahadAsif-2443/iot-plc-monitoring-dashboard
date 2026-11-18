#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ModbusMaster.h>

// Your Wi-Fi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase project credentials
#define API_KEY ""
#define DATABASE_URL ""
#define USER_EMAIL ""
#define USER_PASSWORD ""

// Define UART pins for RS485 communication
#define RXD2 16  // GPIO16 (RX)
#define TXD2 17  // GPIO17 (TX)

// Create instances
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
ModbusMaster node;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void preTransmission() {
  digitalWrite(4, 1); // Assuming DE/RE connected to GPIO4
}

void postTransmission() {
  digitalWrite(4, 0);
}

void setup() {
  Serial.begin(115200);

  // Set DE/RE pin as output for RS485
  pinMode(4, OUTPUT);
  digitalWrite(4, 0); // Enable receive initially

  // Init Modbus communication on Serial2 (UART2)
  Serial2.begin(115200, SERIAL_8E1, RXD2, TXD2);
  node.begin(1, Serial2); // Slave ID 1
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConnected to Wi-Fi");

  // Firebase config
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Faster polling (1s instead of 5s)
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Read 5 Modbus registers (0 to 4)
    uint8_t result = node.readHoldingRegisters(0x0000, 6);
    if (result == node.ku8MBSuccess) {
      uint16_t val0 = node.getResponseBuffer(0);
      uint16_t val1 = node.getResponseBuffer(1);
      uint16_t val2 = node.getResponseBuffer(2);
      uint16_t val3 = node.getResponseBuffer(3);
      uint16_t val4 = node.getResponseBuffer(4);
      uint16_t val5 = node.getResponseBuffer(5);
      

      Serial.println("Register values:");
      Serial.printf("register0: %u\n", val0);
      Serial.printf("register1: %u\n", val1);
      Serial.printf("register2: %u\n", val2);
      Serial.printf("register3: %u\n", val3);
      Serial.printf("register4: %u\n", val4);
      Serial.printf("register5: %u\n", val5);

      String basePath = "/modbus/";
      Firebase.RTDB.setInt(&fbdo, basePath + "register0", val0);
      Firebase.RTDB.setInt(&fbdo, basePath + "register1", val1);
      Firebase.RTDB.setInt(&fbdo, basePath + "register2", val2);
      Firebase.RTDB.setInt(&fbdo, basePath + "register3", val3);
      Firebase.RTDB.setInt(&fbdo, basePath + "register4", val4);
      Firebase.RTDB.setInt(&fbdo, basePath + "register5", val5);


      if (fbdo.httpCode() == 200) {
        Serial.println("Data uploaded to Firebase successfully.\n");
      } else {
        Serial.println("Firebase upload failed: " + fbdo.errorReason());
      }
    } else {
      Serial.println("Modbus read failed.");
    }
  }
}
