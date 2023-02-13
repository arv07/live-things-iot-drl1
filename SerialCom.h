void serialEvents(String serialData)
{
    if (serialData == "PUT_FIGNERPRINT" or serialData == "PUT_FIGNERPRINT_AGAIN" or serialData == "REMOVE_FIGNERPRINT" or serialData == "PRINTS_MATCH")
        {
        }
        else if (serialData == "RESET_WIFI")
        {
            reconfigWifiSettings();
        }
        else if (serialData == "GET_SERVER")
        {
            Serial.println(getItemEEPROM(SERVER_POSITION));
        }
        else if (serialData == "GET_TOKEN")
        {
            Serial.println(getItemEEPROM(TOKEN_POSITION));
        }
        else if (serialData == "GET_PORT")
        {
            Serial.println(getItemEEPROM(PORT_POSITION));
        }
        else if (serialData == "GET_SOCKET_STATE")
        {
            Serial.println(socketIO.isConnected());
        }
        else if (serialData == "GET_RELAY_STATE")
        {
            Serial.println("Get state");
            //getCurrentStateRelay(TOKEN);
        }
        else if (serialData == "GET_DATE")
        {
            RtcDateTime now = Rtc.GetDateTime();

            Serial.println(String(now.Month()) + "/" + String(now.Day()) + "/" + String(now.Year()) + "/\r" + String(now.Hour()) + ":" + String(now.Minute()));
        }
        else if (serialData == "GET_SYSTEM_TIME")
        {
            RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
            Serial.println(compiled);
        }
        else if (serialData == "GET_VARIABLES")
        {
            RtcDateTime now = Rtc.GetDateTime();
            Serial.println(now.DayOfWeek());
        }
        else if (serialData == "GET_ALARM1")
        {

            schHour1 = getVariableFromMemory(81, 1).toInt();
            schMinute1 = getVariableFromMemory(83, 1).toInt();
            schAction1 = getVariableFromMemory(85, 1).toInt();
            schRepeat1 = getVariableFromMemory(88, 1).toInt();
        }
        else if (serialData == "GET_DATA_ALARM1")
        {
            Serial.println(schHour1);
            Serial.println(schMinute1);
            Serial.println(schAction1);
            Serial.println(schRepeat1);
            Serial.println(monday);
            Serial.println(tuesday);
            Serial.println(wednesday);
            Serial.println(thursday);
            Serial.println(friday);
            Serial.println(saturday);
            Serial.println(sunday);
        }
        else if (serialData == "SEND_EVENT")
        {
            String key[] = {"token", "state"};
            String value[] = {TOKEN, "500"};
            int sizeKey = sizeof(key) / sizeof(key[0]);
            int sizeValue = sizeof(value) / sizeof(value[0]);

            emitEvent(key, value, sizeKey, sizeValue, "DEVICE:testEvent");
        }
}