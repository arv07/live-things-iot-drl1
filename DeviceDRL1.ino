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

#include "DeviceSettings.h"

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

DynamicJsonDocument doc(256);

String dataResponse = "";
#define USE_SERIAL Serial1

unsigned long previousMillis = 0; // last time update
long interval = 10000;            // interval at which to do something (milliseconds)
long checkAlarminterval = 30000;


char buffer[4];
char *sptr = NULL;
String serialData = "";

String TOKEN = "";
String SERVER = "";
int PORT;

const int RELAY = 5;//D1
const int ResetAllBtn = 15;//D8

boolean IsRelayOn = false;
boolean stateSensorMovement = false;

boolean firstPetition = true;



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

// #include "SendDataSocket.h"
// #include "Clock.h"
#include "Relay.h"
#include "MovementSensor.h"
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
        USE_SERIAL.printf("[IOc] get event: %s\n", payload);
        USE_SERIAL.printf("[IOc] get event: %s\n", payload);
        USE_SERIAL.printf("-", payload);
        Serial.printf("[IOc] get event: %s\n", payload);

        // responseServerSocket(payload, length);
        eventHadlerSocketIO(payload, length);

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

#include "Config.h"
#include "SerialCom.h"

void setup()
{
    USE_SERIAL.begin(9600);
    Serial.begin(9600);
    EEPROM.begin(512);

    pinMode(RELAY, OUTPUT);
    pinMode(sensorMovement, INPUT);
    pinMode(ResetAllBtn, INPUT);
    
    digitalWrite(RELAY, HIGH);

    USE_SERIAL.setDebugOutput(true);

    startWifi();
    // initilizeClock();

    // Load alarm variables
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
    Serial.println("Vairables configuracion -------");
    TOKEN = getItemEEPROM(TOKEN_POSITION);
    Serial.println(TOKEN);
    delay(500);
    SERVER = getItemEEPROM(SERVER_POSITION);
    Serial.println(SERVER);
    delay(500);
    PORT = getItemEEPROM(PORT_POSITION).toInt();
    Serial.println(PORT);
    delay(500);

    /*SET HEADERS FOR SOCKETIO*/
    String str;
    str = "token:";
    str += TOKEN + "\r\n";
    str += "type:2";
    const char *header = str.c_str();

    // socketIO.setExtraHeaders("token: b050d78346a6450ca4898846dc9c8d5ecb98eef6433ab571885f6b0e0e22\r\ntype: 2");
    socketIO.setExtraHeaders(header);
    socketIO.begin(SERVER, PORT, "/socket.io/?EIO=4");

    //event handler
    socketIO.onEvent(socketIOEvent);
}

void loop()
{
    socketIO.loop();
    int stateResetAllBtn = digitalRead(ResetAllBtn);
    unsigned long currentMillis = millis();
    resetAllSettings(stateResetAllBtn);
    readMovementSensor(currentMillis);
    turnOffRelayIfNoMovement(currentMillis);

    if (Serial.available() > 0)
    {
        serialData = Serial.readStringUntil('\n');
        serialData.trim();
        serialEvents(serialData);
    }

    // To make the first petition to the server in order to get the actual value of the relay
    if (currentMillis - previousMillis > interval and firstPetition == true)
    {
        previousMillis = currentMillis;
        Serial.println("Primera Petición");
        // Serial.println(TOKEN);
        // getCurrentStateRelay(TOKEN);

        String key[] = {"token"};
        String value[] = {TOKEN};
        int sizeKey = sizeof(key) / sizeof(key[0]);
        int sizeValue = sizeof(value) / sizeof(value[0]);
        emitEvent(key, value, sizeKey, sizeValue, "DEVICE:getCurrentStateDRL1");

        RtcDateTime now = Rtc.GetDateTime();
        String time = String(now.Day()) + "/" + String(now.Month()) + "/" + String(now.Year()) + "-" + String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
        sendCurrentDate(TOKEN, time);

        firstPetition = false;
        // do something
    }

    if (currentMillis - previousMillis > checkAlarminterval)
    {
        RtcDateTime now = Rtc.GetDateTime();
        if (now.Hour() == schHour1 and now.Minute() == schMinute1)
        {
            Serial.println("Se dispara alarma");
            Serial.println(now.Minute());
            changeStateRelay(String(schAction1));
        }
        previousMillis = currentMillis;

        // do something
    }

    
}
