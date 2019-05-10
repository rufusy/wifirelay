#ifndef _SERVER_H
#define _SERVER_H
#include "main.h"

void wifiInit(void);
void serverInit(void);
void handleClient(void);
String getContentType(String filename);
bool handleFileRead(String path);

#endif
