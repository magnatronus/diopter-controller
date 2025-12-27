
#ifndef DiopterHTML_h
#define DiopterHTML_h
#include "Arduino.h"
#include <WebServer.h>
class DiopterHTML {

public:
    DiopterHTML();
    void showHomePage(WebServer);


private:

    // Send the HTML(serve it)
    void serverSend(String, WebServer &server);

    // Standard HTML Page Header and Body
    String bodyStart();

    // Standard closure for HTML page
    String bodyEnd();
};
#endif
