/*
 * RGBLEDMgr.cpp
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#include "RGBLEDMgr.h"
#include <WifiHelper.h>
#include <stdlib.h>
#define LIBRARY_LOG_NAME "LSABER2"
#define LIBRARY_LOG_LEVEL LOG_INFO
#include "logging_stack.h"

RGBLEDMgr::RGBLEDMgr(RGBLEDAgent * led) {
	pLed = led;
}

RGBLEDMgr::~RGBLEDMgr() {
	// NOP
}

void RGBLEDMgr::MQTTOffline(){
	if (pLed != NULL){
		if (WifiHelper::isJoined()){
			pLed->set(RGBModeOn, 0xF5, 0xCE, 0x42);
		} else {
			pLed->set(RGBModeOn,0xFF, 0, 0);
		}
	} else {
		LogError(("No LED"));
	}

	LogDebug(("Offline"));
}

void RGBLEDMgr::MQTTOnline(){
	if (pLed != NULL){
		pLed->set(RGBModeOn,0, 0, 0xFF);
	}
	LogDebug(("Online"));
}

void RGBLEDMgr::MQTTSend(){
	if (pLed != NULL){
		pLed->set(RGBModeOnce,0, 0, 0xFF);
	}
	LogDebug(("Send"));
}

void RGBLEDMgr::MQTTRecv(){
	if (pLed != NULL){
		pLed->set(RGBModeOnce,0, 0, 0xFF);
	}
	LogDebug(("Recv"));
}
