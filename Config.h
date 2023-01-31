

String getTokenSaved()
{
    // EEPROM.begin(512);
    char tok[20] = "";
    for (int x = 0; x <= 20; x++)
    {
        // Serial.print(char(EEPROM.read(x)));
        /*Serial.print(token[x]);
        Serial.print(x);
        EEPROM.(x, token[0]);*/
        tok[x] = char(EEPROM.read(x));
        // token += tok[x];
    }

    return String(tok);
}

String getServerSaved()
{
    int position = 31; // Position in EEPROM stack
    char tok[20] = "";
    for (int x = 0; x <= 20; x++)
    {
        tok[x] = char(EEPROM.read(position + x));
    }

    return String(tok);
}

int getPortSaved()
{
    int position = 61; // Position in EEPROM stack
    char tok[20] = "";
    String value;
    for (int x = 0; x <= 20; x++)
    {
        tok[x] = char(EEPROM.read(position + x));
    }
    value = String(tok);
    // opt = option.toInt();
    return value.toInt();
}

void saveToken(char *parameterToken)
{
    for (int x = 0; x <= strlen(parameterToken); x++)
    {
        Serial.print(parameterToken[x]);
        //Serial.print(x);
        EEPROM.write(x, parameterToken[x]);
    }
    EEPROM.commit();
    //Serial.println("Token saved");
    //Serial.println(strlen(parameterToken));
}

void saveServer(char *parameterServer)
{
    int position = 31; // Position in EEPROM stack
    for (int x = 0; x <= strlen(parameterServer); x++)
    {
        Serial.print(parameterServer[x]);
        //Serial.print(x);
        EEPROM.write(position + x, parameterServer[x]);
    }

    EEPROM.commit();
    //Serial.println("Server saved");
    //Serial.println(strlen(parameterServer));
}

void savePort(char *parameterPort)
{
    int position = 61; // Position in EEPROM stack
    for (int x = 0; x <= strlen(parameterPort); x++)
    {
        Serial.print(parameterPort[x]);
        //Serial.print(x);
        EEPROM.write(position + x, parameterPort[x]);
    }

    EEPROM.commit();
    //Serial.println("Port saved");
    //Serial.println(strlen(parameterPort));
}

void reconfigWifiSettings()
{
    WiFiManager wm;

    WiFiManagerParameter tokenP("token", "token", getTokenSaved().c_str(), 20);
    WiFiManagerParameter serverP("server", "server", getServerSaved().c_str(), 20);
    WiFiManagerParameter portP("port", "port", String(getPortSaved()).c_str(), 5);

    wm.setSaveConfigCallback(saveConfigCallback);

    wm.addParameter(&tokenP);
    wm.addParameter(&serverP);
    wm.addParameter(&portP);

    // Waits 60 seconds for config
    wm.setConfigPortalTimeout(120);

    if (!wm.startConfigPortal("Reconfig"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        // reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5000);
    }

    // Serial.println("connected...yeey :)");
    strcpy(parameterToken, tokenP.getValue());
    strcpy(parameterServer, serverP.getValue());
    strcpy(parameterPort, portP.getValue());

    Serial.println(tokenP.getValue());
    Serial.println(serverP.getValue());
    Serial.println(portP.getValue());
    Serial.println(shouldSaveConfig);

    // Save token in EEPROM
    if (shouldSaveConfig == true)
    {
        Serial.println("Se procede a guardar ----");
        saveToken(parameterToken);
        saveServer(parameterServer);
        savePort(parameterPort);

        shouldSaveConfig = false;
        ESP.reset();
        delay(500);
    }
}

void startWifi()
{
    /*Parameter when configuring Wifi*/
    WiFiManagerParameter tokenP("token", "token", getTokenSaved().c_str(), 20);
    WiFiManagerParameter serverP("server", "server", getServerSaved().c_str(), 20);
    WiFiManagerParameter portP("port", "port", String(getPortSaved()).c_str(), 5);

    /*When wifi connection is established then a flag is triggered */
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    /*Add parameter to Wifi interface configuraction*/
    wifiManager.addParameter(&tokenP);
    wifiManager.addParameter(&serverP);
    wifiManager.addParameter(&portP);

    if (!wifiManager.autoConnect("DatasofwareIot", "password"))
    {
        Serial.println("failed to connect and hit timeout");
    }
    else
    {
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.isConnected());
        Serial.println(WiFi.getMode());
        Serial.println(WiFi.status());
    }

    /*Get the parameters entered by the user*/
    strcpy(parameterToken, tokenP.getValue());
    strcpy(parameterServer, serverP.getValue());
    strcpy(parameterPort, portP.getValue());

    if (shouldSaveConfig == true)
    {
        // Save token on EEPROM
        saveToken(parameterToken);
        saveServer(parameterServer);
        savePort(parameterPort);

        shouldSaveConfig = false;
        ESP.reset();
        delay(500);
    }
}
