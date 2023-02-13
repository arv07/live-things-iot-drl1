#include "SendDataSocket.h"
#include "Clock.h"
void changeStateRelay(String state)
{
    String key[] = {"token", "message"};
    String value[] = {TOKEN, "changed"};
    int sizeKey = sizeof(key) / sizeof(key[0]);
    int sizeValue = sizeof(value) / sizeof(value[0]);
    
    // String state = "1";
    Serial.println(state);
    if (state == "1")
    {
        digitalWrite(RELAY, LOW);
        IsRelayOn = true;
        emitEvent(key, value, sizeKey, sizeValue, "DEVICE:confirmChangeStateDRL1");

    }
    else if (state == "0")
    {
        digitalWrite(RELAY, HIGH);
        IsRelayOn = false;
        emitEvent(key, value, sizeKey, sizeValue, "DEVICE:confirmChangeStateDRL1");
    }

}
