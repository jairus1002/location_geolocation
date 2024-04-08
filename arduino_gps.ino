#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Include ArduinoJson library for parsing JSON response

// Function to send HTTP POST request to Google Geolocation API
void sendLocationRequest() {
    HTTPClient http;
    http.begin("https://www.googleapis.com/geolocation/v1/geolocate?key=AIzaSyCK79DYIs-lQHQftTl5JXrhwxm-mKwd0Q0");
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"wifiAccessPoints\": ["
                     "{\"macAddress\": \"00:11:22:33:44:55\", \"signalStrength\": -50},"
                     "{\"macAddress\": \"AA:BB:CC:DD:EE:FF\", \"signalStrength\": -60}"
                     "]}";
    
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);

        // Parse JSON response to extract latitude and longitude
        DynamicJsonDocument jsonBuffer(1024);
        deserializeJson(jsonBuffer, response);
        float latitude = jsonBuffer["location"]["lat"];
        float longitude = jsonBuffer["location"]["lng"];

        // Send latitude and longitude to the server
        sendCoordinatesToServer(latitude, longitude);
    } else {
        Serial.print("Error on sending POST request: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

// Function to send latitude and longitude coordinates to the server
void sendCoordinatesToServer(float latitude, float longitude) {
    HTTPClient http;
    http.begin("http://192.168.8.6/new/gps.php"); // Modify URL according to your server's location
    http.addHeader("Content-Type", "application/json");

    // Construct JSON payload with latitude and longitude
    String payload = "{\"latitude\": " + String(latitude, 6) + ", \"longitude\": " + String(longitude, 6) + "}";

    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    } else {
        Serial.print("Error on sending coordinates to server: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    WiFi.begin("Gaffer", "12345678");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void loop() {
    sendLocationRequest();
    delay(60000); // Send location request every 1 minute
}
