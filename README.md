# 🏠 Smart Home Automation

Control your home appliances using **Google Assistant, Amazon Alexa, Blynk App, and manual switches** — all in one system.

This project integrates **NodeMCU ESP8266**, **Sinric Pro**, and **Blynk** to build a powerful IoT-based home automation system.



## 📸 Project Preview

### 🔌 Circuit Diagram
![Circuit Diagram](images/circuit.png)

### 🧪 Prototype Setup
![Prototype Setup](images/prototype.jpg)

> 📌 Place your images inside an `images` folder in your repository.



## 🚀 Features

- 📱 Control appliances from **Blynk mobile app**
- 🎙️ Voice control via **Google Assistant & Alexa**
- 🔘 Manual control using physical switches
- 🌐 Remote access from anywhere via internet
- 🔄 Real-time synchronization across all platforms
- ⚡ Works even without internet (manual switches)



## 🛠️ Tech Stack

- **Hardware:** NodeMCU ESP8266, Relay Module  
- **IoT Platforms:** Sinric Pro, Blynk  
- **Programming:** Arduino IDE (C++)  
- **Connectivity:** WiFi  



## 🔌 Hardware Components

- NodeMCU ESP8266  
- 4-Channel Relay Module (5V)  
- Push Buttons / Switches  
- Jumper Wires  
- 5V Power Supply  



## ⚙️ Pin Configuration

| Device        | GPIO | NodeMCU Pin |
|--------------|------|------------|
| Relay 1      | 5    | D1         |
| Relay 2      | 4    | D2         |
| Relay 3      | 14   | D5         |
| Relay 4      | 12   | D6         |
| Switch 1     | 10   | SD3        |
| Switch 2     | 0    | D3         |
| Switch 3     | 13   | D7         |
| Switch 4     | 3    | RX         |
| WiFi LED     | 16   | D0         |


## 📲 Platform Setup

### 🔹 Sinric Pro Setup
1. Create an account on **Sinric Pro**
2. Add 4 switch devices
3. Copy:
   - APP KEY  
   - APP SECRET  
   - DEVICE IDs  



### 🔹 Blynk Setup
1. Install **Blynk App**
2. Create a new project → Select **NodeMCU**
3. Add 4 buttons:
   - V1 → Relay 1  
   - V2 → Relay 2  
   - V3 → Relay 3  
   - V4 → Relay 4  
4. Copy the **Auth Token**



## 💻 Code Configuration

Update the following credentials in your code:

```cpp
#define WIFI_SSID   "YOUR-WIFI-NAME"
#define WIFI_PASS   "YOUR-WIFI-PASSWORD"

#define APP_KEY     "YOUR-APP-KEY"
#define APP_SECRET  "YOUR-APP-SECRET"

char auth[] = "YOUR-BLYNK-AUTH";

#define device_ID_1 "SWITCH_ID_1"
#define device_ID_2 "SWITCH_ID_2"
#define device_ID_3 "SWITCH_ID_3"
#define device_ID_4 "SWITCH_ID_4"

```
## 🔄 Working Principle

- 📡 NodeMCU connects to WiFi

- ☁️ Sinric Pro enables Alexa & Google Assistant control

- 📱 Blynk enables mobile app control

- 🔘 Manual switches provide offline control

- 🔁 All platforms stay synchronized in real-time

## 🧠 Functional Overview

1. 🎙️ Voice Control

- “Alexa, turn ON light”

- “Hey Google, turn OFF fan”

2. 📱 App Control

- Control appliances using Blynk buttons (V1–V4)

3. 🔘 Manual Control

- Physical switches work even without internet

## 📂 Project Structure
```
Home-Automation/
│── code.ino
│── README.md
│── images/
│   ├── circuit.png
│   └── prototype.jpg

```
