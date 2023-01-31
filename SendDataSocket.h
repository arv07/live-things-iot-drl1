void getCurrentStateRelay(String token)
{
    //Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();
    
    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:getCurrentStateDRL1");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();
    
    //param1["now"] = (uint32_t) now;
    param["token"] = token;
    //param["mode"] = mode;
    
    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event        
    socketIO.sendEVENT(output);
    //Serial.println(output);
}


void confirmChangeState(String token, String msg)
{
    //Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();
    
    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:confirmChangeStateDRL1");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();
    
    //param1["now"] = (uint32_t) now;
    param["token"] = token;
    param["message"] = msg;
    
    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event        
    socketIO.sendEVENT(output);
    //Serial.println(output);
}


void sendCurrentDate(String token, String msg)
{
    //Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();
    
    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:currentDateDRL1");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();
    
    //param1["now"] = (uint32_t) now;
    param["token"] = token;
    param["date"] = msg;
    
    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event        
    socketIO.sendEVENT(output);
    //Serial.println(output);
}

void confirmChangeScheduler(String token, String msg)
{
    //Serial.println("enviando.....");
    JsonArray array = dataSocket.to<JsonArray>();
    
    // add evnet name
    // Hint: socket.on('event_name', ....
    array.add("DEVICE:setDeviceScheduler_r");

    // add payload (parameters) for the event
    JsonObject param = array.createNestedObject();
    
    //param1["now"] = (uint32_t) now;
    param["token"] = token;
    param["message"] = msg;
    
    // JSON to String (serializion)
    String output;
    serializeJson(dataSocket, output);

    // Send event        
    socketIO.sendEVENT(output);
    //Serial.println(output);
}

