#include <navestidla_esp.h>
#include <arduino-timer.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include "setup.h"

StaticJsonDocument<64> payloadOnline;

// SIGNALS ---------------------
#define DEBUG true
#define BUS_FOR_SIGNALS 0

const static int COUNT_NAVESTIDLA = 1;
Navestidlo_element navestidla[COUNT_NAVESTIDLA] = {
    // TODO - here is the possibility to define more signals
    Navestidlo_element("KL", 1, &n11111_010_pozice_2),
};
auto navestidla_esp = Navestidla_esp(navestidla, COUNT_NAVESTIDLA, DEBUG, BUS_FOR_SIGNALS);
auto esp_timer = timer_create_default();

// NETWORKING ---------------------
const char *ssid = SSID;
const char *password = PASSWORD;
const char *mqtt_server = MQTT_SERVER;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    esp_timer.every(LIVE_BIT_DELAY, send_live_bits, &navestidla_esp);

    setup_wifi();

    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(callback);
    mqttClient.setKeepAlive(3);

    char *mqtt_topic = (char *)"errors/be";
    navestidla_esp.set_mqtt_client_for_errors(&mqttClient, mqtt_topic);
    navestidla_esp.init_all_desks();
}

void loop()
{
    if (!mqttClient.connected())
    {
        reconnect();
    }
    ArduinoOTA.handle();
    mqttClient.loop();

    esp_timer.tick();
}

void setup_wifi()
{
    delay(10);
    // We start by connecting to a WiFi network
    if (DEBUG)
    {
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    randomSeed(micros());

    ArduinoOTA.setHostname(OTANAME);
    ArduinoOTA.setPassword(OTAPASS);

    ArduinoOTA.onStart([]()
                       {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });

    ArduinoOTA.begin();

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    while (!mqttClient.connected())
    {
        if (DEBUG) Serial.print("Attempting MQTT connection...");

        String client = CLIENTID;

        String stringOne = STRONLINE;
        char charBuf[32];
        stringOne.toCharArray(charBuf, 32);

        payloadOnline["st"] = 0;
        char serializedPayload[64];
        serializeJson(payloadOnline, serializedPayload);

        if (mqttClient.connect(client.c_str(), mqtt_username, mqtt_password, charBuf, 1, true, serializedPayload))
        {

            payloadOnline["st"] = 1;
            serializeJson(payloadOnline, serializedPayload);

            mqttClient.publish(charBuf, serializedPayload);
            if (DEBUG) Serial.print("MQTT connected.");

            // ... and resubscribe
            mqttClient.subscribe(NAVESTIT);
        }
        else
        {
            if (DEBUG)
            {
                Serial.print("failed, rc=");
                Serial.print(mqttClient.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length){
    char *pld = (char *)payload;
    if (DEBUG) Serial.println(String((char*)payload));

    char *token;
    token = strtok(topic, "/");
    if ((token != NULL) && (strcmp(token, "navestit") == 0))
    {
        token = strtok(NULL, "/");
        if ((token != NULL) && (strcmp(token, CLIENTID) == 0))
        {
            char *id_navestidla = strtok(NULL, "/");
            navestidla_esp.handle_navestidlo(id_navestidla, pld, length);
        }
    }
}
