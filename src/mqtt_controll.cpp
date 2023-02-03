/**
Version: 0a
Tecnocoli - 10/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
STM32F401 - Clothes Dryer
**/
#include <Arduino.h>
#include <ArduinoHA.h> //MQTT
#include <WiFi.h>
// #include <WiFiClient.h>



#include "defines.h"
#include "io_defines.h"
#include "wifi.h"
#include "display.h"
#include "program.h"
#include "sounds.h"

void mqtt_begin();
void mqtt_loop();
void mqtt_update();
void onSelectProgram(int8_t index, HASelect* sender);
void on_switch_on(bool state, HASwitch* sender);


WiFiClient wifiClient;

extern byte mac[6];
extern bool wifi_connected;


extern float tempIn;
extern float tempOut;
extern float deltaTemp;
extern bool dryerState;
extern bool heaterState;
extern bool heaterEnable;
extern bool fanState;
extern bool fanEnable;
extern bool dryerOn;
extern bool doorState;
extern int dryerCicle;
extern int dryerProgram;
extern bool cooling;
extern bool sleeping;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_sleep;



byte unique[6];


HADevice device;
HAMqtt mqtt(wifiClient, device, 20);
HASwitch switch_on("switch_dryer_on_5");
HABinarySensor door_state("door_dryer_state_5");
HABinarySensor heater_state("heater_dryer_state_5");
HABinarySensor heater_enable("heater_dryer_enable_5");
HABinarySensor fan_state("fan_dryer_state_5");
HABinarySensor fan_enable("fan_dryer_enable_5");
HABinarySensor state_sensor("on_dryer_state_5");
HASensorNumber temp_in_sensor("dryer_temp_in_5", HASensorNumber::PrecisionP1);
HASensorNumber temp_out_sensor("dryer_temp_out_5", HASensorNumber::PrecisionP1);
HASensorNumber delta_sensor("dryer_delta_temp_5", HASensorNumber::PrecisionP1);
HASensor dryer_cycle("dryer_cycle_5");
HASelect program_select("dryer_program_select_5");


void mqtt_begin(){
    Serial.println("mqtt");
    // device.enableSharedAvailability();
    device.enableLastWill();
    // mqtt.onMessage(callback_mqtt);
    Serial.println("mqtt1z");

    unique[0] = 11;
    unique[1] = 99;
    unique[2] = 66;
    unique[3] = 56;
    unique[4] = 23;
    unique[5] = 25;

    Serial.println("mqtt1");
  
    // WiFi.macAddress(mac);
    
    device.setUniqueId(unique, 6);
    device.setName("Clothes Dryer_5");
    device.setSoftwareVersion("2.0.0");
    device.setManufacturer("Tecnocoli");
    device.setModel("10A");
    
    switch_on.setName("Liga");
    switch_on.onCommand(on_switch_on);
    switch_on.setIcon("mdi:tumble-dryer"); 
    switch_on.setState(dryerOn);  

    door_state.setName("Porta");
    door_state.setDeviceClass("door");
    door_state.setIcon("mdi:door");
    door_state.setState(!doorState);
    door_state.setCurrentState(doorState);
    
    fan_state.setName("Ventilador Acionado");
    fan_state.setDeviceClass("power");
    fan_state.setIcon("mdi:fan");
    fan_state.setState(fanState);
    fan_state.setCurrentState(fanState);

    fan_enable.setName("Ventilador Habilitado");
    fan_enable.setDeviceClass("power");
    fan_enable.setIcon("mdi:fan");
    fan_enable.setState(fanEnable);
    fan_enable.setCurrentState(fanEnable);

    heater_state.setName("Aquecedor Acionado");
    heater_state.setDeviceClass("power");
    heater_state.setIcon("mdi:heat-wave");
    heater_state.setState(heaterState);
    heater_state.setCurrentState(heaterState);

    heater_enable.setName("Aquecedor Habilitado");
    heater_enable.setDeviceClass("power");
    heater_enable.setIcon("mdi:heat-wave");
    heater_enable.setState(heaterEnable);
    heater_enable.setCurrentState(heaterEnable);

    state_sensor.setName("Estado Secadora");
    state_sensor.setDeviceClass("power");
    state_sensor.setIcon("mdi:power");
    state_sensor.setState(dryerState);
    state_sensor.setCurrentState(dryerState);

    temp_in_sensor.setName("Temperatura IN");
    temp_in_sensor.setUnitOfMeasurement("°C");
    temp_in_sensor.setDeviceClass("temperature");
    temp_in_sensor.setValue(tempIn);
    temp_in_sensor.setCurrentValue(tempOut);

    temp_out_sensor.setName("Temperatura OUT");
    temp_out_sensor.setUnitOfMeasurement("°C");
    temp_out_sensor.setDeviceClass("temperature");
    temp_out_sensor.setValue(tempOut);
    temp_out_sensor.setCurrentValue(tempOut);
    
    delta_sensor.setName("Temperatura Delta");
    delta_sensor.setUnitOfMeasurement("°C");
    delta_sensor.setDeviceClass("temperature");
    delta_sensor.setValue(deltaTemp);
    delta_sensor.setCurrentValue(deltaTemp);

    dryer_cycle.setName("Ciclo Secadora");
    dryer_cycle.setIcon("mdi:tumble-dryer");
    dryer_cycle.setValue("Desligado");

    program_select.setOptions("Normal;Medio;Leve;Aquecer");
    program_select.onCommand(onSelectProgram);
    program_select.setName("Programa");
    program_select.setIcon("mdi:curtains-closed");
    program_select.setState(dryerProgram);
    program_select.setCurrentState(dryerProgram);
    program_select.setOptimistic(false);
    program_select.setRetain(false);

    Serial.println("mqtt2");
    if (wifi_connected){
        int status=mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
        Serial.print("Broker: ");
        Serial.println(status);
        delay(500);
    } 
    
    if (mqtt.isConnected()){
      Serial.println("mqtt 3");
    }else{
      Serial.println("mqtt 3a");
    }
    
    


  
}

void mqtt_loop(){
  // if (mqtt.isConnected()){
    // Serial.println("Mqtt Loop");
    mqtt.loop();
  // }else{
    // mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
    // Serial.println("Mqtt Reconect");
  // }
  
//   Serial.println("Mqtt Loop");
}

void mqtt_update(){
  // if (mqtt.isConnected()){
  //     // Serial.println("mqtt 30");
  //   }else{
      // Serial.println("mqtt 30a");
      // mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
    // } 
 
  state_sensor.setState(dryerState);
  door_state.setState(!doorState);
  fan_state.setState(fanState);
  fan_enable.setState(fanEnable);
  heater_state.setState(heaterState);
  heater_enable.setState(heaterEnable);
  temp_in_sensor.setValue(tempIn);
  temp_out_sensor.setValue(tempOut);
  delta_sensor.setValue(deltaTemp);
  switch (dryerCicle){
    case 0:
            dryer_cycle.setValue("Desligado");
            break;
        case 1:
            dryer_cycle.setValue("Pre_Aquecer");
            break;
        case 2:
            dryer_cycle.setValue("Secagem");
            break;
        case 3:
            dryer_cycle.setValue("Fim Secagem");
            break;
        case 4:
            dryer_cycle.setValue("Resfriando");
            break;
        default:
            // unknown option
            return;

  }
  program_select.setState(dryerProgram);
  // program_select.setCurrentState(dryerProgram);
  switch_on.setState(dryerOn);
  Serial.println("Mqtt Update");
   
}

void on_switch_on(bool state, HASwitch* sender){
    Serial.println("Switch On");
    if (sleeping){
            sleeping = false;
            digitalWrite(TFT_BACKLIGHT, HIGH);
            play_start_sound();
            loopDelay_sleep = millis();
    }
    dryerOn = (state ? HIGH : LOW);
    sender->setState(state);
    change_data();
    if (dryerOn){
      Serial.println("Dryer On");
      program_begin();
      delay(500);
      program_controll();;
    }else{
      Serial.println("Dryer Off");
      program_controll();;
  }
}

void onSelectProgram(int8_t index, HASelect* sender){
  if (!dryerOn){
      switch (index) {
        case 0:
            dryerProgram = 0;
            Serial.println("Normal");
            break;
        case 1:
            dryerProgram = 1;
            Serial.println("Medio");
            break;
        case 2:
            dryerProgram = 2;
            Serial.println("Leve");
            break;
        case 3:
            dryerProgram = 3;
            Serial.println("Aquecer");
            break;
        default:
            // unknown option
            return;
        }
        sender->setState(index);
        change_data();
  }else{
    // delay(200);
    Serial.println("not select");
    // program_select.setState(dryerProgram);
    // program_select.setCurrentState(dryerProgram);
   
  }
}



