#include "server.h"

File fsUploadFile;  // file object. store the received file temporarily
ESP8266WebServer server(80);    // webserver object. listen for http request on port 80

/* wifi credentials */
const char* ssid = "rufusy";
const char* password = "password.org";

void _wifi_init(void)
{
  // put your setup code here, to run once:
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

void _spiffs_init(void)
{
    SPIFFS.begin();   // start theh SPI Flash Files System
}

void _server_init(void)
{
      server.on("/",_handle_Root);
      server.on("/upload", HTTP_GET,[](){
          if(!_handle_file_read("/upload.html"))
          server.send(404, "text/plain", "404: Not found error");
      });

      server.on("/upload", HTTP_POST,[]()
        {server.send(200);},
        _handle_file_upload
      );
      server.onNotFound([](){
          if(!_handle_file_read(server.uri()))
            server.send(404, "text/plain", "404: Not Found error");
      });

      //server.onNotFound(_handle_Not_Found);

      server.begin();
      Serial.println("HTTP server started");
}

void _handle_Root(void)
{
    // send HTTP status 200 (ok)
    // send some text to the browser
    server.send(200, "text/plain", "hello world");
}

void _handle_Not_Found(void)
{
    // send HTTP status 404 (NOT FOUND) when there's no handler
    // for the URI in the request
    server.send(404, "text/plain", "404: Not Found");
}

void _handle_client(void)
{
    server.handleClient();
}


/*
    convert the file extension to the MIME type
*/
String _get_content_type(String filename)
{
    if(filename.endsWith(".html"))
        return "text/html";
    else if(filename.endsWith(".css"))
        return "text/css";
    else if(filename.endsWith(".js"))
        return "application/javascript";
    else if(filename.endsWith(".ico"))
        return "image/x-icon";
    else if(filename.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}


/*
    send the right file to the client (if it exits)
*/
bool _handle_file_read(String path)
{
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/"))
        path += "index,html";   // send the index file when a folder is requested
    String contentType = _get_content_type(path);  // get the MIME type
    String pathWithGz = path + ".gz";   // check if path contains compressed file
    // check if file exits as a compressed archive or normal
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
    {
        if(SPIFFS.exists(pathWithGz))
            path += ".gz";  // use compressed version of file if available
        File file = SPIFFS.open(path,"r");  // open the file in read mode
        size_t sent = server.streamFile(file, contentType); // send it to the client
        file.close();   // close the file
        Serial.println(String("\tsent file: ")+ path);
        return true;
    }
    Serial.println(String("\tfile not found: ")+ path); // return false if file doesn't exist
    return false;
}

/*
    upload a new file to the spiffs
*/
void _handle_file_upload(void)
{
    HTTPUpload& upload = server.upload();
    if(upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if(!filename.startsWith("/"))
            filename = "/"+filename;
        Serial.print("handleFileUpload name: ");
        Serial.println(filename);
        fsUploadFile = SPIFFS.open(filename, "w");  // open the file for writing. create if absent
        filename = String();
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(fsUploadFile)
            fsUploadFile.write(upload.buf, upload.currentSize); // write the received bytes to the file
    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        if(fsUploadFile) // if file was successfuly created
        {
            fsUploadFile.close();   // close the file again
            Serial.print("handleFileUpload size: ");
            Serial.println(upload.totalSize);
            server.sendHeader("Location","/success.html"); // rdirect to the success page
            server.send(303);
        }
        else
            server.send(500,"text/plain", "500: couldn't create file");
    }
}
