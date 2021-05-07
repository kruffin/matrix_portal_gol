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

#include "ntp.h"

int Ntp::OFFSET_HOURS = -4; // minus 5 for DST in EST

Ntp::Ntp(char *wifiSsid, char *wifiPass, IPAddress *timeServer) {
  this->wifiSsid = wifiSsid;
  this->wifiPass = wifiPass;
  this->timeServer = timeServer;
};

Ntp::~Ntp() {
  
};

bool Ntp::init() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("No Wifi Module found.");
    return false;
  }

  String firmware = WiFi.firmwareVersion();
  if (firmware < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.print("Incorrect firmware, expected: ");
    Serial.print(WIFI_FIRMWARE_LATEST_VERSION);
    Serial.print(" but found: ");
    Serial.println(firmware);
    return false;
  }

  unsigned char tries = 0;
  Serial.print("Attempting to connect to the wifi network...(");
  Serial.print(this->wifiSsid);
  Serial.print(", ");
  Serial.print(this->wifiPass);
  Serial.println(")");
  int s = WiFi.begin(this->wifiSsid, this->wifiPass);
  while (s != WL_CONNECTED && tries < Ntp::maxTries) {
    delay(5000);
    s = WiFi.begin(this->wifiSsid, this->wifiPass);
    tries++;
  }

  if (tries >= Ntp::maxTries) {
    Serial.print("Failed to connect to the wifi network [");
    Serial.print(this->wifiSsid);
    Serial.print("] after ");
    Serial.print(Ntp::maxTries);
    Serial.println(" tries.");
    return false;
  }

  Serial.println("Starting udp...");

  this->udpProto.begin(this->udpRecievePort);
  Serial.println("Udp started.");

  return true;
};

void Ntp::disconnect() {
  Serial.println("Disconnecting from Wifi.");
  this->udpProto.stop();
  WiFi.end();
};

void Ntp::requestNtpPacket() {
  memset(this->packetBuffer, 0, Ntp::NTP_PACKET_SIZE);

  this->packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  this->packetBuffer[1] = 0;     // Stratum, or type of clock
  this->packetBuffer[2] = 6;     // Polling Interval
  this->packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  this->packetBuffer[12]  = 49;
  this->packetBuffer[13]  = 0x4E;
  this->packetBuffer[14]  = 49;
  this->packetBuffer[15]  = 52;

  Serial.println("Sending NTP request...");
  this->udpProto.beginPacket(*this->timeServer, 123); //NTP requests are to port 123
  this->udpProto.write(this->packetBuffer, NTP_PACKET_SIZE);
  this->udpProto.endPacket();
  Serial.println("NTP request sent.");
};

String Ntp::getNtpResponse() {
  if (this->udpProto.parsePacket()) {
    Serial.println("Recieved NTP packet.");
    this->udpProto.read(this->packetBuffer, Ntp::NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;

    this->runningEpoch = epoch;
    return this->getTime(epoch);
  } else {
    return "";
  }
};

String Ntp::getTime(unsigned long epoch) {
  long hour = (epoch  % 86400L) / 3600 + Ntp::OFFSET_HOURS;
  while (hour <= 0) {
    hour += 12;
  } 
  while (hour > 12) {
    hour -= 12;
  }
  unsigned long minute = (epoch  % 3600) / 60;
  char timeStr[10];
  sprintf(timeStr, "%d:%0.2d", hour, minute);
  return String(timeStr);
};

bool Ntp::isNight() {
  long hour = (this->runningEpoch  % 86400L) / 3600 + Ntp::OFFSET_HOURS;
  if (hour < 0) {
    hour += 24;
  } else if (hour >= 24) {
    hour -= 24;
  }
  Serial.print("hour: ");
  Serial.println(hour);
  return (hour >= 22 || hour <= 7); // between 10pm and 7am
};
