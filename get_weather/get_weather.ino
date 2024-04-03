#include <WiFi.h>
#include "ArduinoJson.h"
const char* ssid     = "310"; // Change this to your WiFi SSID
const char* password = "88888888"; // Change this to your WiFi password
const char *host = "opendata.cwb.gov.tw";

const char *dataid = "D0047-053";
const char *privateKey = "CWA-09E1CF46-1C3E-49FF-920C-3E0B6FE2B338";
const char *locationName = "東區";
String authorization = "CWB-4E884048-6F63-4D56-AA33-D37CD194C120";
void get_Weather();

// The default example accepts one data filed named "field1"
// For your own server you can ofcourse create more of them.
int field1 = 0;

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out

void setup()
{
    Serial.begin(115200);
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
void loop(){
  get_Weather();
}
void get_Weather(){
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // Create a URI for the request
  String url = "https://opendata.cwb.gov.tw/api/v1/rest/datastore/";
  url += dataid;
  url += "?Authorization=";
  url += privateKey;
  url += "&locationName=";
  url += locationName;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}
