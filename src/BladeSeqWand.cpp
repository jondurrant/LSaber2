/*
 * BladeSeqWand.cpp
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqWand.h"

BladeSeqWand::BladeSeqWand() {
	// TODO Auto-generated constructor stub

}

BladeSeqWand::~BladeSeqWand() {
	// TODO Auto-generated destructor stub
}

void BladeSeqWand::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	uint8_t pixels = length / WAND_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}

	strip->clear();
	int start = count - pixels;
	if (start < 0)
		start = 0;
	if (start > (length - pixels *2))
		start = length - pixels*2;
	int end = count + pixels;
	if (end > length )
		end = length;

	for (uint i = start; i < end; i++){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
}

void BladeSeqWand::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	if (count < length){

		uint8_t pixels = length / WAND_FRACTION;
		if (pixels < 0){
			pixels = 1;
		}

		if (count < pixels){
			strip->clear();
			uint8_t start = length - pixels;
			for (uint8_t i = start; i < start + count; i++){
				c->get(r, g, b, i, length);
				strip->setPixelColor(i, PicoLed::RGB(r, g, b));
			}
		} else {
			strip->clear();
		}
	}
}

