# Embedded-IoT-Honeypot-for-Real-Time-Attack-Monitoring-using-SIEM
This project, the RNSIT Secure Gateway, is a hardware-integrated Honeypot designed to detect and log unauthorized access attempts into a Security Information and Event Management (SIEM) system. It combines low-level hardware programming (Assembly) with cloud-scale data analytics.

## 🎯 Problem Statement
IoT devices often lack:
- Centralized logging  
- Intrusion detection mechanisms  
- Real-time monitoring  

This makes them highly vulnerable to brute-force attacks, credential stuffing, and reconnaissance scans.

---

## 🛠️ Tech Stack
- **Hardware:** ESP32  
- **Firmware:** Arduino (C/C++)  
- **Low-Level Security:** Xtensa Inline Assembly  
- **Networking:** WiFiClientSecure (HTTPS)  
- **Data Format:** JSON (ArduinoJson)  
- **SIEM:** Splunk Cloud  

---

## ⚙️ System Architecture

Attacker → ESP32 Honeypot → JSON Logs → HTTPS → Splunk SIEM → Dashboard


---

## 🔥 Key Features

### 🕵️ IoT Honeypot
- Fake login web interface hosted on ESP32  
- Mimics a vulnerable IoT gateway  
- Captures:
  - Source IP address  
  - Username  
  - Password  
  - Timestamp  

### 🧠 Real-Time SIEM Integration
- Logs sent securely via HTTPS  
- Uses Splunk HTTP Event Collector (HEC)  
- Enables real-time attack monitoring  

### ⚡ Inline Assembly Security (Unique Feature)
- XOR-based memory obfuscation using Xtensa assembly  
- Prevents exposure of credentials in RAM  
- Demonstrates embedded-level security  

### 🎭 Deception Logic
- Always returns "Authentication Failed"  
- Forces attacker to retry multiple credentials  
- Helps collect richer attack data  

---

## 🔐 Security Design

This project follows a **Defense-in-Depth** approach:

- In-memory credential obfuscation  
- Secure HTTPS log transmission  
- Token-based SIEM ingestion  
- Anti-forensics at firmware level  

---

## 📊 Splunk Dashboard

The dashboard provides:
- Real-time attack monitoring  
- Login attempt count  
- Most attempted usernames/passwords  
- Source IP tracking  
- Time-based attack trends  

📁 Refer: `splunk/dashboard_screenshots`

---

## ▶️ How to Run

1. Clone the repository
2. Create config/secrets.h (refer secrets_example.h)
3. Upload firmware to ESP32 using Arduino IDE
4. Connect ESP32 to WiFi
5. Open ESP32 IP address in browser
6. Enter credentials (simulate attack)
7. View logs in Splunk dashboard


---

## 📸 Demo

(Add screenshots here)

---

## 🧪 Evaluation

- Successful capture of attack attempts  
- Detection of repeated login attempts (brute-force)  
- Real-time visualization in Splunk  
- Accurate log transmission  

---

## 👨‍💻 Authors

- Darshan N  

---

## 📄 License

This project is licensed under the MIT License.
