#include <dht11.h>
#define DHT11PIN 7
#define lightPin A0

dht11 DHT11;

void setup()
{
  Serial.begin(9600);
 
}

void loop()
{
  Serial.println();

  int chk = DHT11.read(DHT11PIN);
  
  double celsius = DHT11.temperature;
  double fahrenheit = ((9 * celsius) / 5) + 32;

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (F): ");
  Serial.println((float)fahrenheit, 2);
  
  String currentLight;

  int currentLightReading = analogRead(lightPin);
  if(currentLightReading < 300)currentLight = "Sunny";
  else if(currentLightReading < 750) currentLight = "Overcast";
  else currentLight = "Night";
    
  Serial.print("Light Index: ");
  Serial.println(currentLight);

  delay(2000);

}
