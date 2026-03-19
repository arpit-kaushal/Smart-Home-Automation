#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"
#include <BlynkSimpleEsp8266.h>
#include <map>

// ---------------- WiFi Credentials ----------------
#define WIFI_SSID   "YOUR-WIFI-NAME"
#define WIFI_PASS   "YOUR-WIFI-PASSWORD"

// ---------------- SinricPro ----------------
#define APP_KEY     "YOUR-APP-KEY"
#define APP_SECRET  "YOUR-APP-SECRET"

// ---------------- Blynk ----------------
char auth[] = "YOUR-BLYNK-AUTH";

// ---------------- Device IDs ----------------
#define device_ID_1   "SWITCH_ID_1"
#define device_ID_2   "SWITCH_ID_2"
#define device_ID_3   "SWITCH_ID_3"
#define device_ID_4   "SWITCH_ID_4"

// ---------------- Relay Pins ----------------
#define RelayPin1 5   // D1
#define RelayPin2 4   // D2
#define RelayPin3 14  // D5
#define RelayPin4 12  // D6

// ---------------- Switch Pins ----------------
#define SwitchPin1 10 // SD3
#define SwitchPin2 0  // D3
#define SwitchPin3 13 // D7
#define SwitchPin4 3  // RX

#define wifiLed 16   // D0

#define DEBOUNCE_TIME 250

// ---------------- Structures ----------------
typedef struct {
  int relayPIN;
  int flipSwitchPIN;
  int blynkPIN;
} deviceConfig_t;

std::map<String, deviceConfig_t> devices = {
  {device_ID_1, {RelayPin1, SwitchPin1, V1}},
  {device_ID_2, {RelayPin2, SwitchPin2, V2}},
  {device_ID_3, {RelayPin3, SwitchPin3, V3}},
  {device_ID_4, {RelayPin4, SwitchPin4, V4}}
};

typedef struct {
  String deviceId;
  bool lastState;
  unsigned long lastChange;
} switchConfig_t;

std::map<int, switchConfig_t> switches;

// ---------------- Setup Relays ----------------
void setupRelays() {
  for (auto &device : devices) {
    pinMode(device.second.relayPIN, OUTPUT);
    digitalWrite(device.second.relayPIN, HIGH);
  }
}

// ---------------- Setup Switches ----------------
void setupSwitches() {
  for (auto &device : devices) {
    switchConfig_t sw;
    sw.deviceId = device.first;
    sw.lastState = true;
    sw.lastChange = 0;

    int pin = device.second.flipSwitchPIN;
    switches[pin] = sw;

    pinMode(pin, INPUT_PULLUP);
  }
}

// ---------------- Sinric Callback ----------------
bool onPowerState(String deviceId, bool &state) {
  int relayPIN = devices[deviceId].relayPIN;
  digitalWrite(relayPIN, !state);

  // Sync Blynk
  Blynk.virtualWrite(devices[deviceId].blynkPIN, state);

  Serial.printf("Device %s turned %s\n", deviceId.c_str(), state ? "ON" : "OFF");
  return true;
}

// ---------------- Handle Manual Switch ----------------
void handleSwitches() {
  unsigned long now = millis();

  for (auto &sw : switches) {
    int pin = sw.first;

    if (now - sw.second.lastChange > DEBOUNCE_TIME) {
      bool state = digitalRead(pin);

      if (state != sw.second.lastState) {
        sw.second.lastChange = now;

        String deviceId = sw.second.deviceId;
        int relayPIN = devices[deviceId].relayPIN;

        bool newState = !digitalRead(relayPIN);
        digitalWrite(relayPIN, newState);

        // Sync Sinric
        SinricProSwitch &mySwitch = SinricPro[deviceId];
        mySwitch.sendPowerStateEvent(!newState);

        // Sync Blynk
        Blynk.virtualWrite(devices[deviceId].blynkPIN, !newState);

        sw.second.lastState = state;
      }
    }
  }
}

// ---------------- Blynk Control ----------------
BLYNK_WRITE(V1) {
  digitalWrite(RelayPin1, !param.asInt());
}
BLYNK_WRITE(V2) {
  digitalWrite(RelayPin2, !param.asInt());
}
BLYNK_WRITE(V3) {
  digitalWrite(RelayPin3, !param.asInt());
}
BLYNK_WRITE(V4) {
  digitalWrite(RelayPin4, !param.asInt());
}

// ---------------- WiFi ----------------
void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(wifiLed, LOW);
  Serial.println("\nWiFi Connected");
}

// ---------------- Sinric Setup ----------------
void setupSinric() {
  for (auto &device : devices) {
    SinricProSwitch &mySwitch = SinricPro[device.first];
    mySwitch.onPowerState(onPowerState);
  }

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);

  pinMode(wifiLed, OUTPUT);
  digitalWrite(wifiLed, HIGH);

  setupRelays();
  setupSwitches();

  setupWiFi();
  Blynk.begin(auth, WIFI_SSID, WIFI_PASS);

  setupSinric();
}

// ---------------- Loop ----------------
void loop() {
  SinricPro.handle();
  Blynk.run();
  handleSwitches();
}
