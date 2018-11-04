// Stripe settings
#define LED_PIN D1
#define NUM_PIXELS_ALL 12
#define BRIGHTNESS 150

int LED_GREEN_COOLDOWN_SECS = 120;
int LED_YELLOW_COOLDOWN_SECS = 180;

// Door settings
#define CHANGE_THRESHOLD_MS 100
#define DOOR_PIN D2

// Sound settings
int playerVolume = 30;

// Network settings
const char* ssid = "*";
const char* password = "*";
const char* espName = "esp8266-fartlight";

//Name of device
String deviceName = "bathroom-door";

// Endpoint settings
String slaveIp = "192.168.100.2";
const char* contenttype = "application/x-www-form-urlencoded";

String userAgent = "Toilet Traffic Light - HTTP-Client";
String clientVer = "0.2";
