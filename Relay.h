#include "SendDataSocket.h"
#include "Clock.h"
void changeStateRelay(String state)
{
    Serial.println(state);
    if(state == "1")
    {
        digitalWrite(RELAY, LOW);
        confirmChangeState(TOKEN, "changed");
    }
    else if(state == "0")
    {
        digitalWrite(RELAY, HIGH);
        confirmChangeState(TOKEN, "changed");
        
    }
    
}
