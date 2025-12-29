
#ifndef DiopterHTML_h
#define DiopterHTML_h
#include "Arduino.h"

class DiopterHTML {
    
    private:
        String bodyStart();
        String bodyEnd();
        String motorController();

    public:
        DiopterHTML();
        String homePage();
};

#endif
