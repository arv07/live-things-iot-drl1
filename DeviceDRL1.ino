/*
 * WebSocketClientSocketIO.ino
 *
 *  Created on: 06.06.2016
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(D4, D5, D2); // IO, SCLK, CE For NodeMCU
RtcDS1302<ThreeWire> Rtc(myWire);

#include <Hash.h>
#include <EEPROM.h>
#include <WiFiManager.h>
WiFiManager wifiManager;
// flag for saving data, for Wifi Manager
boolean shouldSaveConfig = false;
String saveParams = "1";
char parameterToken[20] = "";
char parameterServer[20] = "";
char parameterPort[5] = "";
ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

DynamicJsonDocument doc(256);
DynamicJsonDocument dataSocket(1024);
String dataResponse = "";
#define USE_SERIAL Serial1

unsigned long previousMillis = 0; // last time update
long interval = 10000;            // interval at which to do something (milliseconds)
long checkAlarminterval = 30000;
long checkSensorMovement = 1000;

char buffer[4];
char *sptr = NULL;
String dataFingerPrint = "";

String TOKEN = "";
String SERVER = "";
int PORT;

String eventType = "ENROLL_ENTRY";

const int RELAY = 5;
boolean stateRelay = false;

boolean firstPetition = true;

int sensorMovement = 12;

int schHour1 = 0;
int schMinute1 = 0;
int schAction1 = 0;
int schRepeat1 = 0;
int stateAlarm = 0;

int monday = 0;
int tuesday = 0;
int wednesday = 0;
int thursday = 0;
int friday = 0;
int saturday = 0;
int sunday = 0;


//#include "SendDataSocket.h"
//#include "Clock.h"
#include "Relay.h"

#include "ReceiveDataSocket.h"

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case sIOtype_DISCONNECT:
        USE_SERIAL.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
        socketIO.send(sIOtype_CONNECT, "/");

        Serial.println("Se ha conectado");
        break;
    case sIOtype_EVENT:
    {
        //USE_SERIAL.printf("[IOc] get event aaa: %s\n", payload);
        // deserializeJson(doc, payload);
        USE_SERIAL.printf("[IOc] get event: %s\n", payload);
        USE_SERIAL.printf("[IOc] get event: %s\n", payload);

        USE_SERIAL.printf("-", payload);
        

        responseServerSocket(payload, length);

        break;
    }
    case sIOtype_ACK:
        USE_SERIAL.printf("[IOc] get ack: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_ERROR:
        USE_SERIAL.printf("[IOc] get error: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_EVENT:
        USE_SERIAL.printf("[IOc] get binary: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_ACK:
        USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
        hexdump(payload, length);
        break;
    }
}

// callback notifying us of the need to save config, apply for configure Wifi
void saveConfigCallback()
{
    Serial.println("Should save config...");
    shouldSaveConfig = true;
    saveParams = "2";
    Serial.println(saveParams);
}

/* WiFiManagerParameter tokenP("token", "token", getTokenSaved().c_str(), 20);
WiFiManagerParameter serverP("server", "server", getServerSaved().c_str(), 20);
WiFiManagerParameter portP("port", "port", String(getPortSaved()).c_str(), 5); */

#include "Config.h"

void setup()
{
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(9600);
    Serial.begin(9600);
    EEPROM.begin(512);

    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH);
    // Serial.println("mensaje inicio");

    // Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();


    startWifi();

    //initilizeClock();

    pinMode(sensorMovement, INPUT);

    //Load alarm variables
   /*  schHour1 = getVariableFromMemory(81, 1).toInt();
    schMinute1 = getVariableFromMemory(83, 1).toInt();
    schAction1 = getVariableFromMemory(85, 1).toInt();
    schRepeat1 = getVariableFromMemory(88, 1).toInt();
    monday = getVariableFromMemory(90, 1).toInt();
    tuesday = getVariableFromMemory(92, 1).toInt();
    wednesday = getVariableFromMemory(94, 1).toInt();
    thursday = getVariableFromMemory(96, 1).toInt();
    friday = getVariableFromMemory(98, 1).toInt();
    saturday = getVariableFromMemory(100, 1).toInt();
    sunday = getVariableFromMemory(102, 1).toInt(); */

    // Get token from EEPROM
    TOKEN = getTokenSaved();
    Serial.println(TOKEN);
    delay(500);
    SERVER = getServerSaved();
    Serial.println(SERVER);
    delay(500);
    PORT = getPortSaved();
    Serial.println(PORT);
    delay(500);

    /*SET HEADERS FOR SOCKETIO*/
    //String firedToken = "90e365d710e274a96030";
    String str;
    str = "token:";
    //str += firedToken + "\r\n";
    str += TOKEN + "\r\n";
    str +=  + "\r\n";
    str += "type:2";

    const char *header = str.c_str();

    // socketIO.setExtraHeaders("Authorization: 1234567890");
    // socketIO.setExtraHeaders("token: b050d78346a6450ca4898846dc9c8d5ecb98eef6433ab571885f6b0e0e22\r\ntype: 2");
    // 5e8a7fe76009bc90be55bd57f4f8b8b083af34e4318af6e0f83d2d20cd76
    // socketIO.setExtraHeaders("token:b050d78346a6450ca489");
    socketIO.setExtraHeaders(header);
    // int PORT = 3010;
    //  server address, port and URL
    // socketIO.begin("192.168.1.102", 3010, "/socket.io/?EIO=4");
    
    socketIO.begin(SERVER, PORT, "/socket.io/?EIO=4");
    //socketIO.begin("192.168.1.102", 3010, "/socket.io/?EIO=4");
    // socketIO.begin("129.159.123.207", 8082, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
    // socketIO.onEvent(socketIOEvent);
}

void loop()
{
    socketIO.loop();

    if (Serial.available() > 0)
    {

        dataFingerPrint = Serial.readStringUntil('\n');
        // Blynk.virtualWrite(V4, option);
        dataFingerPrint.trim();
        // opt = option.toInt();
        // Serial.println(dataFingerPrint);
        // int opcion =  s.read() -;
        // Serial.println(comingByte);
        // s.write("9");
        // s.write(opcion);
        // s.println(opcion);
        // s.println(opt);
        // Serial.println("-" + option);

        if (dataFingerPrint == "PUT_FIGNERPRINT" or dataFingerPrint == "PUT_FIGNERPRINT_AGAIN" or dataFingerPrint == "REMOVE_FIGNERPRINT" or dataFingerPrint == "PRINTS_MATCH")
        {
        }
        else if (dataFingerPrint == "RESET_WIFI")
        {
            reconfigWifiSettings();
        }
        else if (dataFingerPrint == "GET_SERVER")
        {
            Serial.println(getServerSaved());
        }
        else if (dataFingerPrint == "GET_TOKEN")
        {
            Serial.println(getTokenSaved());
        }
        else if (dataFingerPrint == "GET_PORT")
        {
            Serial.println(getPortSaved());
        }
        else if (dataFingerPrint == "GET_SOCKET_STATE")
        {
            Serial.println(socketIO.isConnected());
        }
        else if (dataFingerPrint == "GET_RELAY_STATE")
        {
            Serial.println("Get state");
            getCurrentStateRelay(TOKEN);
        }
        else if (dataFingerPrint == "GET_DATE")
        {
            RtcDateTime now = Rtc.GetDateTime();

            Serial.println(String(now.Month()) + "/" + String(now.Day()) + "/" + String(now.Year()) + "/\r" + String(now.Hour()) + ":" + String(now.Minute()));
        }
        else if (dataFingerPrint == "GET_SYSTEM_TIME")
        {
            RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
            Serial.println(compiled);
        }
        else if (dataFingerPrint == "GET_VARIABLES")
        {
            RtcDateTime now = Rtc.GetDateTime();
            Serial.println(now.DayOfWeek());
        }
        else if (dataFingerPrint == "GET_ALARM1")
        {

            schHour1 = getVariableFromMemory(81, 1).toInt();
            schMinute1 = getVariableFromMemory(83, 1).toInt();
            schAction1 = getVariableFromMemory(85, 1).toInt();
            schRepeat1 = getVariableFromMemory(88, 1).toInt();
        } 
        else if (dataFingerPrint == "GET_DATA_ALARM1")
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

        /*uint64_t now = millis();

        if(now - messageTimestamp > 10000) {
            messageTimestamp = now;

            // creat JSON message for Socket.IO (event)
            DynamicJsonDocument doc(1024);
            JsonArray array = doc.to<JsonArray>();

            // add evnet name
            // Hint: socket.on('event_name', ....
            array.add("event_name");

            // add payload (parameters) for the event
            JsonObject param1 = array.createNestedObject();
            param1["now"] = (uint32_t) now;

            // JSON to String (serializion)
            String output;
            serializeJson(doc, output);

            // Send event
            socketIO.sendEVENT(output);

            // Print JSON for debugging
            USE_SERIAL.println(output);
        }*/
    }

    unsigned long currentMillis = millis();

    //To make the first petition to the server in order to get the actual value of the relay
    if (currentMillis - previousMillis > interval and firstPetition == true)
    {
        previousMillis = currentMillis;
        Serial.println("Primera Petición");
        // Serial.println(TOKEN);
        getCurrentStateRelay(TOKEN);
        RtcDateTime now = Rtc.GetDateTime();
        String time = String(now.Day()) + "/" + String(now.Month()) + "/" + String(now.Year()) + "-" + String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
        sendCurrentDate(TOKEN, time);

        firstPetition = false;
        // do something
    }

    if (currentMillis - previousMillis > checkAlarminterval)
    {
        RtcDateTime now = Rtc.GetDateTime();
        if(now.Hour() == schHour1 and now.Minute() == schMinute1)
        {
            Serial.println("Se dispara alarma");
            Serial.println(now.Minute());
            changeStateRelay(String(schAction1));
        }
        previousMillis = currentMillis;
        
        
        // do something
    }

    //Timer movement sensor
    if (currentMillis - previousMillis > checkSensorMovement)
    {
        int val = digitalRead(sensorMovement);

        //Serial.println(val);

        previousMillis = currentMillis;
        
        // do something
    }


}