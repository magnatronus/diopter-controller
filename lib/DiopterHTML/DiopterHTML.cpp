#include <Arduino.h>

#include "DiopterHTML.h"


DiopterHTML::DiopterHTML(){};

// Show the Home page
String DiopterHTML::homePage() {
    Serial.println("home page serving");
    String html = bodyStart();
    html += "<h1>SpiralArm Diopter Controller</h1>";
    html += bodyEnd();
    return html;
};

// Single motor controller as a set of 4 buttons
// up, down, left, right
String motorController(){
    String mc = "";
    return mc;
}



// // Send the HTML(serve it)
// void serverSend(String html, WebServer server){
//     server.send(200, "text/html", html);
// };

// Standard HTML Page Header and Body
String DiopterHTML::bodyStart() {
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
};

// Standard closure for HTML page
String DiopterHTML::bodyEnd(){
    return "</body></html>";
};

