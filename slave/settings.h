// Stripe settings
#define LED_PIN D1
#define NUM_PIXELS_ALL 12
#define BRIGHTNESS 150

int LED_GREEN_COOLDOWN_SECS = 120;
int LED_YELLOW_COOLDOWN_SECS = 180;

// Sound settings
int playerVolume = 30;

// Network settings
const char* ssid = "*";
const char* password = "*";
const char* espName = "esp8266-fartlight-slave";

//Name of device
String deviceName = "bathroom-door-slave";

String userAgent = "Toilet Traffic Light - HTTP-Client (Slave)";
String clientVer = "0.1";
