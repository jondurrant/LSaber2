/*
 * BladeMgr.h
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADEMGR_H_
#define BLADEMGR_H_

#define LEDS_PIN 0
#define LEDS_LENGTH 144 //61

#include <PicoLed.hpp>
#include "pico/stdlib.h"
#include <stdint.h>

#include "BladeColour.h"
#include "BladeSeqInterface.h"
#include "BladeRequest.h"

#define BLADE_SEQ_COUNT 7

class BladeMgr {
public:
	BladeMgr(uint8_t pirPin = 16, uint8_t switchPin = 18);
	virtual ~BladeMgr();


	void loopForever();

	void setColour(bool day, uint8_t r, uint8_t g, uint8_t b);

	void turnOn(bool remote = false, BladeSourceType source = BladeSourceUnknown);

	void turnOff(bool remote = false,  BladeSourceType source = BladeSourceUnknown);

	bool isOn();

	void setSeq(bool day, uint8_t seq);

protected:

	void handleGPIO(uint gpio, uint32_t events);

	void handleShortPress();

	void handleLongPress();


private:
	static void gpioCallback (uint gpio, uint32_t events);

	void initSeqs();

	BladeSeqInterface * getBladeSeqs();

	BladeColour xDayColour;
	BladeColour xNightColour;
	bool xDay = true;

	BladeSeqInterface *xpBladeSeqs[BLADE_SEQ_COUNT];
	uint8_t xDaySeqInd = 0;
	uint8_t xNightSeqInd = 0;

	uint8_t xPirPin = 0;
	uint8_t xSwitchPin = 0;

	uint32_t xTimeTurnedOn = 0;

	uint32_t xOffTimeMS = 30000; //MS

	uint32_t xSwitchTime = 0;

	PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LEDS_PIN, LEDS_LENGTH, PicoLed::FORMAT_GRB);

	bool xOn = false;

	static BladeMgr * pSelf;


};

#endif /* BLADEMGR_H_ */
