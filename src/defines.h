
/*-------- Important IOT Device Settings ---------*/
// #define deviceName        "Test Device" //Specify the name to be seen on network
#define firmwareVersion   "1.0.0" //Specify the firmware version
// #define MQTT_USERNAME     "test_mqtt"
// #define MQTT_KEY          "test_pass"
// #define SERVER         "192.168.0.111" //"192.168.1.105"
#define SERVERPORT        1883
#define RPI_HTTP_PORT     5000 //Port for RPI HTTP server
#define ADA_SERVER       "io.adafruit.com"
#define ADA_USER_NAME    "patilect"
#define ADA_ACC_KEY      "d288fe61159d4fb084882b7de8a13ca6"

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
#define ONN_TIME_ADDR     167 //Starting address for vehicle distance of size 5 bytes
#define OFF_TIME_ADDR     172 //Starting address for vehicle distance of size 5 bytes
#define ON_TIME_ADDR      177 //Starting address for vehicle distance of size 5 bytes
#define TEST_TIME_ADDR    182 //Starting address for vehicle distance of size 5 bytes
#define TIME_SIZE         5 //Size of time in bytes

#define PREAMBLE          "patilect/"
#define MESSAGES          "patilect/feeds/messages" //for specific messages related to batches
// #define PARKINDCTR        "sensor/parkindicator"
// #define DISTANCE          "sensor/distance/"
// #define PARKSLOTSTATUS    "sensor/parkslotstatus/"

#define ON_TIME           "patilect/feeds/ontime"
#define OFF_TIME          "patilect/feeds/offtime"

#define ACCESS_POINT_NAME "ParkSense-ESP-AP"
#define ACCESS_POINT_PWD  "ParkSensePass" //Password for access point

#define OTA_USERNAME     "ps_admin" //OTA username
#define OTA_PASSWORD     "ps_ota_pass" //OTA password

#define LED          2
#define trigP        5
#define echoP        4
#define parkFree     12
#define parkOcc      13
#define waterPumpPin 14
#define errorLED     15 // need to test thoroughly