#include <Servo.h> 
#include <SoftwareSerial.h>

// Define Bluetooth module pins
SoftwareSerial btSerial(2, 3);  // RX, TX

// Define servos
Servo door, window, dispenser;

//// Store current angle
//int door = -1; 
//int window = -1;

int light=4;
int fan=5;
int light2=8;

// Track last command
String lastCommand = "";
bool stateChanged = false;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

 pinMode(light,OUTPUT);
  pinMode(fan,OUTPUT);
 pinMode(light2,OUTPUT);

  door.attach(6);
  door.write(10); 
  window.attach(7);
  window.write(0);
  dispenser.attach(9);
  dispenser.write(7);
   
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
       }else if (command == "light to on" ) {
       digitalWrite(light2,HIGH);
       }else if (command == "light to off" || command == "light to of" ) {
       digitalWrite(light2,LOW);
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
       }else if (command == "dispenser open" ) {
       dispenser.write(100);       
       }else if (command == "dispenser close" ) {
       dispenser.write(7); 
       }else {
       Serial.println("Unknown command.");
       stateChanged = false;
      }
    }
  }

 
  delay(15);
}
