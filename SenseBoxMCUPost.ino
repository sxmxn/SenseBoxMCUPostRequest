#include <ArduinoHttpClient.h>
#include "SenseBoxMCU.h"


const long interval = 600000;
long time_start = 0;
long time_actual = 0;
float p10,p25;
char server[] = "ENTER SERVER";
String name = "SenseboxGreenScript";

WiFiClient client;

SDS011 my_sds(Serial1);

Bee* b = new Bee();


void setup() {
  b->connectToWifi("ENTER NAME OF WIFI","ENTER PASSWORD");
delay(1000);
  //Setup Serial Print
  Serial.begin(9600);

  Serial1.begin(9600);
}

void loop() {
  time_start = millis();
  if (time_start > time_actual + interval) {
    time_actual = millis();
     Serial.println("PM 2,5: ");
     Serial.println(my_sds.getPm25());
     float pm25 = my_sds.getPm25();
     Serial.println("PM 10: ");
     Serial.println(my_sds.getPm10());
     float pm10 = my_sds.getPm10();

     Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connectSSL(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:

    String PostData="pm2_5=";
    PostData=String(PostData + pm25);
    PostData=PostData+"&pm10=";
    PostData=String(PostData + pm10);
    PostData=PostData+"&name=";
    PostData=String(PostData+name);
    Serial.println(PostData);
    
    client.println("POST /api/v1/newSensebox HTTP/1.1");
    client.println("Host: bugfick.herokuapp.com");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    client.println("Connection: close");
    client.println();
  }
  }
  

}
