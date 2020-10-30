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
  
  unsigned long runningEpoch;

private:
  static const int NTP_PACKET_SIZE = 48; // 48 bytes

  char *wifiSsid;
  char *wifiPass;
  IPAddress *timeServer;
  unsigned int udpRecievePort = 2390;
  byte packetBuffer[NTP_PACKET_SIZE];
  WiFiUDP udpProto;
};
