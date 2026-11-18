# IoT-Based PLC Monitoring Dashboard

A portfolio-ready source-code snapshot of an Industrial IoT monitoring project. An ESP32 reads Modbus RTU holding registers from a PLC and sends the values to Firebase Realtime Database. A browser dashboard uses Firebase Authentication and Realtime Database to display register values and a chart.

> **Project status:** This repository is a cleaned source-code snapshot. The original hardware setup is not currently connected, so this package has not been re-tested on a live PLC or Firebase project.

## Architecture

`PLC (Modbus RTU) -> ESP32 -> Firebase Realtime Database -> Web dashboard`

## Included

- `firmware/firebase_uploader/` — ESP32 firmware for reading PLC holding registers and writing values to Firebase.
- `firmware/legacy_local_server/` — an older ESP32 local web-server example, retained for reference only.
- `dashboard/` — Firebase-hosted HTML dashboard and Hosting configuration.

## Hardware and communication (from the project source)

- PLC: FATEK FBS series (original project hardware)
- Controller: ESP32
- Interface: TTL-to-RS485 transceiver
- Protocol: Modbus RTU
- ESP32 UART2 pins in the Firebase uploader: RX GPIO 16, TX GPIO 17
- RS485 driver enable/receiver enable: GPIO 4
- Serial configuration in the source: 115200 baud, 8E1
- Modbus slave ID in the source: 1

Check your exact PLC model, wiring, register map, and serial settings before using the firmware with hardware.

## Configure the firmware

1. Open `firmware/firebase_uploader/firebase_uploader.ino` in Arduino IDE.
2. Install the libraries used by the sketch: `ModbusMaster` and `Firebase ESP Client` (Mobizt).
3. Replace `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD` with your own Wi-Fi details locally. Do not commit real credentials.
4. Create/configure your own Firebase project, enable Email/Password Authentication, and create a Realtime Database.
5. Add your Firebase API key, database URL, user email, and user password in the firmware's configuration section. Keep account passwords private and do not commit them.
6. Confirm the Modbus register addresses and wiring for your PLC before uploading.

## Configure the dashboard

1. Open `dashboard/index.html`.
2. Replace the `YOUR_FIREBASE_...` placeholder values in the Firebase web configuration with the configuration for your own Firebase web app.
3. Enable Email/Password Authentication in Firebase Authentication and create a test user.
4. Set restrictive Realtime Database Rules so only authenticated, authorized users can read or write the data your application needs.
5. Deploy the `dashboard/` folder using Firebase Hosting after installing and configuring the Firebase CLI.

The dashboard is a static HTML page and loads Firebase and Chart.js libraries from their CDNs. An internet connection is required.

## Data path

The firmware source writes register values under the `/modbus/` path in Firebase Realtime Database. Review the exact field names in the sketch and make sure the dashboard's expected fields match before deployment.

## Important security notes

- Never use public read/write Firebase Database Rules for a real project.
- Firebase web configuration values are client-side configuration, not a substitute for database security. Database Rules and Authentication determine access.
- Do not publish Wi-Fi passwords, Firebase user passwords, service-account JSON files, private keys, or personal customer data.
- The `legacy_local_server` sketch exposes a local `/modbus` HTTP endpoint without authentication in its original form. It is included for code reference only; do not expose it to the public internet or deploy it as-is.
- The original archive contained private/project-specific material (documentation, presentation, media, PLC program, and Firebase deployment metadata). Those items are intentionally not included in this public-code package.

## Limitations

- This source snapshot has not been validated against live hardware or a live Firebase project during packaging.
- Verify the register mapping and dashboard field mapping before claiming a working deployment.
- The legacy local server sketch is separate from the Firebase uploader and is not part of the recommended data path.

## Suggested portfolio description

**Industrial IoT PLC Monitoring Dashboard** — Built an ESP32-based Modbus RTU data pipeline to collect PLC holding-register values, send telemetry to Firebase Realtime Database, and visualize live machine data in a web dashboard with Firebase Authentication.
