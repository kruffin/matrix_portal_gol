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

#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

struct configuration {
  char version[8];
  int gmtOffset;
  char wifiName[32];
  char wifiPass[32];
  int golFps;
  int simTime;
  char ntpIp[16];
};

void print_error(std::string prog_name) {
	std::cerr << "Usage: " << prog_name << " <options>" << std::endl
			  << "Options: " << std::endl
			  << "If options are not passed, then defaults () will be used." << std::endl
			  << "--gmt-offset OFFSET\tThe GMT offset (-4)." << std::endl
			  << "--wifi-ssid SSID\tThe SSID of the wifi network (FAKE)." << std::endl
			  << "--wifi-pass PASS\tThe password of the wifi network (WILL NEVER WORK)." << std::endl
			  << "--gol-fps FPS\t\tThe frames per second to run the GOL simulation; must be greater than zero (10)." << std::endl 
			  << "--sim-time TIME\tThe time each simulation should run for in milliseconds (30 seconds)." << std::endl
			  << "--ntp-ip IP\tThe NTP IP to use (129.6.15.28)." << std::endl;
};

int main(int argc, char * argv[]) {

	configuration conf = {
		"GOL001", // Magic version expected by device.
		-4,
		"FAKE",
		"WILL NEVER WORK",
		10,
		30*1000,
		"129.6.15.28"
	};

	/* Expecting 5 parameters:
		1. --gmt-offset : the GMT offset.
		2. --wifi-ssid 	: the SSID of the wifi network.
		3. --wifi-pass	: the password of the wifi network.
		4. --gol-fps	: the frames per second to run the GOL simulation. 
		5. --sim-time	: the time each simulation should run for in milliseconds.
	*/

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--gmt-offset") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			conf.gmtOffset = atoi(argv[i]);
		} else if (arg == "--wifi-ssid") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			strncpy(conf.wifiName, argv[i], std::min((unsigned int)strlen(argv[i]) + 1, (unsigned int)31));
			conf.wifiName[31] = '\0';
		} else if (arg == "--wifi-pass") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			strncpy(conf.wifiPass, argv[i], std::min((unsigned int)strlen(argv[i]) + 1, (unsigned int)31));
			conf.wifiPass[31] = '\0';
		} else if (arg == "--gol-fps") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			conf.golFps = atoi(argv[i]);
			if (conf.golFps <= 0) {
				print_error(argv[0]);
				return -1;
			}
		} else if (arg == "--sim-time") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			conf.simTime = atoi(argv[i]);
			if (conf.simTime <= 0) {
				print_error(argv[0]);
				return -1;
			}
		} else if (arg == "--ntp-ip") {
			++i;
			if (i >= argc) {
				print_error(argv[0]);
				return -1;
			}
			strncpy(conf.ntpIp, argv[i], std::min((unsigned int)strlen(argv[i]) + 1, (unsigned int)15));
			conf.ntpIp[15] = '\0';
		}
	}

	fwrite(&conf, 1, sizeof(configuration), stdout);
	
};

