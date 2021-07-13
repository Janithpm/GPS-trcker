#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>

TinyGPSPlus gps;
SoftwareSerial SIM900A(9, 10);
#define LED_PIN 5
double Lat, Lng;
unsigned long lastTime, delayTime = 1000;
boolean send = true;
String deviceId = "gps001"; // <----- change this

void printSerialData()
{
  while (SIM900A.available())
  {
    Serial.write(SIM900A.read());
  }
}

void sendData(double lat, double lng)
{
  SIM900A.println("AT+CGATT=1"); //to attach the terminal to GPRS service
  delay(200);
  printSerialData();

  SIM900A.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); // connection type
  delay(1000);
  printSerialData();

  SIM900A.println("AT+SAPBR=3,1,\"APN\",\"mobitel\""); // APN
  delay(2500);
  printSerialData();

  SIM900A.println();
  SIM900A.println("AT+SAPBR=1,1"); // Enable the GPRS
  delay(5000);
  printSerialData();

  SIM900A.println("AT+HTTPINIT"); // HTTP Init
  delay(1200);
  printSerialData();

  SIM900A.println("AT+HTTPPARA=\"URL\",\"http://anthelminthic-signa.000webhostapp.com/proxy.php?&deviceId=" + deviceId + "&lat=" + String(lat, 9) + "&lng=" + String(lng, 9) + "\"");
  delay(700); // GET Parameters
  printSerialData();

  SIM900A.println();
  SIM900A.println("AT+HTTPACTION=0"); // Submit GET Request
  delay(4000);
  printSerialData();

  SIM900A.println("AT+HTTPREAD=0,20"); // Read Data from website
  delay(1700);
  printSerialData();

  SIM900A.println("");
  delay(700);
  SIM900A.println("AT+HTTPTERM"); // close HTTP
  printSerialData();
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  SIM900A.begin(9600);
  delay(5000);
}

void loop()
{
  if (Serial.available() && gps.encode(Serial.read()))
  {
    if (gps.location.isValid())
    {
      Lat = gps.location.lat();
      Lng = gps.location.lng();

      if (Lat != 0.00 && Lng != 0.00)
      {
        if (send)
        {
          digitalWrite(LED_PIN, HIGH);
          sendData(Lat, Lng);
          digitalWrite(LED_PIN, LOW);
          lastTime = millis();
          send = false;
        }
      }
    }

    if (millis() - lastTime > delayTime)
    {
      send = true;
      lastTime = 0;
    }
  }
}
