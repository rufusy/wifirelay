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
int relay1_on=  0,
    relay1_off= 0,
    relay2_on=  0,
    relay2_off= 0,
    relay3_on=  0,
    relay3_off= 0,
    relay4_on=  0,
    relay4_off= 0,
    relay5_on=  0,
    relay5_off= 0;
void get_timer_config(int size);
void save_timer_config(int relay_config[4], int relay);
void send_timer_action(void);


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
    get_timer_config(7);
    send_timer_action();
}



void
startUDP(void)
{
    //Serial.println("Starting UDP");
    UDP.begin(123); // start listening for UDP packets on port 123
 /*   Serial.print("Local port:\t");
    Serial.println(UDP.localPort());
    Serial.println();*/

    if(!WiFi.hostByName(NTPServerName, timeServerIP))
    {   // Get the IP address of the NTP server
        //Serial.println("DNS lookup failed. Rebooting...");
        //Serial.flush();
        ESP.reset();
    }
   /* Serial.print("Time server IP:\t");
    Serial.println("timeServerIp");
    Serial.println("\r\nSending NTP request....");*/
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
    return (UNIXTime + (uint32_t)10800);
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
        //Serial.println("\r\nSending NTP request.....");
        sendNTPpacket(timeServerIP);    // send an NTP request
    }
    uint32_t time = getTime();  // check if an NTP response has arrived and get
                                   // the (UNIX) time
    if(time)
    {
        timeUNIX = time;
      /*  Serial.print("NTP response:\t");
        Serial.println(timeUNIX);*/
        lastNTPResponse = currentMillis;
    }
  /*  else if((currentMillis - lastNTPResponse) > 3600000)
    {
        Serial.println("More than 1 hour since last NTP response. Rebooting..");
        Serial.flush();
        ESP.reset();
    }*/
    actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
    if(actualTime != prevActualTime && timeUNIX != 0)
    {   // if a second has passed since last print
        prevActualTime = actualTime;
        getSeconds(actualTime);  
        getMinutes(actualTime);  
        getHours(actualTime);
        //Serial.printf("\rNetwork time->\t%d:%d:%d",hour,minute,second);
    }
}


void 
get_timer_config(int size)
{
    sanitize_serial_in('.', serial_in_cfg, sanitize_serial_in_cfg); 

    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay1") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', size);
        save_timer_config(relay1_timer_config, 1);
    }
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay2") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', size);
        save_timer_config(relay2_timer_config, 2);
    }
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay3") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', size);
        save_timer_config(relay3_timer_config, 3);
    }
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay4") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', size);
        save_timer_config(relay4_timer_config, 4);
    }
    if(strcmp((char*)sanitize_serial_in_cfg[1],"relay5") == 0)
    {
        memset(sanitize_serial_in_cfg[1], '\0', size);
        save_timer_config(relay5_timer_config, 5);
    }
}


void 
save_timer_config(int relay_config[4], int relay)
{
    //01:59.12:00
    // get relay on time
    sanitize_serial_in(':',sanitize_serial_in_cfg[2],sanitize_timer_cfg); 
    relay_config[0] = atoi(sanitize_timer_cfg[0]);
    relay_config[1] = atoi(sanitize_timer_cfg[1]);
    // get relay off time
    sanitize_serial_in(':',sanitize_serial_in_cfg[3],sanitize_timer_cfg);
    relay_config[2] = atoi(sanitize_timer_cfg[0]);
    relay_config[3] = atoi(sanitize_timer_cfg[1]);
    // set action flags
    switch(relay)
    {
        case 1:
            relay1_on=1;
            relay1_off=1;
            break;
        case 2:
            relay2_on=1;
            relay2_off=1;
            break;
        case 3:
            relay3_on=1;
            relay3_off=1;
            break;
        case 4:
            relay4_on=1;
            relay4_off=1;
            break;
        case 5:
            relay5_on=1;
            relay5_off=1;
            break;
        default:
            Serial.println("Timer action flag not set");
    }
}

void 
send_timer_action(void)
{
    /* relay 1*/
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
    // relay 2
    if(relay2_on==1)
    {
        if(relay2_timer_config[0] == hour && relay2_timer_config[1] == minute)
        {
            Serial.print("+relay2.high*");
            relay2_on= 0; 
        }
    }
    if(relay2_off==1)
    {
        if(relay2_timer_config[2] == hour && relay2_timer_config[3] == minute)
        {
            Serial.print("+relay2.low*");
            relay2_off= 0;
        }
    }   

    // relay 3
    if(relay3_on==1)
    {
        if(relay3_timer_config[0] == hour && relay3_timer_config[1] == minute)
        {
            Serial.print("+relay3.high*");
            relay3_on= 0; 
        }
    }
    if(relay3_off==1)
    {
        if(relay3_timer_config[2] == hour && relay3_timer_config[3] == minute)
        {
            Serial.print("+relay3.low*");
            relay3_off= 0;
        }
    }   

    // relay 4
    if(relay4_on==1)
    {
        if(relay4_timer_config[0] == hour && relay4_timer_config[1] == minute)
        {
            Serial.print("+relay4.high*");
            relay4_on= 0; 
        }
    }

    if(relay4_off==1)
    {
        if(relay4_timer_config[2] == hour && relay4_timer_config[3] == minute)
        {
            Serial.print("+relay4.low*");
            relay4_off= 0;
        }
    }   

    // relay 5
    if(relay5_on==1)
    {
        if(relay5_timer_config[0] == hour && relay5_timer_config[1] == minute)
        {
            Serial.print("+relay5.high*");
            relay5_on= 0; 
        }
    }

    if(relay5_off==1)
    {
        if(relay5_timer_config[2] == hour && relay5_timer_config[3] == minute)
        {
            Serial.print("+relay5.low*");
            relay5_off= 0;
        }
    }
}
