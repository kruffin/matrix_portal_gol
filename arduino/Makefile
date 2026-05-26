# ~/.arduino15/staging/libraries/
# ~/Arduino/libraries
ARDUINO_CONFIG_FILE=./arduino-cli.yaml
FQBN=adafruit:samd:adafruit_matrixportal_m4
PORT=/dev/ttyACM0

setup:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} core update-index
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} core install adafruit:samd
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} lib install "Adafruit Protomatter"@1.7.1
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} lib install "Adafruit SleepyDog Library"@1.8.4
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} lib install "Time"@1.6.1
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} lib install "WiFiNINA"@1.8.14

build:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} compile --fqbn ${FQBN} arduino.ino

upload:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} upload -p ${PORT} --fqbn ${FQBN} arduino.ino

monitor:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} monitor -p ${PORT} --fqbn ${FQBN}

print-config:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} config dump

check-board:
	arduino-cli --config-file ${ARDUINO_CONFIG_FILE} board list
