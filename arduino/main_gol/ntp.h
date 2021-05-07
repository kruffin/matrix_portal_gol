#pragma once

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

class Ntp {
public:  
  Ntp(char *wifiSsid, char *wifiPass, IPAddress *timeServer);
  ~Ntp();

  bool init();
  void disconnect();
  void requestNtpPacket();
  String getNtpResponse();
  String getTime(unsigned long epoch);
  bool isNight();
  
  unsigned long runningEpoch;

  static int OFFSET_HOURS;
  char *wifiSsid;
  char *wifiPass;

  const int maxTries = 1;
private:
  static const int NTP_PACKET_SIZE = 48; // 48 bytes

  IPAddress *timeServer;
  unsigned int udpRecievePort = 2390;
  byte packetBuffer[NTP_PACKET_SIZE];
  WiFiUDP udpProto;
};
