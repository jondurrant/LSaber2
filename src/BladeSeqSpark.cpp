/*
 * BladeSeqSpark.cpp
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqSpark.h"
#include <stdio.h>

BladeSeqSpark::BladeSeqSpark() {
	// TODO Auto-generated constructor stub

}

BladeSeqSpark::~BladeSeqSpark() {
	// TODO Auto-generated destructor stub
}

void BladeSeqSpark::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;
	int start, end;
	float bright = 1.0;

	uint8_t pixels = length / SPARK_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}

	uint8_t half = length /2;

	if (count < half){
		start = count - pixels;
		end = count + pixels;

		bright = 1.5;

	} /*else if (count < (half + pixels)){
		start = half - pixels;
		end = half + pixels;

		bright = 1.2;
	} */else {
		int burstStep = count - half;
		pixels = pixels + burstStep;
		start = half - pixels;
		end = half + pixels;
	}

	if (start > (length - pixels *2))
		start = length - pixels*2;
	if (start < 0)
		start = 0;
	if (end > length )
		end = length;

	strip->clear();
	//printf("ON %d, %d-%d\n", count, start, end);
	for (uint8_t i = start; i < end; i++){
		if ((count < length) && (bright != 1.5)){
			if ((i<start+2) || (i>end-3)) {
				bright = 1.2;
			} else {
				bright = 1.0;
			}
		}
		c->get(r, g, b, i, length, bright);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
}

void BladeSeqSpark::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;
	strip->clear();
	float bright = (float)count / (float)length;
	if (bright < 0.1)
		bright = 0.1;

	if (count < length){
		uint8_t newCount = length - count;
		for (uint i = 0; i < newCount; i++){
			c->get(r, g, b, i, length, bright);
			strip->setPixelColor(i, PicoLed::RGB(r, g, b));
		}
	}
}
