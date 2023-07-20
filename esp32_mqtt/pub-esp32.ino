#include <WiFi.h>
#include <PubSubClient.h>

// WiFi configuration
char* ssid     = "your_network_id";
char* password = "your_network_password";
// MQTT configuration
char* broker   = "your_mqtt_broker_id";
int mqtt_port  = 1883;
char* mqtt_username = "your_mqtt_username";
char* mqtt_password = "your_mqtt_password";

char* topic = "your_topic";
char* message = "your_message";

// Create a object to set ESP-32 to mqtt client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt();
}

void loop() {
  client.loop();

  // publish every 5 seconds
  if (millis() - lastMsg > 5000){
    lastMsg = millis();
    client.publish(topic, message);
    Serial.println("Message published: ");
    Serial.print(message);
  }

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic: ");
  Serial.print(topic);
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.print("Message arrived: ");
  Serial.println(message);
}

void setup_mqtt() {
  delay(10);
  client.setServer(broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()){
    delay(100);
    Serial.println();
    Serial.print("Connecting to MQTT Broker");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)){
      Serial.println("Connected to Broker!");
    }
    else {
      Serial.println("Failed to connect to Broker!");
      Serial.print("Broker state: ");
      Serial.print(client.state());
      Serial.println();
      Serial.println("Retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void setup_wifi() {
  delay(100);
  // serial feedback
  Serial.println();
  Serial.print("Connecting WiFi to: ");
  Serial.println(ssid);
  
  // connect to network
  WiFi.begin(ssid, password);
  
  // check status loop
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // After connected into network
  Serial.println();
  Serial.println("WiFi connected");
  
  // Show device IP
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}
