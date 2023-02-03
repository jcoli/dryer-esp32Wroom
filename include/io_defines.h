/**
Version: 0a
Tecnocoli - 06/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
STM32F411 - Clothes Dryer
**/

#ifndef __IO_DEFINES_H__
#define __IO_DEFINES_H__

// #define SPIWIFI       SPI  // The SPI port
// // #define MOSI          PA7
// // #define MISO          PA6
// // #define SCK           PA5
// #define SPIWIFI_SS    PA4   // Chip select pin
// #define ESP32_RESETN  PA2   // Reset pin
// #define SPIWIFI_ACK   PA3   // a.k.a BUSY or READY pin
// #define ESP32_GPIO0   -1

#define TFT_SDA     21
#define TFT_SCL     22
#define TFT_CS      19
#define TFT_RST     18
#define TFT_DC      5
#define TFT_BACKLIGHT 17

#define BT_DOOR     25
#define BT_POWER    26 
#define BT_PROGRAM  27
#define BT_CYCLE    14
 
#define PW_FAN      16 
#define PW_HEATER   4
#define BUZZER_PIN  13

#define THERM1      32
#define THERM2      33

#define WIFI_LED    12

#endif