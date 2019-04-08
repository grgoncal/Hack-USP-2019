//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a node that logs to a central logging node
// The logServer example shows how to configure the central logging nodes
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

const int BTN = 0; //GPIO 0 (Flash Button) 
const int LED=2; //On board blue LED 

// VARIABLES ------------------------------------------------------------------------------------------
char* nusp = "8006946";
int stat = 0;
char* L1 = "HUB1";
char* L2 = "HUB2";
int help = 0;
long us = micros();

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );

size_t logServerId = 0;

// Send message to the logServer every 10 seconds 
Task myLoggingTask(10000, TASK_FOREVER, []() {
#if ARDUINOJSON_VERSION_MAJOR==6

        DynamicJsonDocument jsonBuffer(1024);
        JsonObject msg = jsonBuffer.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& msg = jsonBuffer.createObject();
#endif
    msg["topic"] = String(nusp);
    msg["status"] = String(stat);
    msg["id"] = "user";
    msg["L1"] = String(L1);
    msg["L2"] = String(L2);
    String str;
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, str);
#else
    msg.printTo(str);
#endif
    if (logServerId == 0) // If we don't know the logServer yet
        mesh.sendBroadcast(str);
    else
      if(help) {
        mesh.sendSingle(logServerId, str);
      }

    // log to serial
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, Serial);
#else
    msg.printTo(Serial);
#endif
    Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  delay(50);
  
  pinMode(LED,OUTPUT); 
  pinMode(D0, INPUT_PULLUP);
  
//  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  // Add the task to the your scheduler
  userScheduler.addTask(myLoggingTask);
  myLoggingTask.enable();
}

void loop() {
    userScheduler.execute(); // it will run mesh scheduler as well
    mesh.update();
    if(!stat) {
      btn();  
    } else {
      if(micros() - us > 15000000) {
        digitalWrite(LED, LOW);
        stat = 0;
        help = 0;
      }
    }
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());

  // Saving logServer
#if ARDUINOJSON_VERSION_MAJOR==6
  DynamicJsonDocument jsonBuffer(1024 + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
#endif
  if (root.containsKey("topic")) {
      if (String("HUB1").equals(root["topic"].as<String>())) {
          // check for on: true or false
          logServerId = root["nodeId"];
          Serial.printf("logServer detected!!!\n");
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());
  }
}

void btn() {
    long t = micros();
    delay(50);
    if(!digitalRead(BTN)) {  // PRIMEIRO APERTO
      while(!digitalRead(BTN)) {
        yield();
        if(micros() - t > 3000000) { // SEGUROU
          stat = 1;
          help = 1;
          us = micros();
          digitalWrite(LED, HIGH);
          Serial.print("[STATUS FLARE] "); Serial.println(stat);
          delay(100);
          return;
        }
      }
      delay(50);
      t = micros();
      while(digitalRead(BTN)) {
        yield();
        if(micros() - t > 5000000) { // FALSO APERTO
          stat = 0;
          help = 0;
          Serial.print("[STATUS RESET] "); Serial.println(stat);
          return;
        }
      }
      delay(50);
      while(digitalRead(BTN)) { // SEGUNDO APERTO
        yield();
      }
      delay(200);
      stat = 2;
      help = 1;
      t = micros();
      us = t;
      while(micros() - t < 1000000) {
        if(!digitalRead(BTN)) {
          stat = 3;
          help = 1;
          us = micros();
          digitalWrite(LED, HIGH);
          Serial.print("[STATUS SOCORRO] "); Serial.println(stat);
          return;
        }
      }
      digitalWrite(LED, HIGH);
      Serial.print("[STATUS PERIGO] "); Serial.println(stat);
      return;
    }
}
