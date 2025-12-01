#ifndef definitions_h
#define definitions_h

#include <Arduino.h>
#include "webupdater.h" //Add Web Update feature instead of OTA
#include "callback.h"
#include "connectSubscribe.h"
#include "timer.h"
#include "defines.h"
#include "publish.h"
#include "registerDevice.h"
#include "config.h"
// #include <WiFiManager.h>

//--------------------Ultrasonic---------------------
unsigned long duration;
unsigned int distance, prevDistance=0;
//---------------------------------------------------

unsigned long tkeepUS = 500, tkeepUSpub = 500, tkeepRssi = 1000;
unsigned long tkeepConnect=0, lastReconnectAttempt=0;
unsigned int rssiTime = 60000;
unsigned int connectTime = 5000;
unsigned int usTime = 5000, uspubTime = 10000;

char valueStr[100];
// char SERVER[16] = "192.168.0.111";
char SERVER[16] = "io.adafruit.com";
String incoming[100];
uint8_t wifiReconnectAttemptCount=0;

// Parameters from intial setup
char rpiServer[RPI_IP_SIZE];
char deviceName[DEVICE_NAME_SIZE] = "Hydroponic Timer"; //Specify the name to be seen on network
bool configFreshlySaved = LOW;
char mqttUsername[MQTT_USERNAME_SIZE] = "patilect";
char mqttKey[MQTT_KEY_SIZE] = "d288fe61159d4fb084882b7de8a13ca6";
char onTimeFeed[FEED_SIZE] = "feeds/ontime";
char offTimeFeed[FEED_SIZE] = "feeds/offtime";
char msgsFeed[FEED_SIZE] = "feeds/messages";

const char* firmwareURL = "https://raw.githubusercontent.com/pankajpatil001/hydroponic-timer/master/firmwares/nodemcuv2.bin";

bool wifiConnected = LOW;
bool mqttConnected = LOW;
char deviceUUID[UUID_LENGTH]; // UUID for the device
String defaultUUID = "parksense-default-iot-uuid"; // Default UUID for the device
//--------------------------------------------------------------------------------
bool firstTime = HIGH, connection = LOW;
//-----------------------------------------------------------------------------------------------

bool waterPumpOn = false;
unsigned int onTime = 60, offTime = 600, testTime = 10; //seconds
unsigned int timerTime = 990; //ms
unsigned long tkeepTimer = 3000, timer = 0; // start after 3 secs

// bool parkSpaceOccupied = LOW;
// unsigned int parkSpaceVehicleDistance = 50; //in cm
// unsigned int minVehDistance = 20, maxVehDistance = 250; //in cm
//-----------------------------Webupdater---------------------------------

const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='30%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>Patilect Smart Solutions</b></font></center>"
                "<center><font size=3><b>Hydroponic Timer</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='Patilect' && form.pwd.value=='SmartSolutions')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

WiFiClient wificlient;
WiFiClientSecure wificlientsecure;
// create MQTT object
PubSubClient client(wificlient);
// const char* host = "esp8266-webupdate";
HTTPClient rpihttp; //HTTP client for RPI server
bool serial = HIGH;

// ESP8266WiFiMulti wifiMulti;
ESP8266WebServer httpServer(80); //Choose any number you want, just add this after your ip adrs
ESP8266HTTPUpdateServer httpUpdater; //http://<local-ip>:<any-number>/ as server address

//-------------------------------------------------------


#endif