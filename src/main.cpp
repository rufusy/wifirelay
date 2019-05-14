#include "server.h"
#include "webSocketManager.h"
#include "sanitize.h"
#include <WiFiUdp.h>
//#include "ntp.h"


WiFiUDP UDP;    // instance of WiFiUDP class to receive and send
IPAddress timeServerIP; // timr.nist.gov NTP server address
const char* NTPServerName = "time.nist.gov"; //
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte NTPBuffer[NTP_PACKET_SIZE]; // buffer to hold incoming and outgoing packets

unsigned long intervalNTP = (60000*1);  // request NTP time every 300seconds
unsigned long prevNTP = 0;
unsigned lastNTPResponse = millis();
uint32_t timeUNIX = 0;
unsigned long prevActualTime = 0;

void startUDP(void);
uint32_t getTime(void);
void sendNTPpacket(IPAddress& address);
inline int getSeconds(uint32_t UNIXTime);
inline int getMinutes(uint32_t UNIXTime);
inline int getHours(uint32_t UNIXTime);
void ntp_timer(void);


void
setup()
{
    wifiInit();
    serverInit();
    webSocketinit();
    startUDP();
}


void
loop()
{
    handleClient();
    webSocketLoop();
    ntp_timer();
}


void
startUDP(void)
{
    Serial.println("Starting UDP");
    UDP.begin(123); // start listening for UDP packets on port 123
    Serial.print("Local port:\t");
    Serial.println(UDP.localPort());
    Serial.println();

    if(!WiFi.hostByName(NTPServerName, timeServerIP))
    {// Get the IP address of the NTP server
        Serial.println("DNS lookup failed. Rebooting...");
        Serial.flush();
        ESP.reset();
    }
    Serial.print("Time server IP:\t");
    Serial.println("timeServerIp");
    Serial.println("\r\nSending NTP request....");
    sendNTPpacket(timeServerIP);
}


uint32_t
getTime(void)
{
    if(UDP.parsePacket() == 0)
    {   // if there is no response yet
        return 0;
    }
    UDP.read(NTPBuffer, NTP_PACKET_SIZE);   // read the packet into the buffer
    // combine the 4 timestamp bytes into one 2 bit number
    uint32_t NTPTime =  (NTPBuffer[40]<<24) |
                        (NTPBuffer[41]<<16) |
                        (NTPBuffer[42]<<8)  |
                        (NTPBuffer[43]);
    // convert NTP time to a UNIX timestamp
    // UNIX time starts on jan 1 1970. That's 2208988800 seconds in NTP time
    const uint32_t seventYears = 2208988800UL;
    // subtract seventy years
    uint32_t UNIXTime = NTPTime - seventYears;
    return UNIXTime;
}



void
sendNTPpacket(IPAddress& address)
{
    memset (NTPBuffer, 0, NTP_PACKET_SIZE); // set all bytes in the buffer to 0
    // Initialize values needed to form NTP request
    NTPBuffer[0] = 0b11100011;
    // LI, Version, Mode
    // send a packet requesting a timestamp:
    UDP.beginPacket(address, 123); // NTP requests are to port 123
    UDP.write(NTPBuffer, NTP_PACKET_SIZE);
    UDP.endPacket();
}

inline int
getSeconds(uint32_t UNIXTime)
{
    return UNIXTime % 60;
}

inline int
getMinutes(uint32_t UNIXTime)
{
    return UNIXTime / 60 % 60;
}

inline int
getHours(uint32_t UNIXTime)
{
    return UNIXTime / 3600 % 24;
}


void
ntp_timer(void)
{
    unsigned long currentMillis = millis();
    if (currentMillis - prevNTP > intervalNTP)
    {   // if a minute has passed since last NTP request
        prevNTP = currentMillis;
        Serial.println("\r\nSending NTP request.....");
        sendNTPpacket(timeServerIP);    // send an NTP request
    }
    uint32_t time = getTime();  // check if an NTP response has arrived and get
                                // the (UNIX) time
    if(time)
    {
        timeUNIX = time;
        Serial.print("NTP response:\t");
        Serial.println(timeUNIX);
        lastNTPResponse = currentMillis;
    }
    else if((currentMillis - lastNTPResponse) > 3600000)
    {
        Serial.println("More than 1 hour since last NTP response. Rebooting..");
        Serial.flush();
        ESP.reset();
    }
    uint32_t actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
    if(actualTime != prevActualTime && timeUNIX != 0)
    {// if a second has passed since last print
        prevActualTime = actualTime;
        //Serial.printf("\rUTC time:\t%d:%d:%d ",getHours(actualTime),
                        //getMinutes(actualTime), getSeconds(actualTime));
    }
}
