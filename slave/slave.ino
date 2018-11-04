#include <TM1637.h>
#include <Ticker.h>
#include "settings.h"
#include "mp3player.h"
#include "webclient.h"
#include "ledstripe.h"
#include "display.h"

Ticker ticker;

int doorState = 1; //start with open door

bool cooldownIsActive = false;
int cooldownMode = 0; // 0 = off, 1 = yellow, 2 = green
int cooldownMs = 0;

int lockCountSecs = 0;
int lockCountMins = 0;
bool lockCountDivider = false;
unsigned long lockCountMillis = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial) {} // Wait
  delay(300);
  splashScreen();
  Serial.println();
  Serial.println("Booting....");
  Serial.println();
  
  // Init strip
  Serial.println("Init strip...");
  strip.begin();
  clearStrip();

  Serial.println("Setting randomizer...");
  randomSeed(analogRead(0));

  cooldownMode = 2;

  //Init player 
  Serial.println("Setting up player...");
  setupPlayer();
  Serial.println("DONE.");
  delay(200);

  //Init ticker
  Serial.println();
  Serial.println("Attaching ticker...");
  Serial.println();
  ticker.attach(1, loopCount);

  // Init wifi
  startWIFI();
  startWebserver();

  // Init display
  Serial.println("Init display...");
  tm1637.set(6);
  tm1637.init();

  Serial.println("");
  Serial.println("Setup complete.");
  Serial.println("");
}


void loop() {
  //(re)connect wifi if not connected
  if (WiFiMulti.run() != WL_CONNECTED) {
    delay(1);
    startWIFI();
    return;
  }
  
  server.handleClient();

  if (loopCount > 0) {
    handleTick();
    handleCooldown();
    paint();
    loopCnt = 0;
  }
}


bool handleRequestSetLock() {
  String result = "ignored.";
  if (doorState > 0) {
    doorState = 0;
    resetCooldown();
    setRed();
    playRandomSound();
    result = "okay.";
  }

  server.send(200, "text/plain", result);
}


bool handleRequestSetUnlock() {
  String result = "ignored.";
  if (doorState < 1) {
    doorState = 1;
    initCooldown();
    resetLockedTime();
    result = "okay.";
  }
  server.send(200, "text/plain", result);
}


bool doorLocked() {
  if (doorState == 0) {
    return true;
  }

  return false;
}


void handleTick() {
  if (doorLocked()) {
    tickLockedTime();
  }
}


void initCooldown() {
  cooldownIsActive = true;
}


void handleCooldown() {
  if(cooldownIsActive) {
    if (cooldownMode < 1) {
      cooldownMode = 1;
    }

    if (cooldownMode == 1) {
      if(cooldownMs == 0) {
        setYellow();
      }
      cooldownMs++;
  
      if (cooldownMs >= LED_YELLOW_COOLDOWN_SECS * 1000) {
        cooldownMode = 2;
        cooldownMs = 0;
      }
    }

    if (cooldownMode == 2) {
      if(cooldownMs == 0) {
        setGreen();
      }
      cooldownMs++;
  
      if (cooldownMs >= LED_GREEN_COOLDOWN_SECS * 1000) {
        resetCooldown();
        ledFadeOut = true;
      }
    }
  }
}


void resetCooldown() {
  cooldownIsActive = false;
  cooldownMode = 0;
  cooldownMs = 0;
}


/*
 * Hacky Hacky, time is running.
 * No time for cool animation :(
 * 
 */
void paint() {
  for (int count = 0; count < NUM_PIXELS_ALL; count++) {
    strip.setPixelColor(count, LED_COLOR);

    if (ledAnimActive) {
      if (count == LED_BLACK_PIXEL) {
        strip.setPixelColor(count, 0x000000);
      }
    }
  }
  strip.show();

  if (ledAnimActive) {
    if (ledDelayCurrent >= ledAnimDelayMs) {
      LED_BLACK_PIXEL++;
      ledDelayCurrent = 0;
  
      if (LED_BLACK_PIXEL == NUM_PIXELS_ALL) {
        LED_BLACK_PIXEL = 0;
      }
    }
    ledDelayCurrent++;
  }

  if (ledFadeOut) {
    if (millis() % 300) {
      ledBrightness--;
      strip.setBrightness(ledBrightness);
  
      if(ledBrightness <= 0) {
        ledFadeOut = false;
        ledBrightness = BRIGHTNESS;
        clearStrip();
      }
    }
  }
}


void clearStrip() {
  ledAnimActive = false;
  LED_COLOR = 0x000000;
  strip.setBrightness(BRIGHTNESS);
}


void setRed() {
  ledAnimActive = true;
  LED_COLOR = 0xFF0000;
  strip.setBrightness(BRIGHTNESS);
}


void setGreen() {
  ledAnimActive = false;
  LED_COLOR = 0x00FF00;
  strip.setBrightness(BRIGHTNESS);
}


void setYellow() {
  ledAnimActive = false;
  LED_COLOR = 0xFFFF00;
  strip.setBrightness(BRIGHTNESS);
}


void playRandomSound() {
  uint16_t filecount = mp3.getTotalTrackCount();
  int rand = random(1, filecount + 1);
  Serial.print("Track ");
  Serial.println(rand);
  mp3.playMp3FolderTrack(rand); 
}


void tickLockedTime() {
  unsigned long currentMillis = millis();
  if (lockCountMillis + 1000 < currentMillis) {
    lockCountMillis = currentMillis;
    lockCountSecs++;

    if (lockCountSecs > 59) {
      lockCountSecs = 0;
      lockCountMins++;
    }

    if (lockCountMins > 59) {
      resetLockedTime();
    }

    displayTime(lockCountMins, lockCountSecs);
  }
}


void resetLockedTime() {
  lockCountSecs = 0;
  lockCountMins = 0;
  lockCountMillis = 0;

  tm1637.point(false);
  tm1637.clearDisplay();
}


void displayTime(int minutes, int seconds) {
  TimeDisp[0] = minutes / 10;
  TimeDisp[1] = minutes % 10;
  TimeDisp[2] = seconds / 10;
  TimeDisp[3] = seconds % 10;

  lockCountDivider = !lockCountDivider;
  tm1637.point(lockCountDivider);

  tm1637.display(TimeDisp);
}


void dumpStateChange(String status) {
  Serial.println("-------------");
  Serial.print("NEW Status: ");
  Serial.println(status);
  Serial.println("-------------");
}


/**
 * Dump some information on startup.
 */
void splashScreen() {
  for (int i=0; i<=5; i++) Serial.println();
  Serial.println("#######################################");
  Serial.print("# Furzampel - Client - v. ");
  Serial.println(clientVer);
  Serial.println("# -----------");
  Serial.println("# Chris Figge (flazer)");
  Serial.println("# Mail: info@flazer.net");
  Serial.println("# -----------");
  Serial.print("# DeviceName: ");
  Serial.println(deviceName);
  Serial.println("#######################################");
}


/**
 * Start webserver for handling
 * incoming requests
 */
void startWebserver() {
  Serial.println("Starting HTTP-Server...");
  Serial.println("-- Registering routes.");
  server.on("/lock/", handleRequestSetLock);
  server.on("/unlock/", handleRequestSetUnlock);
  server.onNotFound(handleRequestNotFound);
  Serial.println("-- Launching server ...");
  server.begin();
  Serial.println("-- DONE.");
}


/**
 * Tick, tick, tick
 */
void loopCount() {
  loopCnt++;
}

