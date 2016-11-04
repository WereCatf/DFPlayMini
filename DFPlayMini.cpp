#include <DFPlayMini.h>

#define DF_QUERY_STATUS 0x42
#define DF_SET_SOURCE 0x09
#define DF_SOURCE_FLASH 0x04

int i;

dfplayClass::dfplayClass(HardwareSerial *serialBus, int16_t busyPin)
{
  dfBusyPin=busyPin;
  dfUsingHSerial=true;
  dfHSerial=serialBus;
}

#ifdef DF_USE_SOFTSERIAL
dfplayClass::dfplayClass(SoftwareSerial *serialBus, int16_t busyPin)
{
  dfBusyPin=busyPin;
  dfUsingHSerial=false;
  dfSSerial=serialBus;
}
#endif

__inline__ void dfplayClass::dfUpdateChecksum()
{
  int16_t checksum=0-(dfSendBuffer[1]+dfSendBuffer[2]+dfSendBuffer[3]
    +dfSendBuffer[4]+dfSendBuffer[5]+dfSendBuffer[6]);
  dfSendBuffer[7]=checksum >> 8;
  dfSendBuffer[8]=checksum & 0xff;
}

int8_t dfplayClass::sendCmd(bool checkReturn)
{
  #ifdef DF_USE_SOFTSERIAL
  if(!dfUsingHSerial)
  {
    while(dfSSerial->available()) dfSSerial->read();
    dfSSerial->write(dfSendBuffer, 10);
    dfSSerial->flush();
  }
  else
  #endif
  {
    while(dfHSerial->available()) dfHSerial->read();
    dfHSerial->write(dfSendBuffer, 10);
    dfHSerial->flush();
  }
  delay(30);
  if(!checkReturn) return 0;
  for(i=0; i<10; i++) dfReceiveBuffer[i]=0;

  #ifdef DF_USE_SOFTSERIAL
  if(!dfUsingHSerial){
    i=0;
    while(dfSSerial->available()){
       dfReceiveBuffer[i]=dfSSerial->read();
       i++;
       if(i>=10) break;
    }
  }
  else
  #endif
  {
    i=0;
    while(dfHSerial->available()){
       dfReceiveBuffer[i]=dfHSerial->read();
       i++;
       if(i>=10) break;
    }
  }

  if(dfReceiveBuffer[0]==0x7e && dfReceiveBuffer[1]==0xff){
    if(dfReceiveBuffer[3]==dfSendBuffer[3]) return 0;
    //if(dfReceiveBuffer[3]==0x40) //Do something
  }

  if(dfReceiveBuffer[0]==0 && dfReceiveBuffer[1]==0) return 0;
  return -1;
}

int8_t dfplayClass::isBusy()
{
  if(dfBusyPin!=-1) return digitalRead(dfBusyPin) ? 0 : 1;
  dfSendBuffer[3]=0x42;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return dfReceiveBuffer[6];
  else return -1;
}

void dfplayClass::setVolume(uint16_t volume)
{
  if(volume > 30) volume = 30;
  dfSendBuffer[3]=0x06;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=volume & 0xff;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::next()
{
  dfSendBuffer[3]=0x01;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::previous()
{
  dfSendBuffer[3]=0x02;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

int8_t dfplayClass::setTrack(uint16_t trackNum)
{
  dfSendBuffer[3]=0x03;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=trackNum >> 8;
  dfSendBuffer[6]=trackNum;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return 0;
  else return -1;
}

void dfplayClass::volumeUp()
{
  dfSendBuffer[3]=0x04;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::volumeDown()
{
  dfSendBuffer[3]=0x05;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

int8_t dfplayClass::getVolume()
{
  dfSendBuffer[3]=0x43;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return dfReceiveBuffer[6];
  else return -1;
}

void dfplayClass::setEqualizer(uint8_t equ)
{
  if(equ > 5) return;
  dfSendBuffer[3]=0x07;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=equ;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::loopTrack(uint16_t track)
{
  dfSendBuffer[3]=0x08;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=track >> 8;
  dfSendBuffer[6]=track;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::setSource(uint8_t src)
{
  if(src > 5) return;
  dfSendBuffer[3]=0x09;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=src;
  dfUpdateChecksum();
  sendCmd(true);
}

void dfplayClass::enterStandby()
{
  dfSendBuffer[3]=0x0a;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::enterNormal()
{
  dfSendBuffer[3]=0x0b;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::resetModule()
{
  dfSendBuffer[3]=0x0c;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

int8_t dfplayClass::play()
{
  dfSendBuffer[3]=0x0d;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return 0;
  return -1;
}

void dfplayClass::pause()
{
  dfSendBuffer[3]=0x0e;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

void dfplayClass::setFolderAndFile(uint8_t folder, uint8_t file)
{
  if(folder > 99) return;
  dfSendBuffer[3]=0x0f;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=folder;
  dfSendBuffer[6]=file;
  dfUpdateChecksum();
  sendCmd(true);
  Serial.print("\nRecbuf:");
  for(i=0; i<10; i++) Serial.printf(" 0x", dfReceiveBuffer[i]);
  Serial.println("");
}

void dfplayClass::adjustVolume(uint16_t value)
{
  dfSendBuffer[3]=0x10;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=value >> 8;
  dfSendBuffer[6]=value;
  dfUpdateChecksum();
  sendCmd(true);
  Serial.print("\nRecbuf:");
  for(i=0; i<10; i++) Serial.printf(" 0x", dfReceiveBuffer[i]);
  Serial.println("");
}

void dfplayClass::loopPlayAll(bool playPause)
{
  dfSendBuffer[3]=0x11;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=playPause;
  dfUpdateChecksum();
  sendCmd(true);
  Serial.print("\nRecbuf:");
  for(i=0; i<10; i++) Serial.printf(" 0x", dfReceiveBuffer[i]);
  Serial.println("");
}

void dfplayClass::stop()
{
  dfSendBuffer[3]=0x16;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  sendCmd(false);
}

int8_t dfplayClass::getFirmwareVersion()
{
  dfSendBuffer[3]=0x46;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return dfReceiveBuffer[6];
  else return -1;
}

int16_t dfplayClass::getFlashFiles()
{
  dfSendBuffer[3]=0x49;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return (dfReceiveBuffer[5] << 8) | dfReceiveBuffer[6];
  else return -1;
}

int16_t dfplayClass::getSDFiles()
{
  dfSendBuffer[3]=0x48;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return (dfReceiveBuffer[5] << 8) | dfReceiveBuffer[6];
  else return -1;
}

int16_t dfplayClass::getFolders()
{
  dfSendBuffer[3]=0x4f;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return (dfReceiveBuffer[5] << 8) | dfReceiveBuffer[6];
  else return -1;
}

void dfplayClass::printReply()
{
  Serial.printf("Reply:");
  for(int i=0; i<10; i++) Serial.printf(" %02x", dfReceiveBuffer[i]);
  Serial.printf("\n\n");
}

int16_t dfplayClass::getInitParams()
{
  dfSendBuffer[3]=0x3f;
  dfSendBuffer[4]=0;
  dfSendBuffer[5]=0;
  dfSendBuffer[6]=0x0a;
  dfUpdateChecksum();
  if(sendCmd(true)==0) return (dfReceiveBuffer[5] << 8) | dfReceiveBuffer[6];
  else return -1;
}