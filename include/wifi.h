/**
Version: 0a
Tecnocoli - 10/2022
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
STM32F401 - Clothes Dryer
**/

#ifndef __WIFI_H__
#define __WIFI_H__

void printMacAddress(byte mac[]);
void printCurrentNet();
void printWifiData();
void printEncryptionType(int thisType);
void listNetworks();
void wifi_begin();
void checkStatus();



#endif
