
/*
 * Diopter_Controller
 * Sketch used to control the 4 servos for the Diopter project
 * as well as reading the LIDAR for distance measurement
*/
#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DiopterHTML.h>


// VAR defs
IPAddress ipAddress;
const char* ssid = "DIOPTER-ONE";


// create servo object to control a servo
Servo myservo;

// variable to store the servo position
int pos = 0;

// GPIO pin used to connect the servo
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin = 13;

// WebServer & html helper
WebServer server(80);
DiopterHTML html = DiopterHTML();

// Standard HTML Page Header and Body
String bodyStart() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".input-text { font-size: 30px; margin: 2px;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".submit-btn { background-color: #33CC00; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".btn-stop { background-color: #CC0000; }</style>";
  html += "</head>";
  html += "<body>";
  return html;
}




// Set up WiFi as an AP
void initWiFi(){
  Serial.println("Starting Access Point..");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  ipAddress = WiFi.softAPIP();
  Serial.print("Server address: "); Serial.println(ipAddress);
}

// Set up web server handlers
void initWebServer(){
  Serial.println("Initialising Web Server routes ");
  server.on("/", html.showHomePage(server&));
  //server.on("/app/run", handleStart);
  //server.on("/app/stop", handleStop);
}



void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Running setup");

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // set up wifi & web server
  initWiFi();
  initWebServer();

  // Standard 50hz servo
  myservo.setPeriodHertz(250);

  // attach the servoPin to the servo object and set the sweep range
  // For SG90, 500 and 2400 might be more suitable
  // different servos may require different min/max settings
  myservo.attach(servoPin, 500, 2400);
  myservo.write(0);
  delay(50);

  // Start the web server
  server.begin();
  Serial.println("Setup completed");

}

void loop() {

  // Handle incoming client requests
  server.handleClient();

  // // goes from 0 degrees to 180 degrees
  // for (pos = 0; pos <= 180; pos += 1) {
  //   myservo.write(pos);    // tell servo to go to position in variable 'pos'
  //   delay(100);             // waits 15ms for the servo to reach the position
  // }

  // // goes from 180 degrees to 0 degrees
  // for (pos = 180; pos >= 0; pos -= 1) {
  //   myservo.write(pos);    // tell servo to go to position in variable 'pos'
  //   delay(1);             // waits 15ms for the servo to reach the position
  // }
  // delay(50);
}



