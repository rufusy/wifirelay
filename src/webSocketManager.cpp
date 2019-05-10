#include "webSocketManager.h"

WebSocketsServer webSocket = WebSocketsServer(81);

const char ch1_output[] = "+ch1.output*";
const char ch1_input[] = "+ch1.input*";
const char ch1_high[] = "+ch1.high*";
const char ch1_low[] = "+ch1.low*";

bool dirStatus;
bool wakeUpStatus;


void webSocketinit(void)
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void webSocketLoop(void)
{
    webSocket.loop();
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
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

            // Send the board's operating status and the configured settings
            //if(dirStatus) webSocket.sendTXT(num, ch1_output, strlen(ch1_output));
            //if(!dirStatus) webSocket.sendTXT(num, ch1_input, strlen(ch1_input));
            //if(wakeUpStatus) webSocket.sendTXT(num, ch1_output, strlen(ch1_high));
            //if(!wakeUpStatus) webSocket.sendTXT(num, ch1_output, strlen(ch1_low));
        }
        break;

        case WStype_TEXT:

            // save the new configuration to be used by new connections
            if (strcmp(ch1_output, (const char *)payload) == 0) dirStatus = true;
            if (strcmp(ch1_input, (const char *)payload) == 0) dirStatus = false;
            if (strcmp(ch1_high, (const char *)payload) == 0) wakeUpStatus = true;
            if (strcmp(ch1_low, (const char *)payload) == 0) wakeUpStatus = false;

            // send the payload to all connected clients
            webSocket.broadcastTXT(payload, length);

            // send the payload over to the TIVA board
            for(size_t i = 0; i < length; ++i)
                Serial.print((char) payload[i]);

        break;

    }
}


/*
*   Read updates from tiva. Send them to the browser
*/
void webSocketSend(void)
{
    if(Serial.available() > 0)
    {
        char c[] = {(char)Serial.read()};
        webSocket.broadcastTXT(c, sizeof(c));
    }
}
