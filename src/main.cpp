#include <Arduino.h>
#include "server.h"
#include <WebSocketsServer.h>


WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t, WStype_t, uint8_t *, size_t);


void setup()
{
  _wifi_init();
  _spiffs_init();
  _server_init();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
    webSocket.loop();
    _handle_client();
    if(Serial.available() > 0)
    {
        char c[] = {(char)Serial.read()};
        webSocket.broadcastTXT(c, sizeof(c));
    }
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
    if(type == WStype_TEXT)
    {
        for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
        Serial.println();
    }
}
