DynamicJsonDocument dataSocket(1024);

void emitEvent(String key[], String value[], int sizeKey, int sizeValue, String eventName)
{
    JsonArray array = dataSocket.to<JsonArray>();//
    array.add(eventName);//Set event name

    //add payload (parameters) for the event
    JsonObject param = array.createNestedObject();

    //Assign parameters
    for (int i = 0; i < sizeKey; i++)
    {
        param[key[i]] = value[i];
        /* Serial.print(key[i]);
        Serial.print(":");
        Serial.print(value[i]);
        Serial.println("-"); */
    }

    //JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event
    socketIO.sendEVENT(output);
    Serial.println(output);
}

void sendCurrentDate(String token, String msg)
{
    // Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:currentDateDRL1");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();

    // param1["now"] = (uint32_t) now;
    param["token"] = token;
    param["date"] = msg;

    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event
    socketIO.sendEVENT(output);
    // Serial.println(output);
}

void confirmChangeScheduler(String token, String msg)
{
    // Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();

    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:setDeviceScheduler_r");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();

    // param1["now"] = (uint32_t) now;
    param["token"] = token;
    param["message"] = msg;

    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event
    socketIO.sendEVENT(output);
    // Serial.println(output);
}
