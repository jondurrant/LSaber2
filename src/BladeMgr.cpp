/*
 * BladeMgr.cpp
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#include "BladeMgr.h"
#include "BladeRequest.h"

#include "BladeSeqBinary.h"
#include "BladeSeqNewHope.h"
#include "BladeSeqWand.h"
#include "BladeSeqCandle.h"
#include "BladeSeqSpark.h"
#include "BladeSeqAlert.h"
#include "BladeSeqBright.h"

#include "hardware/gpio.h"
#include <stdio.h>

BladeMgr * BladeMgr::pSelf = NULL;

BladeMgr::BladeMgr(uint8_t pirPin, uint8_t switchPin) {
	pSelf = this;
	xPirPin = pirPin;
	xSwitchPin = switchPin;

	//Set up PIR
	gpio_init(xPirPin);
	gpio_set_dir(xPirPin, GPIO_IN);
	//gpio_pull_down(xPirPin);
	gpio_set_irq_enabled_with_callback(xPirPin,
		GPIO_IRQ_EDGE_RISE,
		true,
		gpioCallback
	);

	//Set Switch
	gpio_init(xSwitchPin);
	gpio_set_dir(xSwitchPin, GPIO_IN);
	gpio_pull_up (xSwitchPin);
	gpio_set_irq_enabled_with_callback(xSwitchPin,
		GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
		true,
		gpioCallback
	);

	initSeqs();
}

BladeMgr::~BladeMgr() {
	// TODO Auto-generated destructor stub
}


void BladeMgr::loopForever(){
	BladeRequest req;
	uint32_t mesg;


	ledStrip.setBrightness(255);

	ledStrip.fill( PicoLed::RGB(0, 0, 0) );
	ledStrip.show();

	for(;;){
		if (req.readFromQueue()){
			switch(req.getReq()){
			case BladeDRGB: {
				setColour(true, req.getRed(),
						req.getGreen(),
						req.getBlue()
						);

				//printf("\nBladeMgr colour\n");

				break;
			}
			case BladeNRGB: {
				setColour(false, req.getRed(),
						req.getGreen(),
						req.getBlue()
						);

				//printf("\nBladeMgr colour\n");

				break;
			}
			case BladeOn: {
				turnOn(true, BladeSourceControl);
				break;
			}
			case BladeOff: {
				turnOff(true, BladeSourceControl);
				break;
			}
			case BladeDSeq: {
				setSeq(true, req.getSeq());
				break;
			}
			case BladeNSeq: {
				setSeq(false, req.getSeq());
				break;
			}
			case BladeDay: {
				xDay = true;
				break;
			}
			case BladeNight: {
				xDay = false;
				break;
			}
			case BladeTimer: {
				xOffTimeMS = req.getTimer() * 1000;
				break;
			}
			}
		}

		if (xTimeTurnedOn != 0){
			uint32_t timeOn = to_ms_since_boot(get_absolute_time()) - xTimeTurnedOn;
			if (timeOn > xOffTimeMS){
				turnOff(false, BladeSourceTimeout);
				xTimeTurnedOn = 0;
			}
		}

		if (xDay){
			getBladeSeqs()->tick(&xDayColour, &ledStrip, LEDS_LENGTH);
		} else {
			getBladeSeqs()->tick(&xNightColour, &ledStrip, LEDS_LENGTH);
		}

	}


}

void BladeMgr::turnOn(bool remote,  BladeSourceType source){
	BladeRequest req;

	if (!isOn()){
		if (xDay){
			getBladeSeqs()->on(&xDayColour, &ledStrip, LEDS_LENGTH);
		} else {
			getBladeSeqs()->on(&xNightColour, &ledStrip, LEDS_LENGTH);
		}
	}



	xTimeTurnedOn = to_ms_since_boot(get_absolute_time());

	if (!remote){
		req.setReq(BladeOn);
		req.setSource(source);
		req.writeToQueue();
	}
	//printf("\nBladeMgr on\n");
	xOn = true;
}

void BladeMgr::turnOff(bool remote,  BladeSourceType source){
	BladeRequest req;

	if (isOn()){
		if (xDay){
			getBladeSeqs()->off(&xDayColour, &ledStrip, LEDS_LENGTH);
		} else {
			getBladeSeqs()->off(&xNightColour, &ledStrip, LEDS_LENGTH);
		}
	}



	xTimeTurnedOn = 0;

	if (!remote){
		req.setReq(BladeOff);
		req.setSource(source);
		req.writeToQueue();
	}
	//printf("\nBladeMgr off\n");

	xOn = false;
}



void BladeMgr::setColour( bool day, uint8_t r, uint8_t g, uint8_t b){
	if (day){
		xDayColour.set(r, g, b);
	} else {
		xNightColour.set(r, g, b);
	}
}


void BladeMgr::gpioCallback (uint gpio, uint32_t events){
	BladeMgr::pSelf->handleGPIO(gpio, events);
}


void BladeMgr::handleGPIO (uint gpio, uint32_t events){
	if (gpio == xPirPin){
		if ((events & 0x08) > 0){
			if (gpio_get(xPirPin))
				turnOn(false, BladeSourcePIR);
		}
	}


	if (gpio == xSwitchPin){
		if ((events & GPIO_IRQ_EDGE_FALL) > 0){
			xSwitchTime = to_ms_since_boot(get_absolute_time ());
		}
		if ((events & GPIO_IRQ_EDGE_RISE) > 0){
			uint32_t t = to_ms_since_boot(get_absolute_time ()) - xSwitchTime;
			if ((t < 100) || (t > 5000)){
				return;
			} else if (t < 1000){
				handleShortPress();
			} else {
				handleLongPress();
			}
		}
	}
}


void BladeMgr::handleShortPress(){
	if (isOn()){
		turnOff(false, BladeSourceSwitch);
	} else {
		turnOn(false, BladeSourceSwitch);
	}
}

void BladeMgr::handleLongPress(){
	//turnOn();
	xDay = ! xDay;
	turnOn(false, BladeSourceSwitch);
}

void BladeMgr::initSeqs(){
	xpBladeSeqs[0] = new BladeSeqAlert;
	xpBladeSeqs[1] = new BladeSeqNewHope;
	xpBladeSeqs[2] = new BladeSeqWand;
	xpBladeSeqs[3] = new BladeSeqCandle;
	xpBladeSeqs[4] = new BladeSeqSpark;
	xpBladeSeqs[5] = new BladeSeqBinary;
	xpBladeSeqs[6] = new BladeSeqBright;
	xDaySeqInd = 6;
	xNightSeqInd = 0;
}


BladeSeqInterface *  BladeMgr::getBladeSeqs(){
	if (xDay){
		return xpBladeSeqs[xDaySeqInd];
	} else {
		return xpBladeSeqs[xNightSeqInd];
	}
}

bool BladeMgr::isOn(){
	return xOn;
}

void BladeMgr::setSeq(bool day, uint8_t seq){
	if (seq < BLADE_SEQ_COUNT) {
		if (day){
			xDaySeqInd = seq;
		} else {
			xNightSeqInd = seq;
		}
	}
}

