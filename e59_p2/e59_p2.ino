/*************************************************************************************************************************************************
 *  TITLE: Using IFTTT For Automation. PIR Example
 *
 *  By Frenoy Osburn
 *  YouTube Video: https://youtu.be/pYa8IHUZJkw
 *************************************************************************************************************************************************/

  /********************************************************************************************************************
 *  Board Settings:
 *  Board: "WeMos D1 R1 or Mini"
 *  Upload Speed: "921600"
 *  CPU Frequency: "80MHz"
 *  Flash Size: "4MB (FS:@MB OTA:~1019KB)"
 *  Debug Port: "Disabled"
 *  Debug Level: "None"
 *  VTables: "Flash"
 *  IwIP Variant: "v2 Lower Memory"
 *  Exception: "Legacy (new can return nullptr)"
 *  Erase Flash: "Only Sketch"
 *  SSL Support: "All SSL ciphers (most compatible)"
 *  COM Port: Depends *On Your System*
 *********************************************************************************************************************/

#include <ESP8266WiFi.h>

const char* ssid = "NETWORK";             
const char* password = "PASSWORD";     

const char* apiKey = "API_KEY";
const char* eventName = "pir_status";
const char* host = "maker.ifttt.com";
const int port = 80;

int pirPin = D2;

void setup() 
{  
  Serial.begin(115200);
  Serial.println("\n\nIFTTT Test\n");
  WiFi.begin(ssid, password);

  Serial.print("Connecting to network");
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);    
    if (++counter > 100) 
      ESP.restart();
    Serial.print( "." );
  }
  Serial.println("\nWiFi connected");
  pinMode(pirPin, INPUT);
  Serial.println("All configured!");
}

void loop() 
{
  if(digitalRead(pirPin) == HIGH)
  {    
    Serial.print("Connecting to ");
    Serial.print(host);
    Serial.print(':');
    Serial.println(port);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host, port)) 
    {
      Serial.println("connection failed");
      delay(5000);
      return;
    }

    // This will send the trigger to the server
    if (client.connected()) 
    {
      String url = "/trigger/";
      url += eventName;
      url += "/with/key/";
      url += apiKey;
      
      Serial.println("Sending trigger to server");
      Serial.println(url);
      
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" + 
             "Content-Type: application/x-www-form-urlencoded\r\n" + 
             "Content-Length: 0\r\n\r\n");
    }

    Serial.println("Trigger sent. Waiting for a bit...");
    delay(1*60*1000); // execute once every minute, don't flood remote service
    Serial.println("Ready for new trigger");
  }
}
