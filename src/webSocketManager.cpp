#include "webSocketManager.h"
#include "sanitize.h"

WebSocketsServer webSocket = WebSocketsServer(81);

char response[12] = {'\0'};

void 
webSocketinit(void)
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void 
webSocketLoop(void)
{
    webSocket.loop();
}


void 
webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
    switch(type)
    {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\r\n", num);
        break;
        case WStype_CONNECTED:
        {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        }
        break;
        case WStype_TEXT:
            // send the payload to all connected clients
            webSocket.broadcastTXT(payload, length);
            // timer configuration is sent as: t.relay1.01:59.12:00
            if((char)payload[0] == 't') // save timer configuration on server
            {
                get_serial_in((char*)payload);
                //Serial.print("timer");
            }
            else 
            {
                // send the payload over to the TIVA board
                for(size_t i = 0; i < length; ++i)
                    Serial.print((char) payload[i]);
            }
        break;
    }
}


/*
*   Read updates from tiva. Send them to the client
*/
void webSocketSend(void)
{
    if(Serial.available()>0)
    {
        size_t response_len = 0, i;
        char input = Serial.read();
        if (input != '\n')
            response[response_len++] = input;
        else 
        {
            webSocket.broadcastTXT(response, strlen(response));
            for (i = 0; i < response_len; ++i)
                response[i] = '\0'; 
        }
    }
}
