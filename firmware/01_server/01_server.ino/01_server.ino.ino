//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a single node (this node) as a logging node
// The logClient example shows how to configure the other nodes
// to log to this server
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
// Prototype
void receivedCallback( uint32_t from, String &msg );

int addrDimmer;
long us = micros();


// Send my ID every 10 seconds to inform others
Task logServerTask(10000, TASK_FOREVER, []() {
#if ARDUINOJSON_VERSION_MAJOR==6
        DynamicJsonDocument jsonBuffer(1024);
        JsonObject msg = jsonBuffer.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& msg = jsonBuffer.createObject();
#endif
    msg["topic"] = "HUB1";
    msg["nodeId"] = mesh.getNodeId();
    msg["id"] = "post";
    msg["dim"]=addrDimmer;

    String str;
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, str);
#else
    msg.printTo(str);
#endif
    mesh.sendBroadcast(str);

    // log to serial
//#if ARDUINOJSON_VERSION_MAJOR==6
//    serializeJson(msg, Serial);
//#else
//    msg.printTo(Serial);
//#endif
//    Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  
  pinMode(D5 ,OUTPUT); 
  analogWrite(D5, 900); // FRACO 
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE | DEBUG ); // all types on
  //mesh.setDebugMsgTypes( ERROR | CONNECTION | SYNC | S_TIME );  // set before init() so that you can see startup messages
//  mesh.setDebugMsgTypes( ERROR | CONNECTION | S_TIME );  // set before init() so that you can see startup messages
  
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  mesh.onNewConnection([](size_t nodeId) {
//    Serial.printf("New Connection %u\n", nodeId);
    yield();
  });

  mesh.onDroppedConnection([](size_t nodeId) {
//    Serial.printf("Dropped Connection %u\n", nodeId);
    yield();
  });

  // Add the task to the your scheduler
  userScheduler.addTask(logServerTask);
  logServerTask.enable();
}

void loop() {
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
  if(Serial.available()>0) {    //Checks is there any data in buffer 
    String s = Serial.readString();
    analogWrite(D5, 200); // FORTE
    us = micros();
    addrDimmer = 1;
  }
  if(micros() - us > 10000000) {
    analogWrite(D5, 900); // FRACO 
    addrDimmer = 0;
  }
}

void receivedCallback( uint32_t from, String &msg ) {
//  Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());
  
#if ARDUINOJSON_VERSION_MAJOR==6
  DynamicJsonDocument jsonBuffer(1024 + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
//    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
#endif
  if (root.containsKey("id")) {
      if (String("user").equals(root["id"].as<String>())) {
        const char* topic = root["topic"];
        const char* stat = root["status"];
        const char* L1 = root["L1"];
        const char* L2 = root["L2"];
        int numberStatus = atoi(stat);
        if(numberStatus) {
          String s = String(topic);
          s = s + ',';
          s = s + String(stat);
          s = s + ',';
          s = s + String(L1);
          s = s + ',';
          s = s + String(L2);
          Serial.print(s);
        }
      }
  }

  
//  String message = msg;
//  Serial.println(message);
}
