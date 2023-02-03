/**
Version: 0a
Tecnocoli - 06/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
STM32F411 - Clothes Dryer
**/
#include <Arduino.h>
#include <ArduinoHA.h> //MQTT

#include "io_defines.h"
#include "defines.h"
#include "display.h"
#include "sounds.h"
#include "input_digital_control.h"

void io_output_begin();

void io_output_begin(){
    
    pinMode(WIFI_LED, OUTPUT);
    pinMode(PW_FAN, OUTPUT);
    pinMode(PW_HEATER, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(WIFI_LED, LOW);
    digitalWrite(PW_FAN, LOW);
    digitalWrite(PW_HEATER, LOW);
    digitalWrite(BUZZER_PIN, LOW);

}
