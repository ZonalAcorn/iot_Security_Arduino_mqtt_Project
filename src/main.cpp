#include <Arduino.h>
#include <ArduinoJson.h>
#include <Arduino_Sensorkit.h>
#include <ArduinoMqttClient.h>
#include <WiFiS3.h>


const char* ssid = "IoTProgram";
const char* password = "InternetOfThingsProgram";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);



// ========== DHT CODE ==========
#ifdef USE_DHT

#define BUZZER_PIN 5
#define BUTTON 4
#define Environment Environment_I2C


const char broker[] = "192.168.1.190";
int        port     = 1885;
const char mqttUser[] = "admin";
const char mqttPass[] = "admin";
const char topic[]  = "iot/Data/Temp";
const char topic2[] = "iot/Data/Humidity";
const char topic3[]  = "iot/Data/Buzzer";
const char topic4[]  = "iot/Control/Buzzer";

const long interval = 1000;
unsigned long previousMillis = 0;


void onMqttMessage(int messageSize) {
  //we recieve a message, print out topic and contents
  Serial.print("Recieved a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.println("'");
  Serial.print("Length: ");
  Serial.println(messageSize);
    String payload = "";
  while (mqttClient.available()) {
    payload += (char)mqttClient.read();
  }
  if (payload == "1") {
    tone(BUZZER_PIN, 2000);
  }
  else {
    noTone(BUZZER_PIN);
  }
  mqttClient.beginMessage(topic3);
  mqttClient.print(digitalRead(BUZZER_PIN));
  mqttClient.endMessage();
}

  

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Oled.begin();
  Oled.setFlipMode(true);
  Wire.begin();
  Serial.begin(9600);
  Environment.begin();
  WiFi.config(IPAddress(192,168,1,191));
  status = WiFi.begin(ssid, password);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
  }
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0,0);
  Oled.print("Connecting to:");
  Oled.setCursor(0,10);
  Oled.print(ssid);
  Oled.setCursor(0,30);
  Oled.print("IP:");
  Oled.print("192.168.1.191");
  delay(5000);
  server.begin();
  Oled.clear();
  Oled.setCursor(0,10);
  Oled.print("Connected");
  Oled.setCursor(0,20);
  Oled.print(WiFi.status());
  delay(2000);
  Oled.clear();
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setId("R4-1");
  mqttClient.setUsernamePassword(mqttUser, mqttPass);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic4);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic4);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic3);
  Serial.println();
}





void loop() {
  mqttClient.poll();
  Serial.print("Temperature = ");
  Serial.print(Environment.readTemperature());
  Serial.println(" C");
  Serial.print("Humidity = ");
  Serial.print(Environment.readHumidity());
  Serial.println(" %");
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0, 0);
  Oled.print("Temperature = ");
  Oled.setCursor(0, 10);
  Oled.print(Environment.readTemperature());
  Oled.print(" C");
  Oled.setCursor(0, 20);
  Oled.print("Humidity = ");
  Oled.setCursor(0, 30);
  Oled.print(Environment.readHumidity());
  Oled.print(" %");
  Oled.refreshDisplay();
  if (digitalRead(BUTTON) == HIGH) {
    tone(BUZZER_PIN, 2000);
    mqttClient.beginMessage(topic3);
    mqttClient.print(digitalRead(BUZZER_PIN));
    mqttClient.endMessage();
    delay(1000);
    noTone(BUZZER_PIN);
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.println("Sending messages to topics: ");
    Serial.println(topic);
    Serial.println(Environment.readTemperature());
    Serial.println(topic2);
    Serial.println(Environment.readHumidity());
    Serial.println(topic3);
    Serial.println(BUZZER_PIN);

    mqttClient.beginMessage(topic);
    mqttClient.print(Environment.readTemperature());
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(Environment.readHumidity());
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(digitalRead(BUZZER_PIN));
    mqttClient.endMessage();
  }
  delay(1000);
}



// ========== Light Code ==========
#elif defined(USE_LIGHT)

#define LIGHT_SENSOR A3
#define LED 6
#define DIAL A0

const char broker[] = "192.168.1.190";
int        port     = 1885;
const char mqttUser[] = "admin";
const char mqttPass[] = "admin";
const char topic[]  = "iot/Data/Light";
const char topic2[]  = "iot/Control/Dial";
const char topic3[]  = "iot/Data/LED";
const char topic4[]  = "iot/Control/LED";

const long interval = 1000;
unsigned long previousMillis = 0;

void onMqttMessage(int messageSize) {
  //we recieve a message, print out topic and contents
  Serial.print("Recieved a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.println("'");
  Serial.print("Length: ");
  Serial.println(messageSize);
  String payload = "";
  while (mqttClient.available()) {
    payload += (char)mqttClient.read();
  }
  if (payload == "1") {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}



void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LIGHT_SENSOR, INPUT);
  Oled.begin();
  Oled.setFlipMode(true);
  Wire.begin();
  Serial.begin(9600);
  WiFi.config(IPAddress(192,168,1,192));
  status = WiFi.begin(ssid, password);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
  }
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0,0);
  Oled.print("Connecting to:");
  Oled.setCursor(0,10);
  Oled.print(ssid);
  Oled.setCursor(0,30);
  Oled.print("IP:");
  Oled.print("192.168.1.192");
  delay(5000);
  server.begin();
  Oled.clear();
  Oled.setCursor(0,10);
  Oled.print("Connected");
  Oled.setCursor(0,20);
  Oled.print(WiFi.status());
  delay(2000);
  Oled.clear();
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setId("R4-2");
  mqttClient.setUsernamePassword(mqttUser, mqttPass);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic4);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic4);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic4);
  Serial.println();
}


void loop () {
  mqttClient.poll();
  mqttClient.beginMessage(topic2);
  mqttClient.endMessage();
  Oled.setFont(u8x8_font_chroma48medium8_r);
  Oled.setCursor(0, 10);
  Oled.print("Light = ");
  Oled.setCursor(0, 20);
  Oled.print(analogRead(LIGHT_SENSOR));
  Oled.refreshDisplay();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("Sending messages to topics: ");
    Serial.println(topic);
    Serial.println(analogRead(LIGHT_SENSOR));
    Serial.println(topic2);
    Serial.println(analogRead(DIAL));
    Serial.println(topic3);
    Serial.println(LED);

    mqttClient.beginMessage(topic);
    mqttClient.print(analogRead(LIGHT_SENSOR));
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(analogRead(DIAL));
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(digitalRead(LED));
    mqttClient.endMessage();
  }
  delay(1000);
}

// ========== Servo Code ==========
#elif defined(USE_SERVO)

#include <Servo.h>
Servo myServo;


#define SERVO 1

int        DIAL     = 90;
int        angle    = 90;

const char broker[] = "192.168.1.190";
int        port     = 1885;
const char mqttUser[] = "admin";
const char mqttPass[] = "admin";
const char topic[]  = "iot/Data/Dial_Speed";
// Servo angle is 0-180 with 90 being motionless
const char topic2[]  = "iot/Data/Servo_Speed";
const char topic4[]  = "iot/Control/Dial";
const char topic5[]  = "iot/Control/Servo_Speed";




void onMqttMessage(int messageSize) {
  // Capture EVERYTHING from the stream first
  String topic = mqttClient.messageTopic();
  String payload = "";
  while (mqttClient.available()) {
    payload += (char)mqttClient.read();
  }
  payload.trim();

  // Now safe to use both
  Serial.print("Topic: "); Serial.println(topic);
  Serial.print("Payload: "); Serial.println(payload);

  if (topic.equals(topic4)) {
    if (payload == "") {
      Serial.print("Payload empty, dropping");
    }
    else {
      Serial.println("ENTERED TOPIC4 BRANCH");
      DIAL = payload.toInt();
      angle = map(DIAL, 0, 1023, 180, 90);
      Serial.print("DIAL: "); Serial.println(DIAL);
      Serial.print("Angle: "); Serial.println(angle);
    }
  }
  if (topic.equals(topic5)) {
    if (payload == "90") {
      mqttClient.subscribe(topic4);
    } else {
      mqttClient.unsubscribe(topic4);
      angle = payload.toInt();
    }
  }
}



void setup() {
  myServo.attach(SERVO);
  Wire.begin();
  Serial.begin(9600);
  WiFi.config(IPAddress(192,168,1,193));
  status = WiFi.begin(ssid, password);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
  }
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  mqttClient.setId("R4-3");
  mqttClient.setUsernamePassword(mqttUser, mqttPass);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  delay(3000);

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic4);
  Serial.println();

  mqttClient.subscribe(topic4);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic5);
  Serial.println();

  mqttClient.subscribe(topic5);
  

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic4);
  Serial.println(topic5);
  Serial.println();
}


void loop () {
  mqttClient.poll();
  // sets the servo position according to the scaled value
  myServo.write(angle);
  Serial.println("Sending messages to topics: ");
  Serial.println(topic);
  Serial.println(DIAL);
  Serial.println(topic2);
  Serial.println(myServo.read());
  mqttClient.beginMessage(topic);
  mqttClient.print(DIAL);
  mqttClient.endMessage();

  mqttClient.beginMessage(topic2);
  mqttClient.print(myServo.read());
  mqttClient.endMessage();
}


#else
#error "No sensor type defined! Select an environment: DHT, LIGHT, or SERVO"
#endif