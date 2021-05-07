The file `create_config.cpp` allows you to configure the GOL Clock with a Wifi SSID and password as well as the GMT offset to use.

    ./create_config > /dev/ttyACM0

## Build
To compile you just need g++.

    g++ create_config.cpp -o create_config