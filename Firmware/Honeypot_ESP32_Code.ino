#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "Secrets.h"

// 1. PROJECT CONFIGURATION (SECURED)
//const char* ssid = "YOUR_WIFI_SSID";
//const char* password = "YOUR_WIFI_PASSWORD";

//const char* splunk_url = "YOUR_SPLUNK_URL";
//const char* splunk_token = "YOUR_SPLUNK_TOKEN";

WebServer server(80);

// 2. ASSEMBLY LOGIC: RAM Obfuscation
void obfuscateInMemory(char* data, int len) {
  for (int i = 0; i < len; i++) {
    char* ptr = &data[i];
    asm volatile (
      "l8ui  a2, %0, 0\n\t"  
      "xor   a2, a2, %1\n\t" 
      "s8i   a2, %0, 0\n\t"  
      : "+r"(ptr)       
      : "r"(0xAA)            
      : "a2"                 
    );
  }
}

// 3. RNSIT SECURE GATEWAY UI GENERATOR
String getLoginPage(bool isError = false) {
  String errorMsg = isError ? 
    "<div style='color: #f87171; background: rgba(220, 38, 38, 0.15); padding: 12px; border-radius: 8px; margin-bottom: 20px; border: 1px solid #ef4444; font-size: 13px; font-weight: 500;'>Authentication Failed: Invalid Security Token.</div>" : "";
  
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <style>
        body { font-family: 'Segoe UI', Tahoma, sans-serif; background-color: #0b0e14; color: #d1d5db; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
        .gateway-box { background: #1f2937; padding: 50px 40px; border-radius: 12px; border-top: 4px solid #3b82f6; width: 360px; text-align: center; box-shadow: 0 25px 50px -12px rgba(0,0,0,0.6); }
        .logo { font-size: 12px; color: #60a5fa; font-weight: bold; letter-spacing: 3px; margin-bottom: 8px; text-transform: uppercase; }
        h2 { margin: 0 0 30px 0; font-size: 24px; color: #ffffff; font-weight: 500; }
        input { width: 100%; padding: 14px; margin: 10px 0; border: 1px solid #374151; border-radius: 8px; background: #111827; color: white; box-sizing: border-box; font-size: 14px; }
        input:focus { border-color: #3b82f6; outline: none; box-shadow: 0 0 0 2px rgba(59, 130, 246, 0.2); }
        input[type='submit'] { background: #2563eb; color: white; border: none; font-weight: 600; cursor: pointer; transition: 0.2s; margin-top: 20px; letter-spacing: 1px; }
        input[type='submit']:hover { background: #1d4ed8; }
        .footer { font-size: 10px; color: #4b5563; margin-top: 40px; border-top: 1px solid #374151; padding-top: 15px; }
    </style>
    <title>RNSIT | Secure Network Gateway</title>
</head>
<body>
    <div class='gateway-box'>
        <div class='logo'>RNSIT - Dept of CSE-CY</div>
        <h2>Secure Gateway v4.2</h2>
)rawliteral";

  html += errorMsg;

  html += R"rawliteral(
        <form action='/login' method='POST'>
            <input type='text' name='user' placeholder='Administrator ID' required>
            <input type='password' name='pass' placeholder='Security Access Token' required>
            <input type='submit' value='AUTHENTICATE'>
        </form>
        <div class='footer'>Node ID: RNSIT-GW-01 | AES-256 Session Active</div>
    </div>
</body>
</html>
)rawliteral";
  
  return html;
}

// 4. LOGGING & RECURSIVE REDIRECT LOGIC
void handleLogin() {
    String user = server.arg("user");
    String pass = server.arg("pass");
    String ip = server.client().remoteIP().toString();

    // 1. RAM Security: Obfuscate password immediately
    char* passArr = (char*)pass.c_str();
    obfuscateInMemory(passArr, pass.length());
    Serial.println("\n[SECURE] RAM Data Obfuscated.");

    // 2. Prepare JSON for Splunk
    obfuscateInMemory(passArr, pass.length()); // De-obfuscate for SIEM
    StaticJsonDocument<512> doc;
    JsonObject event = doc.createNestedObject("event");
    event["type"] = "gateway_intrusion_attempt";
    event["source_ip"] = ip;
    event["username"] = user;
    event["password"] = pass;
    event["node_name"] = "RNSIT_Secure_Gateway";
    
    String jsonStr;
    serializeJson(doc, jsonStr);

    // 3. Send to SIEM via Secure HTTPS
    WiFiClientSecure client;
    client.setInsecure(); 
    HTTPClient https;

    if (https.begin(client, splunk_url)) {
        https.addHeader("Authorization", splunk_token);
        https.addHeader("Content-Type", "application/json");
        int httpCode = https.POST(jsonStr);
        Serial.printf("[SIEM] Transmission Success. Code: %d\n", httpCode);
        https.end();
    }

    // 4. REDIRECT: Reload the page with the error message
    server.send(200, "text/html", getLoginPage(true));
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }

    server.on("/", []() { server.send(200, "text/html", getLoginPage(false)); });
    server.on("/login", HTTP_POST, handleLogin);
    
    server.begin();
    Serial.println("\n--- RNSIT SECURE GATEWAY OPERATIONAL ---");
    Serial.print("Access URL: http://");
    Serial.println(WiFi.localIP());
}

void loop() {
    server.handleClient();
}
