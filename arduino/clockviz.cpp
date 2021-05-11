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

#include "clockviz.h"
#include "secrets.h"
#include "version.h"

ClockViz::ClockViz(int width, int height, char *ntpIp) {
  this->width = width;
  this->height = height;
  this->timeText = Text();
  this->ntpIP = new IPAddress();
  this->set_ntp_ip(ntpIp);
  
  this->ssid = (char *)malloc((strlen(SECRET_SSID)+1) * sizeof(char));
  strcpy(this->ssid, SECRET_SSID);
  this->pass = (char *)malloc((strlen(SECRET_PASS)+1) * sizeof(char));
  strcpy(this->pass, SECRET_PASS);
  this->timeGrabber = new Ntp(this->ssid, this->pass, this->ntpIP);

  this->showTime = true;
  this->hasTime = false;
  this->textY = 2;
  this->lastTime = 0;
  this->dt = 0;
  this->lastTimePulled = millis();
  this->backOffTime = 0;
};

ClockViz::~ClockViz() {
  delete this->timeGrabber;
  delete this->ntpIP;
  delete this->ssid;
  delete this->pass;
};

void ClockViz::set_credentials(char *ssid, char *password) {
  delete this->ssid;
  delete this->pass;

  if (ssid == NULL) {
    ssid = SECRET_SSID;
  }

  if (password == NULL) {
    password = SECRET_PASS;
  }

  this->ssid = (char *)malloc((strlen(ssid)+1) * sizeof(char));
  strcpy(this->ssid, ssid);

  this->pass = (char *)malloc((strlen(password)+1) * sizeof(char));
  strcpy(this->pass, password);

  this->timeGrabber->wifiSsid = this->ssid;
  this->timeGrabber->wifiPass = this->pass;
}

void ClockViz::set_ntp_ip(char *ntpIp) {
   if (ntpIp != NULL && !this->ntpIP->fromString(ntpIp)) {
    Serial.print("Failed to parse NTP IP address [");
    Serial.print(ntpIp);
    Serial.println("]. Defaulting to 129.6.15.28");
    (*this->ntpIP)[0] = 129;
    (*this->ntpIP)[1] = 6;
    (*this->ntpIP)[2] = 15;
    (*this->ntpIP)[3] = 28;
  }
};

bool ClockViz::init(Adafruit_Protomatter *matrix) {
  this->timeText.value = "GETTING TIME";
  this->timeText.draw(2,2, matrix, matrix->color565(255, 0, 0));
  this->timeText.value = VERSION;
  this->timeText.draw(2,this->height - Text::textHeight - 2, matrix, matrix->color565(0, 255, 0));
  matrix->show();
  if (!this->timeGrabber->init()) {
    Serial.println("Failed to start time grabber.");
    return false;
  }
  this->timeGrabber->requestNtpPacket();
  this->timeColor = Colors::get_colors(this->isNight())[1 + random(MAX_COLORS-1)];
  return true;
};

void ClockViz::draw(Adafruit_Protomatter *matrix, cell_color *col) {
  if (this->showTime) {
    this->timeText.draw(2,this->textY, matrix, matrix->color565(this->timeColor.red, this->timeColor.green, this->timeColor.blue));
  }
};

void ClockViz::randomize() {
  this->textY += 2;
  if (this->textY > this->height - Text::textHeight - 1) {
    this->textY = 2;
  }
  
  this->timeColor = Colors::get_colors(this->isNight())[1 + random(MAX_COLORS-1)];
};

void ClockViz::update(unsigned long not_used) {
  String s;
  if (!this->hasTime) {
    s = this->timeGrabber->getNtpResponse();
    if (s.length() != 0) {
      this->hasTime = true;
      this->lastTime = millis();
      this->lastTimePulled = lastTime;
      this->backOffTime = 0;
      this->timeGrabber->disconnect();
    }
  }

  unsigned long now = millis();
  if (!this->hasTime && (now - this->lastTimePulled - this->backOffTime > 30 * 1000)) {
    // 30 seconds has passed and no ntp response, request it again.
    this->timeGrabber->requestNtpPacket();
    this->backOffTime += 30 * 1000; // Wait another 30 seconds before requesting again.
  }

  if (this->hasTime && (now - this->lastTimePulled > 4 * 60 * 60 * 1000)) {
    // 4 hours have passed, attempt to resync with ntp
    while (!this->timeGrabber->init()) {
      Serial.println("Failed to start time grabber.");
      delay(1000);
    }
    this->timeGrabber->requestNtpPacket();
    this->hasTime = false;
  }
  
  if (0 != this->timeGrabber->runningEpoch) {
    // DEBT: This won't work forever since the return from millis() will eventually wrap back to 0. (50 days according
    // to the arduino.cc millis() documentation).
    this->dt += now - this->lastTime; // Keep a count of the milliseconds that are building up.
    unsigned long seconds = this->dt / 1000;
    this->dt -= seconds * 1000; // subtract off any seconds we took
    this->lastTime = now;

    this->timeGrabber->runningEpoch += seconds;
    if (this->showTime) {
      s = this->timeGrabber->getTime(this->timeGrabber->runningEpoch);
    }
  }

  if (s.length() != 0) {
    this->timeText.value = s;
  }
};
