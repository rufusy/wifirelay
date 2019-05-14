#include "webSocketManager.h"

WebSocketsServer webSocket = WebSocketsServer(81);


const char ch1_on[] = "+ch1.on*";
const char ch1_off[] = "+ch1.off*";
const char ch2_on[] = "+ch2.on*";
const char ch2_off[] = "+ch2.off*";
const char ch3_on[] = "+ch3.on*";
const char ch3_off[] = "+ch3.off*";
const char ch4_on[] = "+ch4.on*";
const char ch4_off[] = "+ch4.off*";
const char ch5_on[] = "+ch5.on*";
const char ch5_off[] = "+ch5.off*";

const char relay1_on[] = "+relay1.on*";
const char relay1_off[] = "+relay1.off*";
const char relay2_on[] = "+relay2.on*";
const char relay2_off[] = "+relay2.off*";
const char relay3_on[] = "+relay3.on*";
const char relay3_off[] = "+relay3.off*";
const char relay4_on[] = "+relay4.on*";
const char relay4_off[] = "+relay4.off*";
const char relay5_on[] = "+relay5.on*";
const char relay5_off[] = "+relay5.off*";

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
            //if (strcmp(ch1_output, (const char *)payload) == 0) dirStatus = true;
            //if (strcmp(ch1_input, (const char *)payload) == 0) dirStatus = false;
            //if (strcmp(ch1_high, (const char *)payload) == 0) wakeUpStatus = true;
            //if (strcmp(ch1_low, (const char *)payload) == 0) wakeUpStatus = false;

            // send the payload to all connected clients
            webSocket.broadcastTXT(payload, length);

            if(strcmp("+ch1.high*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch1_on,strlen(ch1_on));
            if(strcmp("+ch1.low*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch1_off,strlen(ch1_off));

            if(strcmp("+ch2.high*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch2_on,strlen(ch2_on));
            if(strcmp("+ch2.low*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch2_off,strlen(ch2_off));

            if(strcmp("+ch3.high*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch3_on,strlen(ch3_on));
            if(strcmp("+ch3.low*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch3_off,strlen(ch3_off));

            if(strcmp("+ch4.high*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch4_on,strlen(ch4_on));
            if(strcmp("+ch4.low*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch4_off,strlen(ch4_off));

            if(strcmp("+ch5.high*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch5_on,strlen(ch5_on));
            if(strcmp("+ch5.low*",(const char*)payload) == 0)
                webSocket.broadcastTXT(ch5_off,strlen(ch5_off));


                if(strcmp("+relay1.high*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay1_on,strlen(relay1_on));
                if(strcmp("+relay1.low*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay1_off,strlen(relay1_off));

                if(strcmp("+relay2.high*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay2_on,strlen(relay2_on));
                if(strcmp("+relay2.low*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay2_off,strlen(relay2_off));

                if(strcmp("+relay3.high*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay3_on,strlen(relay3_on));
                if(strcmp("+relay3.low*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay3_off,strlen(relay3_off));

                if(strcmp("+relay4.high*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay4_on,strlen(relay4_on));
                if(strcmp("+relay4.low*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay4_off,strlen(relay4_off));

                if(strcmp("+relay5.high*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay5_on,strlen(relay5_on));
                if(strcmp("+relay5.low*",(const char*)payload) == 0)
                    webSocket.broadcastTXT(relay5_off,strlen(relay5_off));

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
