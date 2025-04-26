// /*-------- WiFi Connection Settings -------------*/
// #define ssid1   "ModiInfonet_PKD"
// #define pwd1    "@##pankaj##@"

// #define ssid2    "Bajrangbali2"
// #define pwd2    "11JaiShreeRam11"

// #define ssid3   "Pankaj's iPhone"
// #define pwd3    "iPhone91"

// #define ssid5    "Bajrangbali5"
// #define pwd5    "11JaiShreeRam11"

// #define ssid7   "Redmi Note 4"
// #define pwd7    "ILPatil64"


/*-------- Important IOT Device Settings ---------*/
#define deviceName        "Test Device" //Specify the name to be seen on network
#define firmwareVersion   "1.0.0" //Specify the firmware version
#define MQTT_USERNAME     "test_mqtt"
#define MQTT_KEY          "test_pass"
// #define SERVER         "192.168.0.111" //"192.168.1.105"
#define SERVERPORT        1883
#define RPI_HTTP_PORT     5000 //Port for RPI HTTP server

// EEPROM Layout
#define EEPROM_SIZE       512 //Size of EEPROM in bytes
#define REGISTRATION_FLAG_ADDR 65
#define UUID_START_ADDR   66 //Starting address for UUID
#define UUID_LENGTH       36 //Size of UUID in bytes

#define PREAMBLE          "parksense/"
#define MESSAGES          "feeds/messages/" //for specific messages related to batches
// #define PARKINDCTR        "sensor/parkindicator"
#define DISTANCE          "sensor/distance/"
#define PARKSLOTSTATUS    "sensor/parkslotstatus/"

#define ACCESS_POINT_NAME "ESP-Access-Point"
#define ACCESS_POINT_PWD  "1234567890" //Password for access point

#define OTA_USERNAME     "admin" //OTA username
#define OTA_PASSWORD     "ota_pass" //OTA password

#define LED          2
#define trigP        5
#define echoP        4
#define parkFree     12
#define parkOcc      13