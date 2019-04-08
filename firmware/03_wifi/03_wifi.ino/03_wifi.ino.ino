#include <ESP8266WiFi.h>
#include <DNSServer.h>   // tirar depois
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

// MQTT ---------------------------------------------------------------------------------
const char* serialNumber = "DEVCE NAME";
const char* deviceName = serialNumber;
char* mqttServer = "BROKER-IP";
char* mqttUser = "MQTT-USR";
char* mqttPwd = "MQTT-PASS";
char payload[150];

// WIFI ---------------------------------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;


void setup() {
  Serial.begin(115200);
  delay(50);
  
  initWiFiManager();
  initMQTT();
}

void loop() {
  mqttloop();
  if(Serial.available()>0) {    //Checks is there any data in buffer 
    String s = Serial.readString();
    mqttloop();
    s.toCharArray(payload, 100);
//    snprintf(payload, 150, "%s", );
    client.publish("TOPIC", payload);
  }
  else
  {
    mqttloop();
    delay(100);                      // Wait for a second
  }

}

/////////////////////////////////////////////////////////////////////////////////////////
// INTERNET /////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// INITIALIZE WIFIMANAGER ---------------------------------------------------------------
void initWiFiManager() {
  
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setBreakAfterConfig(true);

    if (!wifiManager.autoConnect("GrindBit AP")) {
        Serial.println("Failed to connect and hit timeout");
        ESP.reset();
        delay(200);
    }
    Serial.println("Connected.");
}

// CONFIG CALLBACK WIFIMANAGER -----------------------------------------------------------
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.print("Entered config mode: ");
  Serial.println(WiFi.softAPIP());
}

/////////////////////////////////////////////////////////////////////////////////////////
// MQTT /////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// MQTT INIT ----------------------------------------------------------------------------
void initMQTT() {
    client.setServer(mqttServer, 1883);
    client.setCallback(mqtt_callback);
}

// MQTT LOOP ----------------------------------------------------------------------------
void mqttloop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// MQTT RECONNECT -----------------------------------------------------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection: ");
    if (client.connect(deviceName, mqttUser, mqttPwd)) {
      Serial.println("MQTT Connected.");
      client.subscribe("HUB1");
      client.subscribe("HUB2");
    } else {
      Serial.print("Failed: "); Serial.print(client.state()); Serial.println("Retry in .2 seconds");
      yield();
      delay(200);
    }
  }
}

// MQTT CALLBACK -------------------------------------------------------------------------
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);
}



//void setup() {
//  Serial.begin(115200);
//  delay(50);
//}
//
//void loop() {
//  if(Serial.available()>0) {    //Checks is there any data in buffer 
//    Serial.print("We got:");
//    Serial.print(Serial.readString());  //Read serial data byte and send back to serial monitor
//  }
//  else
//  {
//    delay(100);                      // Wait for a second
//  }
//}
