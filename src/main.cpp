#include "server.h"
#include "webSocketManager.h"
#include "sanitize.h"
#include "WiFiUdp.h"

//char incoming_tiVa_data[20]  = {'0'};
String incoming_tiVa_data;

WiFiUDP UDP;    // instance of WiFiUDP class to receive and send
IPAddress timeServerIP; // timr.nist.gov NTP server address
const char* NTPServername = "time.nist.gov";

void setup()
{
    wifiInit();
    serverInit();
    webSocketinit();


}

void loop()
{
    handleClient();
    webSocketLoop();
    //webSocketSend();
    //get_serial_in();
    //sanitize_serial_in();
    /*
    if(Serial.available())
    {
        incoming_tiVa_data = Serial.readStringUntil('\n');
        Serial.print(incoming_tiVa_data);
    }*/
}
