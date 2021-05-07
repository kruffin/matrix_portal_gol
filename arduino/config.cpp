#include "config.h"

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>

// On-board external flash (QSPI or SPI) macros should already
// defined in your board variant if supported
// - EXTERNAL_FLASH_USE_QSPI
// - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
#if defined(EXTERNAL_FLASH_USE_QSPI)
  Adafruit_FlashTransport_QSPI flashTransport;

#elif defined(EXTERNAL_FLASH_USE_SPI)
  Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

#else
  #error No QSPI/SPI flash are defined on your board variant.h !
#endif

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;

#define SD_CS_PIN 10

const char Config::config_file_name[] = "config.txt";
bool Config::is_initialized = false;

bool Config::init() {
  if (Config::is_initialized) {
    return true;
  }

  if (!flash.begin()) {
    Serial.println("Failed to initialize the flash chip.");
    return false;
  }

  if (!fatfs.begin(&flash)) {
    Serial.println("Failed to mount the flash filesystem.");
    return false;
  }

  Config::is_initialized = true;
  return true;
};

configuration Config::read_config() {
  if (!Config::init()) {
    Serial.println("Failed to read config file.");
    return configuration {
      "GOL001",
      -4,
      "",
      "",
      10
    };
  }

  File f = fatfs.open(Config::config_file_name);
  if (f) {
    configuration toRet;
    int i = 0;
    while (f.available() && i < sizeof(configuration)) {
      *((char*)&toRet + i) = f.read();
      ++i;
    }

    return toRet;
  }

  Serial.println("No config file found on SD card.");
  return configuration {
    "GOL001",
    -4,
    "",
    "",
    10
  };
};

bool Config::write_config(configuration conf) {
  if (!Config::init()) {
    Serial.println("Failed to write config file.");
    return false;
  }

  File f = fatfs.open(Config::config_file_name, FILE_WRITE);
  if (f) {
    f.seek(0);
    f.write((char *)&conf, sizeof(configuration));
    f.close();
    return true;
  }

  Serial.println("Failed to open the file to write config file.");
  return false;
};
