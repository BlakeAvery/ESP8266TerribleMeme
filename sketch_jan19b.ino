//TODO: DOCUMENT THIS HORSESHIT
//TODO: MAKE IT BETTER
//TODO: Add full network
#include <ESP8266WiFi.h>
const int timeout = 500;
void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.setTimeout(timeout); //We modify the timeout as needed.
  Serial.println();
  Serial.println("Chip ID: " + String(ESP.getChipId()));
  Serial.println("Core Version: " + String(ESP.getCoreVersion()));
  Serial.println("SDK Version: " + String(ESP.getSdkVersion()));
  Serial.println("CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz");
  Serial.println("Program size: " + String(ESP.getSketchSize()));
  Serial.println("Default timeout: " + String(timeout));
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Connect to Wi-Fi? 1: yes, 2: no");
  Serial.setTimeout(15000);
  int choice = Serial.readStringUntil('\n').toInt();
  Serial.println(String(choice));
  if(choice == 1) wifiSetupWizard();
  Serial.println("Ready for commands.");
  Serial.setTimeout(timeout);
}

void wifiSetupWizard() {
  String password = "\0";
  Serial.println("Scanning for networks...");
  int networksFound = WiFi.scanNetworks();
  for(int a = 0; a < networksFound; a++) {
    Serial.println(String(a) + ": " + WiFi.SSID(a) + " | Security: " + secType(WiFi.encryptionType(a)) + " | Signal: " + WiFi.RSSI(a)); //Imma keep it real with you chief I'm using this until i figure out printf
  }
  Serial.println("Select Wi-Fi network using index.");
  int b = Serial.readStringUntil('\n').toInt();
  if(WiFi.encryptionType(b) == 255) {
    Serial.println("This Wi-Fi encryption type is not supported.");
    return;
  }
  if(WiFi.encryptionType(b) != 7) {
    Serial.println("Enter password:");
    password = Serial.readStringUntil('\n');
  }
  String s = WiFi.SSID(b); //PLEASE COMPILE
  int len = s.length() + 1;
  const char* ssid/*[33]*/ = s.c_str(); // 33 is max ssid length if this doesnt fucking compile i will fuck my bottle
  const char* passwd = password.c_str();
  if(password != "\0") {
    WiFi.begin(ssid,passwd,WiFi.channel(b),WiFi.BSSID(b),true);
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      digitalWrite(LED_BUILTIN, LOW);
      delay(25);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(25);
    }
    delay(250);
    Serial.println("\nConnected. IP: " + WiFi.localIP());
  }
}

String secType(int sec) {
  switch(sec) {
    case 5:
      return "WEP (Wow, you're still using this?)";
      break;
    case 2:
      return "WPA-TKIP (WPA1)";
      break;
    case 4:
      return "WPA-CCMP (WPA2)";
      break;
    case 7:
      return "Open";
      break;
    case 8:
      return "Auto";
      break;
    default:
      return "Unknown/Unsupported";
      break;
  }
}

void echo() {
  Serial.setTimeout(15000);
  Serial.println("Echo init");
  delay(1000);
  String s = Serial.readStringUntil('\n');
  Serial.println(s);
  Serial.setTimeout(timeout);
}

void uptime() {
  unsigned long secs = millis() / 1000;
  Serial.println("\nUptime is " + String(secs) + " seconds");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  if(Serial.available() > 0) {
    int command = Serial.readStringUntil('\n').toInt();
    switch(command) {
      case 0:
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("ALERT! System going down for restart.");
        delay(500);
        ESP.restart();
      case 1:
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("System Information");
        Serial.println("Program Size: " + String(ESP.getSketchSize()) + " bytes");
        Serial.println("Core Version: " + String(ESP.getCoreVersion()));
        Serial.println("Chip ID: " + String(ESP.getChipId()));
        Serial.println();
        break;
      case 2:
        digitalWrite(LED_BUILTIN, HIGH);
        echo();
        break;
      case 3:
        digitalWrite(LED_BUILTIN, HIGH);
        for(int x = 0; x <= 500; x++) {
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println(String(x));
          delay(50);
          digitalWrite(LED_BUILTIN, HIGH);
          delay(50);
        }
        Serial.println("Done.");
        break;
      case 4:
        uptime();
        break;
      case 5:
        wifiSetupWizard();
        break;
      default:
        Serial.println("Command not found.");
    }
  }
}
