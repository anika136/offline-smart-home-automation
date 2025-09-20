#include <Servo.h> 
#include <SoftwareSerial.h>

// Define Bluetooth module pins
SoftwareSerial btSerial(2, 3);  // RX, TX

// Define servos
Servo door, window;

//// Store current angle
//int door = -1; 
//int window = -1;

int light=4;
int fan=5;

// Track last command
String lastCommand = "";
bool stateChanged = false;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

 pinMode(light,OUTPUT);
  pinMode(fan,OUTPUT);

  door.attach(6);
  door.write(10); 
  window.attach(7);
  window.write(0); 
}

void loop() {
  if (btSerial.available()) {
    String command = btSerial.readString();
    command.trim();

    Serial.println("Received via BT: " + command);

    if (command != lastCommand) {
      lastCommand = command;
      stateChanged = true;

       if (command == "light on") {
       digitalWrite(light,HIGH);
       } else if (command == "light off" || command == "light of" ) {
       digitalWrite(light,LOW);
       }else if (command == "fan on" ) {
       digitalWrite(fan,HIGH);
       }else if (command == "fan of" || command == "fan off" ) {
       digitalWrite(fan,LOW);
       }else if (command == "door open" ) {
       door.write(110);       
       }else if (command == "door close" ) {
       door.write(10); 
       }else if (command == "window open" ) {
       window.write(180);       
       }else if (command == "window close" ) {
       window.write(0); 
       }else {
       Serial.println("Unknown command.");
       stateChanged = false;
      }
    }
  }

 
  delay(15);
}
