/*
 * BladeSeqNewHope.cpp
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqNewHope.h"
#include "pico/stdlib.h"
#include <stdio.h>

BladeSeqNewHope::BladeSeqNewHope() {
	// TODO Auto-generated constructor stub

}

BladeSeqNewHope::~BladeSeqNewHope() {
	// TODO Auto-generated destructor stub
}

void BladeSeqNewHope::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;
	float bright = 1.0;

	printf("NewHope %d of %d\n", count, length);

	uint8_t pixels = length / SPARK_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}
	pixels = count - pixels;

	for (uint i = 0; i < count; i++){
		if (count < (length - 1)){
			if ( i > pixels){
				bright = (float)(i + pixels) / (float)pixels;
			}
		} else {
			bright = 1.0;
		}
		c->get(r, g, b, i, length, bright);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
		//printf("NH %d [%x, %x, %x]\n", i, r, g, b);
	}
}

void BladeSeqNewHope::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	if (count < length){
		bladeOn(length - count, c, strip, length);
	}
}

