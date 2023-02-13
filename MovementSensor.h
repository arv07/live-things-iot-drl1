long checkSensorMovement = 1000;
long turnOffRelayTime = 1000;
const int sensorMovement = 12;     // D6
unsigned long previousMillis2 = 0; // last time update
unsigned long previousMillis3 = 0; // last time update
int counter = 0;

void readMovementSensor(unsigned long currentMillis)
{
    if (currentMillis - previousMillis2 > checkSensorMovement)
    {
        int val = digitalRead(sensorMovement);
        if (val == 1 and stateSensorMovement == true)
        {
            String key[] = {"token", "state"};
            String value[] = {TOKEN, "1"};
            int sizeKey = sizeof(key) / sizeof(key[0]);
            int sizeValue = sizeof(value) / sizeof(value[0]);
            emitEvent(key, value, sizeKey, sizeValue, "DEVICE:DRL1ActiveByMovementSensor");
            changeStateRelay("1");
            Serial.println("Sensor movimiento activado");
            //previousMillis3 = 0;
        }
        previousMillis2 = currentMillis;
    }
}

void turnOffRelayIfNoMovement(unsigned long currentMillis)
{
    if (currentMillis - previousMillis3 > turnOffRelayTime)
    {
        //Serial.print("Counter:");Serial.println(counter);
        if(stateSensorMovement == true and IsRelayOn == true)
        {
            counter++;

            if(counter == 61)
            {
                Serial.println("Apagar relay ----");
                String key[] = {"token", "state"};
                String value[] = {TOKEN, "0"};
                int sizeKey = sizeof(key) / sizeof(key[0]);
                int sizeValue = sizeof(value) / sizeof(value[0]);
                emitEvent(key, value, sizeKey, sizeValue, "DEVICE:DRL1ActiveByMovementSensor");
                changeStateRelay("0");
                counter = 0;
            }
        }
        previousMillis3 = currentMillis;
    }
}