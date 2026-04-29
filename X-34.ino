#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "G02Boom";
const char* password = "G02BOOM!?@";

const int ENA = 14; 
const int IN1 = 27;
const int IN2 = 26;

const int IN3 = 25;
const int IN4 = 33;
const int ENB = 32;


const int speed = 200; 

WebServer server(80);

// Motor Control Functions 
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Motor A Forward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Motor B Forward
  analogWrite(ENA, speed); analogWrite(ENB, speed);
  Serial.println("DRIVE: Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Motor A Backward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Motor B Backward
  analogWrite(ENA, speed); analogWrite(ENB, speed);
  Serial.println("DRIVE: Reverse");
}

void turnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Motor A Backward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Motor B Forward
  analogWrite(ENA, speed); analogWrite(ENB, speed);
  Serial.println("DRIVE: Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Motor A Forward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Motor B Backward
  analogWrite(ENA, speed); analogWrite(ENB, speed);
  Serial.println("DRIVE: Right");
}

void stopMotor() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);    analogWrite(ENB, 0);
  Serial.println("BRAKE: Stop");
}

//Site localhost do ESP32
void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0'>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#222;color:white;touch-action:manipulation;}.btn{width:80px;height:80px;margin:10px;background:#444;color:white;border:2px solid #fff;border-radius:10px;font-size:12px;font-weight:bold;}.btn:active{background:#00ff00;color:black;}</style></head><body>";
  html += "<h1>RC PRO CONTROL</h1>";
  html += "<script>function send(cmd){fetch('/'+cmd);}</script>";
  html += "<button class='btn' ontouchstart=\"send('forward')\" ontouchend=\"send('stop')\">FORWARD</button><br>";
  html += "<button class='btn' ontouchstart=\"send('left')\" ontouchend=\"send('stop')\">LEFT</button>";
  html += "<button class='btn' ontouchstart=\"send('right')\" ontouchend=\"send('stop')\">RIGHT</button><br>";
  html += "<button class='btn' ontouchstart=\"send('backward')\" ontouchend=\"send('stop')\">REVERSE</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  //Definir todos os pinos de motores como output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Iniciar access point
  WiFi.softAP(ssid, password);
  
  //associar requests do access point a funcões do esp 32
  server.on("/", handleRoot);
  server.on("/forward", []() { moveForward(); server.send(200); });
  server.on("/backward", []() { moveBackward(); server.send(200); });
  server.on("/left", []() { turnLeft(); server.send(200); });
  server.on("/right", []() { turnRight(); server.send(200); });
  server.on("/stop", []() { stopMotor(); server.send(200); });
  
  server.begin();
}

void loop() {
  server.handleClient();
}
