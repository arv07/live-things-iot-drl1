#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(datestring);
}

/*Set date and time manually*/
void setDateTime(String year, String month, String day, String hour, String minute, String second)
{
    RtcDateTime manual = RtcDateTime(year.toInt(), month.toInt(), day.toInt(), hour.toInt(), minute.toInt(), second.toInt());
    Rtc.SetDateTime(manual);
}

void saveInMemory(int position, String variable, int length)
{
    for (int x = 0; x <= length; x++)
    {
        //Serial.println("Escribiendo");
        EEPROM.write(position + x, variable[x]);
    }
    EEPROM.commit();
}

String getVariableFromMemory(int position, int length)
{
    char variable[2] = "";
    for (int x = 0; x <= length; x++)
    {
        variable[x] = char(EEPROM.read(position + x));
    }
    //Serial.println(variable);
    //schHour1 = String(tok).toInt();

    return String(variable);
}

/*To set the part one of the alarm*/
void alarmOneP1(String name, String repeat, String action, String hour, String minute)
{

    //HOUR
    saveInMemory(81, hour, 1);
    //MINUTE
    saveInMemory(83, minute, 1);
    //ACTION
    saveInMemory(85, action, 1);
    //REPEAT
    saveInMemory(88, repeat, 1);

    //send message to server to confirm
    confirmChangeScheduler(TOKEN, "changed");
    delay(200);
    ESP.restart();

}

void alarmOneChangeState(String state)
{
    saveInMemory(104, state, 1);
}

void alarmOneP2(String monday, String tuesday, String wednesday, String thursday, String friday, String saturday, String sunday)
{
    saveInMemory(90, monday, 1);
    saveInMemory(92, tuesday, 1);
    saveInMemory(94, wednesday, 1);
    saveInMemory(96, thursday, 1);
    saveInMemory(98, friday, 1);
    saveInMemory(100, saturday, 1);
    saveInMemory(102, sunday, 1);
    
}

void initilizeClock()
{
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    // RtcDateTime compiled = RtcDateTime("Jan 25 2017", "16:30:00");
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid())
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
}
