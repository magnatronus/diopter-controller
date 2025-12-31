
/*
 * Diopter_Controller
 * Sketch used to control the 4 servos for the Diopter project
 * as well as reading the LIDAR for distance measurement
 */
#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"

// VAR defs
IPAddress ipAddress;
const char *ssid = "DIOPTER-ONE";
int stepIncrement = 5; // should really be 1 (5 for testing)

// create servo object to control a servo
Servo leftHorizontalServo;
Servo leftVerticalServo;
Servo rightHorizontalServo;
Servo rightVerticalServo;

// Max and mid point for each a servo
int servoMIN = 30;
int servoMID = 90;
int servoMAX = 150;

// variable to store the 4 servo positions
// position and calibrate for each
int posLeftHorizontal = servoMID;
int posLeftVertical = servoMID;
int posLeftVC = 0;
int posLeftHC = 0;
int posRightHorizontal = servoMID;
int posRightVertical = servoMID;
int posRightVC = 0;
int posRightHC = 0;

// GPIO pin used to connect the servos
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int leftVerticalPin = 12;
int leftHorizontalPin = 13;
int rightVerticalPin = 14;
int rightHorizontalPin = 15;

// WebServer & html helper
WebServer server(80);

// Render the specified file uploaded to SPIFFS
// if file not found return a 404
void renderFileStream(String filename)
{
  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    server.send(404, "text/plain", "File Not Found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Render a page to the HTTP server defined by the string [html]
void renderPage(int httpCode, String html)
{
  server.send(httpCode, "text/html", html);
}

// Set up WiFi as an AP
void initWiFi()
{
  Serial.println("Starting Access Point..");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  ipAddress = WiFi.softAPIP();
  Serial.print("Server address: ");
  Serial.println(ipAddress);
}

// Set up web server handlers
void initWebServer()
{
  Serial.println("Initialising Web Server routes ");
  server.serveStatic("/style.css", SPIFFS, "/style.css");
  server.serveStatic("/app.js", SPIFFS, "/app.js");
  server.on("/", []()
            { renderFileStream("/home.html"); });

  // LEFT EYE CONTROL
  server.on("/lz", []()
            { 
              Serial.println("LEFT ZERO");
              posLeftHC = posLeftHorizontal - 90;
              posLeftVC = posLeftVertical - 90;
              renderFileStream("/home.html"); });
  server.on("/lc", []()
            { 
              Serial.println("LEFT CENTER");
              posLeftHorizontal = 90;
              posLeftVertical = 90;
              leftHorizontalServo.write(posLeftHorizontal + posLeftHC);
              leftVerticalServo.write(posLeftVertical + posLeftVC);
              renderFileStream("/home.html"); });
  server.on("/lu", []()
            { 
              Serial.println("LEFT UP:" + posLeftVertical);
              if(posLeftVertical - stepIncrement > 0){
                posLeftVertical -= stepIncrement;
                leftVerticalServo.write((posLeftVertical + posLeftVC));
              }              
              renderFileStream("/home.html"); });
  server.on("/ld", []()
            { 
              Serial.println("LEFT DOWN:" + posLeftVertical); 
              posLeftVertical +=stepIncrement;
              leftVerticalServo.write((posLeftVertical + posLeftVC));                
              renderFileStream("/home.html"); });
  server.on("/ll", []()
            { 
              Serial.println("LEFT LEFT"); 
              if(posLeftHorizontal - stepIncrement > 0){
                posLeftHorizontal -= stepIncrement;
                leftHorizontalServo.write(posLeftHorizontal + posLeftHC);
              }                 
              renderFileStream("/home.html"); });
  server.on("/lr", []()
            { 
              Serial.println("LEFT RIGHT"); 
              posLeftHorizontal +=stepIncrement;
              leftHorizontalServo.write(posLeftHorizontal + posLeftHC);  
              renderFileStream("/home.html"); });
}

// Init the servo connection
// attach the servoPins to the servo object and set the sweep range
// For SG90, 500 and 2400 might be more suitable
// different servos may require different min/max settings
void initServo(Servo &s, int p)
{
  s.attach(p);
  s.write(servoMID);
  delay(10);
}

// test servo control
void exerciseServo(Servo &s){
    for (int posDegrees = servoMIN; posDegrees <= servoMAX; posDegrees++)
  {
    s.write(posDegrees);
    delay(20);
  }
  delay(20);
  s.write(servoMID);
}

// ESP32 SETUP
void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Running Diopter setup");

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // set up wifi & web server
  initWiFi();
  initWebServer();

  // Standard  servos (use 50 - 250)
  leftVerticalServo.setPeriodHertz(250);
  leftHorizontalServo.setPeriodHertz(250);
  rightVerticalServo.setPeriodHertz(250);
  rightHorizontalServo.setPeriodHertz(250);

  // init servos
  initServo(leftHorizontalServo, leftHorizontalPin);
  initServo(leftVerticalServo, leftVerticalPin);
  initServo(rightHorizontalServo, rightHorizontalPin);
  initServo(rightVerticalServo, rightVerticalPin);

  // exercise servos
  exerciseServo(leftHorizontalServo);
  exerciseServo(leftVerticalServo);
  exerciseServo(rightHorizontalServo);
  exerciseServo(rightVerticalServo);

  // Start the web server
  server.begin();
  Serial.println("Setup completed");
}

void loop()
{

  // Handle incoming client requests
  server.handleClient();
}
