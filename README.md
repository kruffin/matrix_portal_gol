# Game of Life (GOL) and Other Simulations in a Clock

![Game of Life Clock](/images/gol_web.jpg)
![Game of Life Clock Back](/images/back_web.jpg)
![Game of Life Clock Side](/images/side_web.jpg)

## What is it?
This is a cool clock that performs some simple simulations to keep you in a trance while you watch time fly.

It also changes the colors to more muted ones for night time, since it's hard to sleep in the same room with it otherwise.

## How is it organized?
There are four main sections in the repo:

1. Arduino code (all the logic for the clock and simulations).
2. CircuitPython code (just a Game of Life simulation with colors based on how long a cell has been alive).
3. Configuration program (allows you to set, in persistent flash, configuration of various parameters that allow the arduino code to work).
4. 3D models for a case that can be printed to house the panel and microcontroller.

## How did it come about?
This repo has grown over time. It started as a test to see what Conway's Game of Life would look like after some coloring tweaks on Adafruit's RGB panel, kit here ( https://www.adafruit.com/product/4812 ). 

It was slow, so I decided to rewrite it in C++ with Arduino. I was blown away with the difference (try setting the fps to something like 60; I find 15 is a nice speed that allows you to see the changes and isn't too fast or slow).

As I had that running and was mesmerized by the cells living and dying, I thought it would be neat to have it tell me the time. The Matrix Portal microcontroller doesn't have a real time clock so I went with the next "best" thing of using the Wifi to ask an NTP server the time and use a GMT offset from that. And then there was time.

After a while of that and nights where I had to turn it off to sleep, I decided to reduce the colors at "night time" (10pm and 7am) to muted ones.

Then came the boids. I had a really bad implementation based on a memory of boids from college, but they followed around aggressively and were boidish. Later the description from here (https://people.ece.cornell.edu/land/courses/ece4760/labs/s2021/Boids/Boids.html ) was used for a proper simulation.

![Game of Life Clock - Boids](/images/boids_web.jpg)

After that some questionable things were added :) What I call cars that just travel north/south/east/west starting at a random position and wrap. Rain came next, that comes from northeast or northwest. And finally, the abomination that is "sprite rain" with possibly the worst small images ever created. But was an interesting/easy attempt using palettes and in-code number arrays to represent images.

![Game of Life Clock - Cars](/images/cars_web.jpg)
![Game of Life Clock - Rain](/images/rain_web.jpg)
![Game of Life Clock - Sprite Rain](/images/sprite_rain_web.jpg)

## How do you use it?
If you just want the CircuitPython GOL then all you need to do is drag the code.py over to the drive and you are good to go.

For the Arduino code, you need to do a few things (assuming a Matrix Portal as the microcontroller):

1. Update the ESP32 firmware so Wifi will work.
2. Get the Arduino IDE.
3. Add some libraries to the IDE via the Library Manager.
  * Adafruit Protomatter 1.4.1 - install all dependencies.
  * WifiNINA - Add Zip from: https://github.com/adafruit/WiFiNINA/archive/master.zip
  * Adafruit SleepyDog 1.6.1
4. Add the Adafruit boards via Preferences -> Additional Boards Manager URLs : https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
  * Reference page for steps: https://learn.adafruit.com/adafruit-matrixportal-m4/arduino-ide-setup
5. Add the board via the Boards Manager.
  * Adafruit SAMD Boards - 1.7.10
  * Arduino SAMD Boards
6. Create a secrets.h file with a couple of entries (allows you to default some wifi credentials if you want).
7. Flash the code.

Once it's running. Compile the `create_config.cpp` file in the config folder. Then run it from the commandline and pass it the parameters you want different from defaults; at a minumum the wifi user and pass are needed. After you get those parameters, funnel it to the display through the USB connection.

On linux and mac it should be somewhere like `/dev/ttyACM0`.

Example:

    ./create_config --wifi-ssid FAKE --wifi-pass "WILL NEVER WORK" --gol-fps 15 --gmt-offset -4 --sim-time 30000 > /dev/ttyACM0

The settings should take effect almost immediately. You should now be viewing something cool :)

If you want a case for it then look no further than the `case` folder. It comes in 3 pieces; 2 slide over the display and can be screwed in place while the 3rd covers the USB plug and circuit board that sticks out. If I had a better printer with a larger print bed then it could be done without having the joint near the middle (the open scad file used is also included so feel free to work with that and slice it how you want).

    TODO: Put some disassembled case pictures here.
