//#define BTN D0
//
//// VARIABLES ------------------------------------------------------------------------------------------
//char* nusp = "8006946";
//int stat = 0;
//char* L1 = "";
//char* L2 = "";
//char* L3 = "";
//int help = 0;
//
//void setup() {
//  Serial.begin(115200);
//  delay(50);
//  
//  pinMode(BTN, INPUT_PULLUP); 
//  attachInterrupt(digitalPinToInterrupt(BTN), handleInterrupt, CHANGE); 
//}
//
//void loop() {
//  Serial.println(digitalRead(BTN));
//  delay(500);
//}
//
//void handleInterrupt() {
//  long t = micros();
//  delay(50);
//  while(digitalRead(BTN) == 1){
//    if(micros() - t > 3000) {
//      stat = 1;
//      help = 1;
//      Serial.print("[STATUS] "); Serial.println(stat);
//      return;
//    }
//  }
//  while(digitalRead(BTN) == 0) {
//    if(micros() - t > 5000) {
//      stat = 0;
//      help = 0;
//      Serial.print("[STATUS] "); Serial.println(stat);
//      return;
//    }
//  }
//  delay(50);
//  while(digitalRead(BTN) == 1) {
//    stat = 2;
//    help = 1;
//  }
//  delay(50);
//  while(digitalRead(BTN) == 0) {
//    if(micros() - t > 1000) {
//      return;
//    }
//  }
//  delay(50);
//  stat = 3;
//  help = 1;
//  Serial.print("[STATUS] "); Serial.println(stat);
//  return;
//}


// VARIABLES ------------------------------------------------------------------------------------------
char* nusp = "8006946";
int stat = 0;
char* L1 = "";
char* L2 = "";
char* L3 = "";
int help = 0;


const int BTN = 0; //GPIO 0 (Flash Button) 
//const int LED=2; //On board blue LED 

void setup() { 
  Serial.begin(115200); 
  pinMode(D5 ,OUTPUT); 
  pinMode(BTN, INPUT_PULLUP); 
} 
 
void loop() { 
  analogWrite(D5, 900); // FRACO 
  delay(3000);
  analogWrite(D5, 200); // FORTE
  delay(3000);
} 

//void btn() {
//    long t = micros();
//    delay(50);
//    if(!digitalRead(BTN)) {  // PRIMEIRO APERTO
//      while(!digitalRead(BTN)) {
//        yield();
//        if(micros() - t > 3000000) { // SEGUROU
//          stat = 1;
//          help = 1;
//          Serial.print("[STATUS FLARE] "); Serial.println(stat);
//          delay(100);
//          return;
//        }
//      }
//      delay(50);
//      t = micros();
//      while(digitalRead(BTN)) {
//        yield();
//        if(micros() - t > 5000000) { // FALSO APERTO
//          stat = 0;
//          help = 0;
//          Serial.print("[STATUS RESET] "); Serial.println(stat);
//          return;
//        }
//      }
//      delay(50);
//      while(digitalRead(BTN)) { // SEGUNDO APERTO
//        yield();
//      }
//      delay(100);
//      stat = 2;
//      help = 1;
//      t = micros();
//      while(micros() - t < 1000000) {
//        if(!digitalRead(BTN)) {
//          stat = 3;
//          help = 1;
//          Serial.print("[STATUS SOCORRO] "); Serial.println(stat);
//          return;
//        }
//      }
//      Serial.print("[STATUS PERIGO] "); Serial.println(stat);
//      return;
//    }
//}

