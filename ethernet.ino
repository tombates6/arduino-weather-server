#include <SPI.h>
#include <Ethernet.h>
#include <dht11.h>
#define DHT11PIN 7
#define lightPin A0

dht11 DHT11;
char line1[100];
char buffer[100];
byte mac[] = { 0xC0, 0x15, 0xEA, 0x51, 0xFC, 0x11 };
byte ip[] = { 192, 168, 1, 150 };

EthernetServer server = EthernetServer(80);
EthernetClient client;
void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);

  server.begin();
}

void readHeader();
int readRequestLine(char *line);
boolean pageNameIs(char* name);
//int valueOfParam(char param);
void sendBody();

void loop() {
  client = server.available();
  if (client){
    if(client.connected()) {
      readHeader();
      if(!pageNameIs("/")) {
        client.stop();
        return;
      }

      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/html"));
      client.println();
      sendBody();
      client.stop();
    }
  }

}

void readHeader() 
{
  readRequestLine(line1);
  while (readRequestLine(buffer) > 1 && buffer[0] != '\n') {}
}

int readRequestLine(char *line)
{
  char ch;
  int i = 0;
  while (ch != '\n' && i < 100 && client.available())
  {
    if (client.available())
    {
      ch = client.read();
      line[i] = ch;
      i ++;
    }
  }
  line[i] = '\0';
  Serial.println(line);
  return i;
}

boolean pageNameIs(char* name)
{
   // page name starts at char pos 4
   // ends with space
   int i = 4;
   char ch = line1[i];
   while (ch != ' ' && ch != '\n' && ch != '?')
   {
     if (name[i-4] != line1[i])
     {
       return false;
     }
     i++;
     ch = line1[i];
   }
   return true;
}

void sendBody()
{
  
  int chk = DHT11.read(DHT11PIN);
  double celsius = DHT11.temperature;
  double fahrenheit = ((9 * celsius) / 5) + 32;
  
  String currentLight;
  int currentLightReading = analogRead(lightPin);      
  if(currentLightReading < 300)currentLight = "Sunny";
  else if(currentLightReading < 750) currentLight = "Overcast";
  else currentLight = "Night";
  
  client.println(F("<html><body>"));
  client.println(F("<h1>Weather in Driggs</h1>"));
  
  client.println(F("<table border = 1>"));

  //temperature
  client.print(F("<tr><td>Temperature(F)"));
  client.print(F("</td><td>")); client.print((float)fahrenheit, 2);
  client.println(F("</td></tr>"));

//humidity
  client.print(F("<tr><td>Humidity (%)"));
  client.print(F("</td><td>")); client.print((float)DHT11.humidity, 2);
  client.println(F("</td></tr>"));

  //light index
  client.print(F("<tr><td>Light Index"));
  client.print(F("</td><td>")); client.print(currentLight);
  client.println(F("</td></tr>"));
  

  client.println(F("</table>"));
  client.println(F("</body></html>"));
}
