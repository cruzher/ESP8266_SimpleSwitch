#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define WIFI_SSID            "ssid"
#define WIFI_PASS            "pass"
#define WIFI_HOSTNAME        "nodeXXXX"
#define MQTT_HOST            "127.0.0.1"
#define MQTT_TOPIC           "topic"
#define MQTT_CLIENTID        "nodeXXXX"
#define MQTT_USERNAME        "user"
#define MQTT_PASSWPRD        "pass"
#define LED_PIN              2

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.hostname(WIFI_HOSTNAME);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWPRD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(OUTPUT, LED_PIN);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_HOST, 1883); 
  
  if (!client.connected()) {
    reconnect();
  }
  
  char msg[3];
  snprintf (msg, 50, "on", 10);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(MQTT_TOPIC, msg);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

void loop() {}
