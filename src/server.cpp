#include "server.h"

ESP8266WebServer server(80);    // webserver object. listen for http request on port 80

/* wifi credentials */
const char* ssid = "rufusy";
const char* password = "password.org";

/*
*
*/
void wifiInit(void)
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Connecting.....");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("....");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(++i);
    Serial.print("");
  }

  Serial.println("Connection is established!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void serverInit(void)
{
    SPIFFS.begin();   // start the SPI Flash Files System
    /* called when url is not defined. */
    /* load content from SPIFFS */
    server.onNotFound([](){
        if(!handleFileRead(server.uri()))
        server.send(404, "text/plain", "File Not Found!");
    });
    server.begin();
    Serial.println("HTTP server started");
}

void handleClient(void)
{
    server.handleClient();
}

/*
    convert the file extension to the MIME type
*/
String getContentType(String filename)
{
    if (filename.endsWith(".htm")) return "text/html";
    else if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".png")) return "image/png";
    else if (filename.endsWith(".gif")) return "image/gif";
    else if (filename.endsWith(".jpg")) return "image/jpeg";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".xml")) return "text/xml";
    else if (filename.endsWith(".pdf")) return "application/x-pdf";
    else if (filename.endsWith(".zip")) return "application/x-zip";
    else if (filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}


/*
    send the right file to the client (if it exits)
*/
bool handleFileRead(String path)
{
    //Serial.println("handleFileRead: " + path);
    if (path.endsWith("/"))
        path += "index.html";   // send the index file when a folder is requested
    String contentType = getContentType(path);  // get the MIME type
    String pathWithGz = path + ".gz";   // check if path contains compressed file
    // check if file exits as a compressed archive or normal
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
    {
        if(SPIFFS.exists(pathWithGz))
            path += ".gz";  // use compressed version of file if available
        File file = SPIFFS.open(path,"r");  // open the file in read mode
        size_t sent = server.streamFile(file, contentType); // send it to the client
        file.close();   // close the file
        //Serial.println(String("\tsent file: ")+ path);
        return true;
    }
    Serial.println(String("\tfile not found: ")+ path); // return false if file doesn't exist
    return false;
}
