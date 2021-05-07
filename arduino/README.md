
## Requirments
This requires a file "secrets.h" with the contents:

    #define SECRET_SSID ""
    #define SECRET_PASS ""

That contains a wifi network's SSID and password. This is to get the time from an NTP
server.

It also requires that the firmware on the ESP32 is upgraded as described here: https://learn.adafruit.com/upgrading-esp32-firmware/upgrade-an-airlift-all-in-one-board

## Configuration
If the NTP time isn't desired then just comment out the line:

    //#define USE_NTP

This will cause all of the NTP stuff to be left out when compiled.
