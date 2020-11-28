#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <irsend.h>


const char* ssid = "<Wifi SSID>"; // Enter your WiFi name
const char* password =  "<Wifi password>"; // Enter WiFi password
const char* mqttServer = "<MQQT Server address>";
const int   mqttPort = 1883;
const char* mqttUser = "<MQTT user>";
const char* mqttPassword = "<MQTT password>";
const char* mqttTopic = "esp8266-01/radio/cmd";
const char* mqttState = "esp8266-01/radio/state";

WiFiClient espClient;
PubSubClient client(espClient);
IRsend irsend(3);  //send on RX PIN, GPIO3



void setup() {
  Serial.begin(9600); //set baudrate
  WiFi.begin(ssid, password); //initialize wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort); //connect to MQTT server
  client.setCallback(callback);   //set callback function
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.publish(mqttState, "STATUS:Online"); //Send a message: Online - for debugging
  client.subscribe(mqttTopic);
  irsend.begin();
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;

  Serial.print("Message converted to string: ");
  Serial.println(message);
  if (message == "poweron") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFFA25D);
    client.publish(mqttState, "poweron pushed");
  }
  if (message == "poweroff") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFFE21D);
    client.publish(mqttState, "poweroff pushed");
  }
  if (message == "mode01") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);

    radioSend(0xFF22DD);
    client.publish(mqttState, "mode01 pushed");
  }
  if (message == "mode02") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFFC23D);
    client.publish(mqttState, "mode02 pushed");
  }
  if (message == "mode03") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFFE01F);
    client.publish(mqttState, "mode03 pushed");
  }
  if (message == "mode04") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF906F);
    client.publish(mqttState, "mode04 pushed");
  }
  if (message == "mode05") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF6897);
    client.publish(mqttState, "mode05 pushed");
  }
  if (message == "mode06") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFFB04F);
    client.publish(mqttState, "mode06 pushed");
  }
  if (message == "mode07") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF30CF);
    client.publish(mqttState, "mode07 pushed");
  }
  if (message == "mode08") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF7A85);
    client.publish(mqttState, "mode08 pushed");
  }
  if (message == "dimmerup") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF5AA5);
    client.publish(mqttState, "dimmerup pushed");
  }
  if (message == "dimmerdown") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF10EF);
    client.publish(mqttState, "dimmerdown pushed");
  }
  if (message == "timers") {
    Serial.print("Message arrived in topic: ");
    Serial.println(message);
    radioSend(0xFF629D);
    client.publish(mqttState, "timers pushed");
  }
}

void radioSend(unsigned long code) {
  for (int i = 0; i < 3; i++) {
    //irsend.sendNECStandard(sAddress, sCommand, sRepeats);
    //uint16_t sAddress = 0xa90;
    //uint8_t sRepeats = 3;
    //irsend.sendNECStandard(sAddress, code, sRepeats);
    irsend.sendNEC(code, 32);
    delay(3000);
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    WiFi.begin(ssid, password); //reinitialize wifi

    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
        Serial.println("connected");
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
    }
    client.publish(mqttState, "STATUS:Online"); //Send a message: Online - for debugging
    client.subscribe(mqttTopic);
  }
  client.loop();
}
