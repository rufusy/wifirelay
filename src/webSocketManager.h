#ifndef _WEB_SOCKET_MANAGER_H
#define _WEB_SOCKET_MANAGER_H

#include "main.h"



void webSocketinit(void);
void webSocketLoop(void);
void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);
void webSocketSend(void);

#endif
