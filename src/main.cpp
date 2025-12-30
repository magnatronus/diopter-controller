
/*
 * Diopter_Controller
 * Sketch used to control the 4 servos for the Diopter project
 * as well as reading the LIDAR for distance measurement
 */
#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
// #include <DiopterHTML.h>
#include "SPIFFS.h"

// VAR defs
IPAddress ipAddress;
const char *ssid = "DIOPTER-ONE";
int stepIncrement = 1;  // should really be 1 (5 for testing)

// create servo object to control a servo
Servo leftHorizontalServo;
Servo leftVerticalServo;

// variable to store the servo position
int posLeftHorizontal = 0;
int posLeftVertical = 0;

// GPIO pin used to connect the servo
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int leftVerticalPin = 12;
int leftHorizontalPin = 13;

// WebServer & html helper
WebServer server(80);
// DiopterHTML htmlGen = DiopterHTML();

// Render the specified file uploaded to SPIFFS
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

// Render a page to the HTTP server deined by the string [html]
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
  server.on("/lc", []()
            { 
              Serial.println("LEFT CENTER");
              posLeftHorizontal = 90;
              posLeftVertical = 90;
              leftHorizontalServo.write(posLeftHorizontal);
              leftVerticalServo.write(posLeftVertical);
              renderFileStream("/home.html"); 
            });
  server.on("/lu", []()
            { 
              Serial.println("LEFT UP");
              if(posLeftVertical - stepIncrement > 0){
                posLeftVertical -= stepIncrement;
                leftVerticalServo.write(posLeftVertical);
              }              
              renderFileStream("/home.html"); 
            });
  server.on("/ld", []()
            { 
              Serial.println("LEFT DOWN"); 
              posLeftVertical +=stepIncrement;
              leftVerticalServo.write(posLeftVertical);                
              renderFileStream("/home.html"); 
            });
  server.on("/ll", []()
            { 
              Serial.println("LEFT LEFT"); 
              if(posLeftHorizontal - stepIncrement > 0){
                posLeftHorizontal -= stepIncrement;
                leftHorizontalServo.write(posLeftHorizontal);
              }                 
              renderFileStream("/home.html"); 
            });
  server.on("/lr", []()
            { 
              Serial.println("LEFT RIGHT"); 
              posLeftHorizontal +=stepIncrement;
              leftHorizontalServo.write(posLeftHorizontal);  
              renderFileStream("/home.html"); 
            });
}

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Running setup");

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

  // attach the servoPins to the servo object and set the sweep range
  // For SG90, 500 and 2400 might be more suitable
  // different servos may require different min/max settings
  leftVerticalServo.attach(leftVerticalPin, 500, 2400);
  leftVerticalServo.write(posLeftVertical);
  delay(50);
  leftHorizontalServo.attach(leftHorizontalPin, 500, 2400);
  leftHorizontalServo.write(posLeftHorizontal);
  delay(50);

  // Start the web server
  server.begin();
  Serial.println("Setup completed");
}

void loop()
{

  // Handle incoming client requests
  server.handleClient();

}
