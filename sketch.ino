//ESP32 Code
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Yagya's Oplus"; // Your WiFi SSID
const char* password = "qxrv2202";   // Your WiFi password

const char* mqtt_broker = "broker.hivemq.com"; // Public MQTT broker
const char* mqtt_topic = "your_topic/weather"; // Change this to your topic

WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor setup
#define DHTPIN 15     // DHT22 data pin connected to GPIO 15

// Relay setup
#define RELAY_PIN 26  // Relay control pin connected to GPIO 26

// PIR Motion Sensor setup
#define PIR_PIN 23    // PIR output pin connected to GPIO 23

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");

    client.setServer(mqtt_broker, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Read temperature and humidity from DHT sensor
    float temperature = 25.0; // Placeholder value, replace with actual DHT reading
    float humidity = 50.0;    // Placeholder value, replace with actual DHT reading
    
    // Read motion status
    int motionDetected = digitalRead(PIR_PIN);

    // Create a JSON message
    String message = String("{\"temperature\":") + temperature + 
                     String(",\"humidity\":") + humidity + 
                     String(",\"motionDetected\":") + motionDetected + "}";
    
    Serial.println("Publishing: " + message);
    client.publish(mqtt_topic, message.c_str());

    delay(2000); // Delay for stability
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            delay(2000);
        }
    }
}
