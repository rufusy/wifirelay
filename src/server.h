#ifndef _SERVER_H
#define _SERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>


void _wifi_init(void);
void _server_init(void);
void _handle_Root(void);  // function prototypes for HTPP handlers
void _handle_Not_Found(void);
void _handle_client(void);
void _spiffs_init(void); // initialize spiffs
String _get_content_type(String filename); // convert file extension to the MIME type
bool _handle_file_read(String path);    // send the right file to the client (if it exits)
void _handle_file_upload(void); // upload a new file to SPIFFS

#endif
