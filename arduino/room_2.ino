#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
int buzzer = 6;
int servo_pin = 4;

#define card "3B 1B 3E 02"  // main card UID
#define tag  "A3 18 FB 2C"  // special tag UID

MFRC522 mfrc522(SS_PIN, RST_PIN); 
Servo myServo;

// --- Variables for tag detection ---
int consecutiveTagCount = 0;   
unsigned long lastTagTime = 0;

void setup() {
  Serial.begin(9600); 
  SPI.begin();        
  mfrc522.PCD_Init(); 
  Serial.println("Approximate your card to the reader...");
  pinMode(buzzer, OUTPUT);

  myServo.attach(servo_pin); 
  myServo.write(125); // closed position
}

void loop() {
  automatic();
}

void automatic() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Build UID string
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  Serial.print("UID tag: ");
  Serial.println(content.substring(1));

  if (content.substring(1) == card) {
    Serial.println("Access Granted -> Gate Open");
    openGate();
    consecutiveTagCount = 0;
  }
  else if (content.substring(1) == tag) {
    Serial.println("Special Tag Detected");
    handleTagDetection();
  }
  else {
    Serial.println("Access Denied");
    consecutiveTagCount = 0;
  }

  // 🚨 IMPORTANT: Stop card communication so it won’t read again until removed
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void openGate() {
  myServo.write(0); 
  delay(5000);      
  myServo.write(125); 
  Serial.println("Gate Closed");
}

void handleTagDetection() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastTagTime > 3000) {
    consecutiveTagCount = 0;
  }

  consecutiveTagCount++;
  lastTagTime = currentMillis;

  if (consecutiveTagCount == 1) {
    Serial.println("1st tag detected -> short beep");
    shortBeep();
  }
  else if (consecutiveTagCount == 2) {
    Serial.println("2nd tag detected -> short beep");
    shortBeep();
  }
  else if (consecutiveTagCount == 3) {
    Serial.println("3rd tag detected -> Alarm beep!");
    alarmBeep();
    consecutiveTagCount = 0; 
  }
}

// --- buzzer functions ---
void shortBeep() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}

void alarmBeep() {
  unsigned long start = millis();
  while (millis() - start < 5000) { 
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}
