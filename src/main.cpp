#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <WakeOnLan.h>

// Wifi
WiFiUDP UDP;
const char *ssid = "mywifi";
const char *password = "mywifi";
unsigned long previousMillis = 0;
unsigned long interval = 30000;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

// Magic packet
ESP8266WebServer server(12345);
WakeOnLan WOL(UDP);
const char *MAC = "00:00:00:00:00:00";

// Blinker
bool lastStatus = false;
unsigned long previousBlinkMillis = 0;

void blink(int times, int offTime, int onTime)
{
  digitalRead(LED_BUILTIN) == HIGH ? lastStatus = false : lastStatus = true;

  for (int i = 0; i < times; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(offTime);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(onTime);
  }

  if (lastStatus == false)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
}

void blinkPeriod(int times, int period) 
{ 
  blink(times, period / 2, period / 2); 
}

void initWifi()
{
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.printf("\nTrying to connect to %s", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    blinkPeriod(10, 100);
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void onWifiConnect(const WiFiEventStationModeGotIP &event)
{
  Serial.println("\nConnected to WiFi!\nSSID: " + String(WiFi.SSID()) + "\nIP address: " + event.ip.toString());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event)
{
  Serial.println("\nDisconnected from WiFi");
  WiFi.disconnect();

  initWifi();
}

void wakePC()
{
  blinkPeriod(8, 100);
  WOL.sendMagicPacket(MAC);
  delay(1000);
  server.send(200, "text/plain", "Magic Packet sent to " + String(MAC));
}

void setup()
{
  // Initialization
  pinMode(LED_BUILTIN, OUTPUT);
  blinkPeriod(15, 100);

  WOL.setRepeat(3, 100);
  Serial.begin(9600);

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  initWifi();

  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  server.begin();
  server.on("/wake", wakePC);
}

void loop()
{
  unsigned long currentMillis = millis();

  server.handleClient();

  if (currentMillis - previousBlinkMillis >= interval && WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    previousBlinkMillis = currentMillis;
  }
  delay(900);
}