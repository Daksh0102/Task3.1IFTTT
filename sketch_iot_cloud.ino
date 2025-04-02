#include <WiFiNINA.h>         // Library for WiFi connection
#include <ArduinoHttpClient.h> // Library for HTTP requests
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

// WiFi credentials
const char* ssid = "abc";  // Replace with your WiFi SSID
const char* password = "abcdefgh"; // Replace with your WiFi password

// IFTTT Webhook Key
const char* iftttKey = "btAr8rwB6ETgfSf4DdLHkX";  // Replace with your IFTTT Key
WiFiClient wifi;
HttpClient client = HttpClient(wifi, "maker.ifttt.com", 80);

// Light Sensor Setup
const int threshold = 800;       // Adjust this based on your sensor's readings
bool sunlightState = false;  // Track if sunlight is detected

void setup() {
    Serial.begin(9600);
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Wire.begin();
    lightMeter.begin();
    
}

void sendIFTTTRequest(const char* event) {
    String url = "/trigger/" + String(event) + "/with/key/" + iftttKey;
    client.get(url);
    Serial.println("IFTTT Request Sent: " + url);
}

void loop() {
    int lightValue = lightMeter.readLightLevel();  // Read light sensor value
    Serial.print("Light Sensor Value: ");
    Serial.println(lightValue);

    if (lightValue > threshold && !sunlightState) {
        sendIFTTTRequest("sunlight_detected");  // Trigger when sunlight appears
        sunlightState = true;
        Serial.println("Sunlight detected");
    } 
    else if (lightValue <= threshold && sunlightState) {
        sendIFTTTRequest("sunlight_not_found");  // Trigger when sunlight disappears
        sunlightState = false;
        Serial.println("Sunlight gone");
    }
    
    delay(1000);  // Check every 1 minute
}
