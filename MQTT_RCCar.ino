#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "test123";
const char* password = "12345678";

const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "hust/iot/rccar/control_v1";

WiFiClient espClient;
PubSubClient client(espClient);

int Speed = 255;
int enA = 23;
int enB = 5;
int IN1 = 19;
int IN2 = 18;
int IN3 = 22;
int IN4 = 21;

// Điều khiển động cơ với logic tín hiệu đã được đảo chuẩn theo phần cứng
void forward() {
  ledcWrite(enA, Speed); ledcWrite(enB, Speed);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  ledcWrite(enA, Speed); ledcWrite(enB, Speed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void left() {
  ledcWrite(enA, Speed); ledcWrite(enB, Speed);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void right() {
  ledcWrite(enA, Speed); ledcWrite(enB, Speed);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void stopCar() {
  ledcWrite(enA, 0); ledcWrite(enB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)payload[i];
  }

  // Logic đã được trả về chuẩn mực
  if (messageTemp == "F") forward();
  else if (messageTemp == "B") backward();
  else if (messageTemp == "L") left();
  else if (messageTemp == "R") right();
  else if (messageTemp == "S") stopCar();
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {     
      client.subscribe(mqtt_topic);
    } else {     
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  pinMode(enA, OUTPUT); pinMode(enB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  ledcAttach(enA, 5000, 8);
  ledcAttach(enB, 5000, 8);

  stopCar();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}