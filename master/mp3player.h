#include <SoftwareSerial.h>
#include <DFMiniMp3.h>

class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack) {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);
  }

  static void OnCardOnline(uint16_t code) {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);
  }

  static void OnCardInserted(uint16_t code) {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code) {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }
};

SoftwareSerial secondarySerial(D3, D4); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

void setupPlayer() {
  mp3.begin();
  uint16_t volume = mp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  mp3.setVolume(playerVolume);

  uint16_t filecount = mp3.getTotalTrackCount();
  Serial.print("Total tracks: ");
  Serial.println(filecount);
}

