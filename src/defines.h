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
// #define deviceName        "Test Device" //Specify the name to be seen on network
#define firmwareVersion   "1.0.0" //Specify the firmware version
// #define MQTT_USERNAME     "test_mqtt"
// #define MQTT_KEY          "test_pass"
// #define SERVER         "192.168.0.111" //"192.168.1.105"
#define SERVERPORT        1883
#define RPI_HTTP_PORT     5000 //Port for RPI HTTP server

// EEPROM Layout
#define EEPROM_SIZE       512 //Size of EEPROM in bytes
#define RPI_IP_ADDR      0 //Starting address for RPI IP address of size 16 bytes
#define RPI_IP_SIZE      16 //Size of RPI IP address in bytes
#define DEVICE_NAME_ADDR 16 //Starting address for device name of size 20 bytes
#define DEVICE_NAME_SIZE 20 //Size of device name in bytes
#define MQTT_USERNAME_ADDR 36 //Starting address for MQTT username of size 43 bytes
#define MQTT_USERNAME_SIZE 44 //Size of MQTT username in bytes
#define MQTT_KEY_ADDR    80 //Starting address for MQTT key of size 49 bytes
#define MQTT_KEY_SIZE    50 //Size of MQTT key in bytes
#define REGISTRATION_FLAG_ADDR 130 //Starting address for registration flag of size 1 byte
#define REGISTRATION_FLAG_SIZE 1 //Size of registration flag in bytes
#define UUID_START_ADDR   131 //Starting address for UUID of size 36 bytes
#define UUID_LENGTH       36 //Size of UUID in bytes

#define PREAMBLE          "parksense/"
#define MESSAGES          "feeds/messages/" //for specific messages related to batches
// #define PARKINDCTR        "sensor/parkindicator"
#define DISTANCE          "sensor/distance/"
#define PARKSLOTSTATUS    "sensor/parkslotstatus/"

#define ACCESS_POINT_NAME "ParkSense-ESP-AP"
#define ACCESS_POINT_PWD  "ParkSensePass" //Password for access point

#define OTA_USERNAME     "ps_admin" //OTA username
#define OTA_PASSWORD     "ps_ota_pass" //OTA password

#define LED          2
#define trigP        5
#define echoP        4
#define parkFree     12
#define parkOcc      13