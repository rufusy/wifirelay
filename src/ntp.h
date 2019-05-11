
/*#ifndef _NTP_H
#define _NTP_H
#include <WiFiUdp.h>


const char* NTPServerName = NULL;
const int NTP_PACKET_SIZE;
byte NTPBuffer[NTP_PACKET_SIZE];

unsigned long intervalNTP;
unsigned long prevNTP;
unsigned lastNTPResponse;
uint32_t timeUNIX;
unsigned long prevActualTime;

void startUDP(void);
uint32_t getTime(void);
void sendNTPpacket(IPAddress& address);
inline int getSeconds(uint32_t UNIXTime);
inline int getMinutes(uint32_t UNIXTime);
inline int getHours(uint32_t UNIXTime);
void ntp_timer(void);


#endif
*/
