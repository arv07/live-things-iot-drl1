//EEPROM Address
const int TOKEN_POSITION = 0;
const int SERVER_POSITION = 31;
const int PORT_POSITION = 61;
unsigned long startTimeResetAllBtn = 0;

WiFiManager wifiManager;
// flag for saving data, for Wifi Manager
boolean shouldSaveConfig = false;
String saveParams = "1";
char parameterToken[20] = "";
char parameterServer[20] = "";
char parameterPort[5] = "";


// callback notifying us of the need to save config, apply for configure Wifi
void saveConfigCallback()
{
    Serial.println("Should save config...");
    shouldSaveConfig = true;
    saveParams = "2";
    Serial.println(saveParams);
}


String getItemEEPROM(int position)
{
    //String result;
    char tok[20] = "";
    for (int x = 0; x <= 20; x++)
    {
        tok[x] = char(EEPROM.read(position + x));
    }

    return String(tok);
}

void setItemEEPROM(char *value, int position)
{
    for (int x = 0; x <= strlen(value); x++)
    {
        Serial.print(value[x]);
        EEPROM.write(position + x, value[x]);
    }
    EEPROM.commit();
}

void startWifi()
{
    /*Parameter when configuring Wifi*/
    WiFiManagerParameter tokenP("token", "token", getItemEEPROM(TOKEN_POSITION).c_str(), 20);
    WiFiManagerParameter serverP("server", "server", getItemEEPROM(SERVER_POSITION).c_str(), 20);
    WiFiManagerParameter portP("port", "port", getItemEEPROM(PORT_POSITION).c_str(), 5);

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
        setItemEEPROM(parameterToken, TOKEN_POSITION);
        setItemEEPROM(parameterServer, SERVER_POSITION);
        setItemEEPROM(parameterPort, PORT_POSITION);

        shouldSaveConfig = false;
        ESP.reset();
        delay(500);
    }
}

void reconfigWifiSettings()
{
    WiFiManager wm;

    WiFiManagerParameter tokenP("token", "token", getItemEEPROM(TOKEN_POSITION).c_str(), 20);
    WiFiManagerParameter serverP("server", "server", getItemEEPROM(SERVER_POSITION).c_str(), 20);
    WiFiManagerParameter portP("port", "port", getItemEEPROM(PORT_POSITION).c_str(), 5);

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
        setItemEEPROM(parameterToken, TOKEN_POSITION);
        setItemEEPROM(parameterServer, SERVER_POSITION);
        setItemEEPROM(parameterPort, PORT_POSITION);

        shouldSaveConfig = false;
        ESP.reset();
        delay(500);
    }
}


void resetAllSettings(int stateResetAllBtn)
{
    if (stateResetAllBtn == HIGH) {
    // if the button is pressed
    if (startTimeResetAllBtn == 0) {
      // if this is the first time the button was pressed
      startTimeResetAllBtn = millis(); // store the current time
    } else if (millis() - startTimeResetAllBtn >= 8000) {
      // if the button has been pressed for 5 seconds
      Serial.println("Se procede a reiniciar");
      startTimeResetAllBtn = 0;
    }
  }else{
    startTimeResetAllBtn = 0;
  } 
}