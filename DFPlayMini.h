#ifndef _DFPLAYMINI_H
#define _DFPLAYMINI_H

//Uncomment the define if you don't intend to use soft-serial
#define DF_USE_SOFTSERIAL

#include <Arduino.h>
#ifdef DF_USE_SOFTSERIAL
#include <SoftwareSerial.h>
#endif

//Equalizer settings
#define DF_EQU_NORMAL 0
#define DF_EQU_POP 1
#define DF_EQU_ROCK 2
#define DF_EQU_JAZZ 3
#define DF_EQU_CLASSIC 4
#define DF_EQU_BASS 5

//Playback source
#define DF_SRC_USB 0
#define DF_SRC_SD 1
#define DF_SRC_AUX 2
#define DF_SRC_FLASH 3
#define DF_SRC_SLEEP 4 //PC or SLEEP

class dfplayClass
{
  public:
    dfplayClass(HardwareSerial *serialBus, int16_t busyPin=-1);
    #ifdef DF_USE_SOFTSERIAL
    dfplayClass(SoftwareSerial *serialBus, int16_t busyPin=-1);
    #endif

    //A return-value of -1 indicates an error for functions with
    //return-type of int8_t
    int8_t isBusy();
    void setVolume(uint16_t volume);
    void next();
    void previous();
    //Track 1 - 2999
    int8_t setTrack(uint16_t trackNum);
    void volumeUp();
    void volumeDown();
    int8_t getVolume();
    void setEqualizer(uint8_t equ);
    void loopTrack(uint16_t track);
    void setSource(uint8_t src);
    //Don't know how to end standby, except by re-inserting SD-card
    void enterStandby();
    //Doesn't seem to do anything
    void enterNormal();
    //Reset all settings (vol 30, src SD, track 01 etc.)
    void resetModule();
    int8_t play();
    void pause();
    //Specify folder and file to playback
    //Important: Folders must be named 01~99, files must be named 001~255
    void setFolderAndFile(uint8_t folder, uint8_t file);
    //No idea what this one does
    void adjustVolume(uint16_t value);
    //Loop all tracks, starts from track 1
    //Set playPause to false to pause playback
    void loopPlayAll(bool playPause);
    void stop();
    int8_t getFirmwareVersion();
    int16_t getFlashFiles();
    int16_t getSDFiles();
    int16_t getFolders();
    void printReply();
    int16_t getInitParams();

  private:
    void dfUpdateChecksum();
    int8_t sendCmd(bool checkReturn);

    uint8_t dfSendBuffer[10] = { 0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF };
    uint8_t dfReceiveBuffer[10];
    int16_t dfBusyPin;
    bool dfUsingHSerial;
    HardwareSerial *dfHSerial;
    #ifdef DF_USE_SOFTSERIAL
    SoftwareSerial *dfSSerial;
    #endif
};

#endif