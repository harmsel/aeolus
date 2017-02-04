#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <Ethernet.h>

/************************* WiFi Access Point *********************************/


#define WLAN_SSID "IOT"
#define WLAN_PASS "!HvAIOT!"
//#define WLAN_SSID       "DeMachine"
//#define WLAN_PASS       "88888888"

// voor de regelmatige updates van de server
const char* server = "http://supplement.nl/wind/aeolusData.php";
String laatsteUpdate = "firstrun";
int oudeMinuten = -99; // dan gaat hij na uploaden van code eerst een update halen

//host name voor MQTT
#define HOST_NAME "aeolus"

// tijd voor switch tussen OTA en programma draaien
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;
boolean mLedON = false;
bool updateOTA = 0; // met 0 staat hij standaard op NIET OTA updaten

// =================== overige variabelen ==========
#define PIN D1 //Datapin van ledstrip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
int windArray[200];
int startWindData;
String oudeWindString;
String windString;
int itemsInArray;

/************************* MQTT - Adafruit.io  *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "harmsel"
#define AIO_KEY         "6a9288dd7ff74fe688aecb2a283aacd2"

// ----  ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client; // or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// --- Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//---  Feeds ----------------------------------
// verander alleen achteraan de regel alleen naam /feeds/naam
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ota-onoff");
Adafruit_MQTT_Publish MQTT_debug = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/mqtt-debug");
Adafruit_MQTT_Publish verseWind = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/versewind");

//  ============= NTP TIJD server =============
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// ========================== SETUP =================================== //
void setup() {
  Serial.begin(115200);
   delay(10);
  strip.begin();
  WiFi.mode(WIFI_STA); //WIFI_AP = Access Point, STA = workSTA(tions).
  Serial.println("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    colorWipe(strip.Color(50, 0, 0), 0);
    delay(100);
    Serial.print("."); // geeft puntjes in de serial monitor tijdens inloggen
    colorWipe(strip.Color(0, 0, 0), 0);
   // ESP.restart();
  }
  Serial.print("...... WiFi connected");

  // Setup MQTT subscription for feed.
  mqtt.subscribe(&onoffbutton);

  OTAserver();

  colorWipe(strip.Color(0, 50, 0), 0);
  // ---- tijd dingen
  delay (200); // even geduld voordat hij die UDP shizzle goed heeft (dat helpt)
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(3000);
}

void loop() {
  OTAenMQTT();  // OTA of niet

}
