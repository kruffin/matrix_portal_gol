#include "utility/wl_definitions.h"
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
#include "Adafruit_SleepyDog.h"


int Ntp::OFFSET_HOURS = -4; // minus 5 for DST in EST
const char Ntp::ntpServerName[] = "pool.ntp.org";
Ntp *Ntp::instance = NULL;

Ntp::Ntp(char *wifiSsid, char *wifiPass, IPAddress *timeServer) {
  this->wifiSsid = wifiSsid;
  this->wifiPass = wifiPass;
  this->timeServer = timeServer;
  this->totalFailures = 0;
  //this->timeClient.setUpdateInterval(1000 * 60 * 30); // 30 minutes.
  Ntp::instance = this;
};

Ntp::~Ntp() {
  
};

bool Ntp::init() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("No Wifi Module found.");  
    return false;
  }

  WiFi.noLowPowerMode();
  //WiFi.lowPowerMode();
  if (WiFi.status() != WL_CONNECTED) {
    if (this->totalFailures >= 5) {
      Serial.println("Restarting ESP due to too many WiFi connection failures.");
      int countdownMS = Watchdog.enable(20);
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
      Serial.print("Wifi connection failed with error code: ");
      char* codes[10] = { (char*)"WL_IDLE_STATUS",
          (char*)"WL_NO_SSID_AVAIL",
          (char*)"WL_SCAN_COMPLETED",
          (char*)"WL_CONNECTED",
          (char*)"WL_CONNECT_FAILED",
          (char*)"WL_CONNECTION_LOST",
          (char*)"WL_DISCONNECTED",
          (char*)"WL_AP_LISTENING",
          (char*)"WL_AP_CONNECTED",
          (char*)"WL_AP_FAILED"
      };
      Serial.println(codes[s]);
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
      this->totalFailures++;    
      return false;
    }
  }

  Serial.println("Starting udp...");
  //timeClient.begin();
  this->udpProto.begin(this->udpRecievePort);
  Serial.println("Udp started.");
  setSyncProvider(Ntp::ntpTimeSync);
  setSyncInterval(60 * 30); // 30 minutes



  return true;
};

void Ntp::disconnect() {
  //Serial.println("Disconnectingj from Wifi.");
  //this->udpProto.stop();
    ////WiFi.disconnect();
    ////WiFi.end();
  //WiFi.lowPowerMode();
};

void Ntp::requestNtpPacket() {
  // if (WiFi.status() != WL_CONNECTED) {
  //   return;
  // }

  // this->udpProto.flush(); // Get rid of any UDP packets that may already be there

  // memset(this->packetBuffer, 0, Ntp::NTP_PACKET_SIZE);

  // this->packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  // this->packetBuffer[1] = 0;     // Stratum, or type of clock
  // this->packetBuffer[2] = 6;     // Polling Interval
  // this->packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // // 8 bytes of zero for Root Delay & Root Dispersion
  // this->packetBuffer[12]  = 49;
  // this->packetBuffer[13]  = 0x4E;
  // this->packetBuffer[14]  = 49;
  // this->packetBuffer[15]  = 52;

  // Serial.print("Sending NTP request... [");
  // Serial.print((*this->timeServer)[0]);
  // Serial.print(".");
  // Serial.print((*this->timeServer)[1]);
  // Serial.print(".");
  // Serial.print((*this->timeServer)[2]);
  // Serial.print(".");
  // Serial.print((*this->timeServer)[3]);
  // Serial.println("]");
  // this->udpProto.beginPacket(*this->timeServer, 123); //NTP requests are to port 123
  // this->udpProto.write(this->packetBuffer, NTP_PACKET_SIZE);
  // this->udpProto.endPacket();
  // Serial.println("NTP request sent.");
};

String Ntp::getNtpResponse() {
  String toRet = "";
  // while (this->udpProto.parsePacket()) {
  //   Serial.println("Recieved NTP packet.");
  //   this->udpProto.read(this->packetBuffer, Ntp::NTP_PACKET_SIZE);

  //   unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  //   unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  //   // combine the four bytes (two words) into a long integer
  //   // this is NTP time (seconds since Jan 1 1900):
  //   unsigned long secsSince1900 = highWord << 16 | lowWord;

  //   // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  //   const unsigned long seventyYears = 2208988800UL;
  //   // subtract seventy years:
  //   unsigned long epoch = secsSince1900 - seventyYears;

  //   if (epoch > this->runningEpoch) {
  //     this->runningEpoch = epoch;
  //     toRet = this->getTime(epoch);
  //   }
  // }
  return toRet;
};

void Ntp::update() {
  //timeClient.update();
  //this->runningEpoch = timeClient.getEpochTime();
  this->runningEpoch = now();
}

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
  // Serial.print("hour: ");
  // Serial.println(hour);
  return (hour >= 22 || hour <= 7); // between 10pm and 7am
};

time_t Ntp::ntpTimeSync() {
  if (Ntp::instance) {
    return Ntp::instance->getNtpTime();
  }
  Serial.print(millis());
  Serial.println(": No NTP instance set.");
  return 0;
}

time_t Ntp::getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address
  IPAddress zeroIp = IPAddress(0, 0, 0, 0);
  while (udpProto.parsePacket() > 0) ; // discard any previously received packets
  Serial.print(millis());
  Serial.println(": Transmit NTP Request");
  // get a random server from the pool
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1000 * 20) { // 20 seconds
    int s = WiFi.status();
    if (s != WL_CONNECTED) {
      char* codes[10] = { 
        (char*)"WL_IDLE_STATUS",
        (char*)"WL_NO_SSID_AVAIL",
        (char*)"WL_SCAN_COMPLETED",
        (char*)"WL_CONNECTED",
        (char*)"WL_CONNECT_FAILED",
        (char*)"WL_CONNECTION_LOST",
        (char*)"WL_DISCONNECTED",
        (char*)"WL_AP_LISTENING",
        (char*)"WL_AP_CONNECTED",
        (char*)"WL_AP_FAILED"
      };
      Serial.print("Wifi status: ");
      Serial.println(codes[s]);

      this->udpProto.stop();
      this->init();
    }
    
    Serial.print(millis());
    Serial.println(": Looking up NTP server IP.");
    WiFi.hostByName(ntpServerName, ntpServerIP);
    if (ntpServerIP != zeroIp) { 
      break; 
    }
    
    
  }
  Serial.print(millis());
  Serial.print(": ");
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  this->sendNTPpacket(ntpServerIP);
  beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = udpProto.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.print(millis());
      Serial.println(": Receive NTP Response");
      udpProto.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL;// + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.print(millis());
  Serial.println(": No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void Ntp::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udpProto.beginPacket(address, 123); //NTP requests are to port 123
  udpProto.write(packetBuffer, NTP_PACKET_SIZE);
  udpProto.endPacket();
}
