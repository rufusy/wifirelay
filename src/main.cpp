#include "server.h"
#include "webSocketManager.h"
#include "sanitize.h"
#include <WiFiUdp.h>


/**********************************************************\
    NTP functionality
\**********************************************************/
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
uint32_t actualTime;
int hour,minute,second;

void startUDP(void);
uint32_t getTime(void);
void sendNTPpacket(IPAddress& address);
inline int getSeconds(uint32_t UNIXTime);
inline int getMinutes(uint32_t UNIXTime);
inline int getHours(uint32_t UNIXTime);
void ntp_timer(void);


/**********************************************************\
    Relay timer functionality
\**********************************************************/
int relay1_timer_config[4] = {0,0,0,0}; 
int relay2_timer_config[4] = {0,0,0,0};
int relay3_timer_config[4] = {0,0,0,0};
int relay4_timer_config[4] = {0,0,0,0};
int relay5_timer_config[4] = {0,0,0,0};

int relay1_on=0,
    relay1_off=0;
   
void relay1_timer(void);
void relay2_timer(void);
void relay3_timer(void);
void relay4_timer(void);
void relay5_timer(void);


void
setup()
{
    wifiInit();
    startUDP();
    serverInit();
    webSocketinit();
}


void
loop()
{
    handleClient();
    webSocketLoop();
    webSocketSend();
    ntp_timer();
    sanitize_serial_in('.', serial_in_cfg, sanitize_serial_in_cfg); 
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay1") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', sizeof(sanitize_serial_in_cfg[1]));
        relay1_timer();
    }
/*
    if( relay1_timer_config[0] == hour && relay1_timer_config[1] == minute)
        Serial.print("+relay1.high*");
    if( relay1_timer_config[2] == hour && relay1_timer_config[3] == minute)
        Serial.print("+relay1.low*");*/
        //Serial.printf("\rNetwork time->\t%d:%d:%d",hour,minute,second);
        //Serial.println("");
      /*  Serial.printf("\rrelay1 timer->\ton:%d:%d off:%d:%d", 
                    relay1_timer_config[0],
                    relay1_timer_config[1],
                    relay1_timer_config[2],
                    relay1_timer_config[3]);*/ 

        if(relay1_on==1)
        {
            if(relay1_timer_config[0] == hour && relay1_timer_config[1] == minute)
            {
                Serial.print("+relay1.high*");
                relay1_on= 0; 
            }
        }

        if(relay1_off==1)
        {
            if(relay1_timer_config[2] == hour && relay1_timer_config[3] == minute)
            {
                Serial.print("+relay1.low*");
                relay1_off= 0;
            }
        }


 /*   if(strcmp((char*)sanitize_serial_in_cfg[1],"relay2") == 0)
        relay2_timer(); 
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay3") == 0)
        relay3_timer(); 
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay4") == 0)
        relay4_timer();*/
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
    {   // Get the IP address of the NTP server
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
    second = UNIXTime % 60;
    return 1;
}

inline int
getMinutes(uint32_t UNIXTime)
{
    minute= UNIXTime / 60 % 60;
    return 1;
}


inline int
getHours(uint32_t UNIXTime)
{
    hour= UNIXTime / 3600 % 24;
    return 1;
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
    actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
    if(actualTime != prevActualTime && timeUNIX != 0)
    {   // if a second has passed since last print
        prevActualTime = actualTime;
        getSeconds(actualTime);  
        getMinutes(actualTime);  
        getHours(actualTime);
    }
}



void
relay1_timer(void)
{   //t.relay1.01:59.12:00
/*    sanitize_on_timer(':',sanitize_serial_in_cfg[2]);  
    relay1_timer_config[0] = atoi(sanitize_timer_cfg[0]);
    relay1_timer_config[1] = atoi(sanitize_timer_cfg[1]);
    sanitize_off_timer(':',sanitize_serial_in_cfg[3]);
    relay1_timer_config[2] = atoi(sanitize_timer_cfg[0]);
    relay1_timer_config[3] = atoi(sanitize_timer_cfg[1]);*/

    // get relay on time
    sanitize_serial_in(':',sanitize_serial_in_cfg[2],sanitize_timer_cfg); 
    relay1_timer_config[0] = atoi(sanitize_timer_cfg[0]);
    relay1_timer_config[1] = atoi(sanitize_timer_cfg[1]);
    // get relay off time
    sanitize_serial_in(':',sanitize_serial_in_cfg[3],sanitize_timer_cfg);
    relay1_timer_config[2] = atoi(sanitize_timer_cfg[0]);
    relay1_timer_config[3] = atoi(sanitize_timer_cfg[1]);

    relay1_on=1;
    relay1_off=1;
}
/*
void
relay2_timer(void)
{
    Serial.print("Relay2");
}

void
relay3_timer(void)
{
    Serial.print("Relay3");

}

void
relay4_timer(void)
{
    Serial.print("Relay4");

}

void
relay5_timer(void)
{
    Serial.print("Relay5");

}*/