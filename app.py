from flask import Flask, render_template
from paho.mqtt import client as mqtt

app = Flask(__name__)

# Global variable to hold sensor data
sensor_data = {"temperature": 0, "humidity": 0, "motionDetected": 0}

# MQTT Settings
MQTT_BROKER = "broker.hivemq.com"
MQTT_TOPIC = "your_topic/weather"  # Use the same topic as in ESP32 code

# Callback function when a message is received
def on_message(client, userdata, message):
    global sensor_data
    payload = message.payload.decode('utf-8')
    print(f"Received message: {payload}")
    sensor_data = eval(payload)  # Convert string to dictionary

# MQTT setup
mqtt_client = mqtt.Client()
mqtt_client.on_message = on_message
mqtt_client.connect(MQTT_BROKER, 1883, 60)
mqtt_client.subscribe(MQTT_TOPIC)
mqtt_client.loop_start()

@app.route('/')
def index():
    return render_template('index.html', data=sensor_data)

if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=5000)
