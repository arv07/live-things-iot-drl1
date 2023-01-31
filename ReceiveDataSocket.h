DynamicJsonDocument dataResponseSocket(256);

void responseServerSocket(uint8_t *payload, size_t length)
{
    // String data = "";

    int id = strtol((char *)payload, &sptr, 10);
    USE_SERIAL.printf("[IOc] get event: %s id: %d\n", payload, id);

    if (id)
    {
        payload = (uint8_t *)sptr;
    }

    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        USE_SERIAL.print(F("deserializeJson() failed: "));
        USE_SERIAL.println(error.c_str());
        return;
    }

    String eventName = doc[0];

    // Serial.println(eventName);

    if (eventName == "DEVICE:changeStateDRL1")
    {
        String data = doc[1];

        // deserializeJson(doc, data);
        DeserializationError error = deserializeJson(dataResponseSocket, data);

        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }

        String value = dataResponseSocket["response"];
        // Serial.printf(dataResponseSocket["response"]);

        changeStateRelay(value);

        // Serial.println("Evento esperado");
        // Serial.println(value);
    }
    else if (eventName == "DEVICE:getCurrentStateDRL1_r")
    {
        String data = doc[1];

        // deserializeJson(doc, data);
        DeserializationError error = deserializeJson(dataResponseSocket, data);

        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }

        String value = dataResponseSocket["response"];
        // Serial.printf(dataResponseSocket["response"]);

        changeStateRelay(value);
        /* if(value == "1")
        {
            stateRelay = true;
        }
        else if(value == "0")
        {
            stateRelay == false;
        } */

        // Serial.println("Evento esperado");
        // Serial.println(value);
    }
    else if (eventName == "DEVICE:updateDateTimeDRL1")
    {
        String data = doc[1];

        // deserializeJson(doc, data);
        DeserializationError error = deserializeJson(dataResponseSocket, data);

        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }

        String year = dataResponseSocket["year"];
        String month = dataResponseSocket["month"];
        String day = dataResponseSocket["day"];
        String hour = dataResponseSocket["hour"];
        String minute = dataResponseSocket["minute"];
        String second = dataResponseSocket["second"];
        // Serial.printf(dataResponseSocket["response"]);
        setDateTime(year, month, day, hour, minute, second);
        //changeStateRelay(value);
        /* if(value == "1")
        {
            stateRelay = true;
        }
        else if(value == "0")
        {
            stateRelay == false;
        } */

        // Serial.println("Evento esperado");
        // Serial.println(value);
    }


    else if (eventName == "DEVICE:setDeviceScheduler")
    {
        String data = doc[1];

        // deserializeJson(doc, data);
        DeserializationError error = deserializeJson(dataResponseSocket, data);

        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }

        String part = dataResponseSocket["part"];

        if(part == "1")
        {
            Serial.println("Part 1");
            String action = dataResponseSocket["action"];
            String hour = dataResponseSocket["hour"];
            String minute = dataResponseSocket["minute"];
            String repeat = dataResponseSocket["repeat"];
            String name = dataResponseSocket["name"];

            Serial.println(action);
            Serial.println(hour);
            Serial.println(minute);
            Serial.println(repeat);

            alarmOneP1(name, repeat, action, hour, minute);
        }
        else if(part == "2")
        {
            Serial.println("Part 2");
            String repeat_days = dataResponseSocket["repeat_days"];

        
            Serial.println(repeat_days);
            Serial.println(repeat_days[0]);

            deserializeJson(dataResponseSocket, repeat_days);

            alarmOneP2(dataResponseSocket[0],dataResponseSocket[1],dataResponseSocket[2],dataResponseSocket[3],dataResponseSocket[4],dataResponseSocket[5],dataResponseSocket[6]);

        }


        


        
        /* String minute = dataResponseSocket["minute"];
        String second = dataResponseSocket["second"]; */
        // Serial.printf(dataResponseSocket["response"]);
        
        //changeStateRelay(value);
        /* if(value == "1")
        {
            stateRelay = true;
        }
        else if(value == "0")
        {
            stateRelay == false;
        } */

        // Serial.println("Evento esperado");
        // Serial.println(value);
    }

    else if (eventName == "DEVICE:changeStateScheduler")
    {
        String data = doc[1];

        // deserializeJson(doc, data);
        DeserializationError error = deserializeJson(dataResponseSocket, data);

        if (error)
        {
            USE_SERIAL.print(F("deserializeJson() failed: "));
            USE_SERIAL.println(error.c_str());
            return;
        }

        String stateAlarm = dataResponseSocket["state"];

        // Serial.printf(dataResponseSocket["response"]);
        
        //changeStateRelay(value);
        /* if(value == "1")
        {
            stateRelay = true;
        }
        else if(value == "0")
        {
            stateRelay == false;
        } */

        // Serial.println("Evento esperado");
        // Serial.println(value);
    }

    /*String eventName3 = doc[1];
    Serial.println("event ---: %s\n" + String(eventName.c_str()));

    Serial.println("eve 33 ---: %s\n" + String(eventName3.c_str()));
    USE_SERIAL.printf("[IOc] event name: %s\n", eventName.c_str());

    USE_SERIAL.printf("[IOc] event name 3: %s\n", eventName3.c_str());*/
}