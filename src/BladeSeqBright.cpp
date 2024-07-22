/*
 * BladeSeqBright.cpp
 *
 *  Created on: 3 Jan 2022
 *      Author: jondurrant
 */

#include "BladeSeqBright.h"
#include <stdio.h>

BladeSeqBright::BladeSeqBright() {
	// TODO Auto-generated constructor stub

}

BladeSeqBright::~BladeSeqBright() {
	// TODO Auto-generated destructor stub
}


void BladeSeqBright::on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;
	float bright = 0.0;

	for (uint i = 0; i < length; i++){
		bright = ((float)i/(float)length) * 2.0;
		c->get(r, g, b, i, length, bright);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
		//printf("Bright(l=%d, b=%f), [%x,%x,%x]\n", i, bright, r, g, b );
	}

	strip->show();
}

void BladeSeqBright::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	strip->show();
}

void BladeSeqBright::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	//NOP
}

