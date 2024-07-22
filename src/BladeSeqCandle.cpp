/*
 * BladeSeqCandle.cpp
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqCandle.h"
#include <stdio.h>

BladeSeqCandle::BladeSeqCandle() {
	// TODO Auto-generated constructor stub

}

BladeSeqCandle::~BladeSeqCandle() {
	// TODO Auto-generated destructor stub
}



void BladeSeqCandle::on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	uint8_t pixels = length / CANDLE_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}

	//strip->clear();
	int start = length - pixels;
	if (start < 0)
		start = 0;

	for (uint i = start; i < length; i++){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
	strip->show();
}

void BladeSeqCandle::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	strip->show();
}

void BladeSeqCandle::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	//NOP
}

