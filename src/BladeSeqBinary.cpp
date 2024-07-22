/*
 * BladeSeqBinary.cpp
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqBinary.h"

BladeSeqBinary::BladeSeqBinary() {
	// TODO Auto-generated constructor stub

}

BladeSeqBinary::~BladeSeqBinary() {
	// TODO Auto-generated destructor stub
}

void BladeSeqBinary::on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;
	if (c->isBlack()){
		for (uint i = 0; i < length; i++){
			c->get(r, g, b, i, length);
			strip->setPixelColor(i, PicoLed::RGB(r, g, b));
		}

	} else {
		c->get(r, g, b, 0, length);
		strip->fill( PicoLed::RGB(r, g, b));
	}

	strip->show();
}

void BladeSeqBinary::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	strip->show();
}

void BladeSeqBinary::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	//NOP
}
