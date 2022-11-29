/**
GOL Clock - A clock that displays simulations.
Copyright (C) 2021 Kevin Ruffin

This file is part of GOL Clock.

GOL Clock is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GOL Clock is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
  unsigned int totalFailures = 0;
};
