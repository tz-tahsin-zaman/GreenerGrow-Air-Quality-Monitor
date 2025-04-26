#include <SoftwareSerial.h>

#include <stdlib.h>

/***********************/

/* change with your data*/

String apiKey = "IJOTGCPL1F0KRV0M"; // Change with your apikey

String ssid = "Rain"; // change with your wifi name

String password = "rain@2022";// Change with your wifi password

/***********************/

int LM35InputPin = 0; //select the input pin for temperature sensor LM35

SoftwareSerial ser(2, 3); // TX, RX

int SensorVal = 0; // variable to store the value from the sensor

void setup()

{

Serial.begin(9600);

ser.begin(115200);

ser.println("AT");

delay(2000);

if(ser.find("OK"))

{

Serial.println("AT response is OK");

while(connectWiFi() == false);

}

}

void loop()

{

SensorVal = analogRead(LM35InputPin);
float TempVal = (0.0049*SensorVal)*100;
char buf[10];
String strTemp = dtostrf(TempVal, 4, 1, buf);
Serial.println(strTemp);
String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";
ser.println(cmd);
delay(2000);
if(ser.find("Error"))
{
Serial.println("AT+CIPSTART error");
return;
}



String SndStr = "GET /update?api_key=";
SndStr += apiKey;
SndStr +="&field1=";
SndStr += String(strTemp);
SndStr += "\r\n\r\n";
cmd = "AT+CIPSEND=";
cmd += String(SndStr.length());
ser.println(cmd);
delay(5000);
if(ser.find(">"))
{
ser.print(SndStr);
}
else
{
ser.println("AT+CIPCLOSE");
Serial.println("AT+CIPCLOSE");
}
delay(5000);
}
boolean connectWiFi(void)
{
ser.println("AT+CWMODE=3");
delay(5000);
String cmd="AT+CWJAP=\"";
cmd+=ssid;
cmd+="\",\"";
cmd+=password;
cmd+="\"";
ser.println(cmd);
delay(2000);
if(ser.find("OK"))
{
Serial.println("wifi connection OK");
return true;
}else
{
Serial.println("wifi connection error");
return false;
}
}
