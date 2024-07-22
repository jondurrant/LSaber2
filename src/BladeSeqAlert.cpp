/*
 * BladeSeqAlert.cpp
 *
 *  Created on: 24 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqAlert.h"

BladeSeqAlert::BladeSeqAlert() {
	// TODO Auto-generated constructor stub

}

BladeSeqAlert::~BladeSeqAlert() {
	// TODO Auto-generated destructor stub
}



void BladeSeqAlert::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	if (xBladeSeq == BladeSeqTurnOff){
		bladeOff(0, NULL, strip, length);
	}

	if (xBladeSeq == BladeSeqTurnOn){
		uint32_t sinceStart = nowMS() - xStartTime;
		uint32_t seqTime = sinceStart % BLADE_ALERT_SEQ_MS;
		float fStep = (float)seqTime / ((float)BLADE_ALERT_SEQ_MS / (float)length );
		uint32_t step = (int)fStep;
		//uint32_t step = seqTime / (BLADE_ALERT_SEQ_MS / length);

		if (step > length){
			step = length;
		}

		if (step != lastStep){
			strip->clear();

			//printf("Step %d seqTime %d length %d\n", step, seqTime, length);
			bladeOn(step, c, strip, length);
			lastStep = step;

			if (sinceStart > (BLADE_ALERT_SEQ_MS/2)){
				step = (step + (length /2))% length;
				bladeOn(step, c, strip, length);
			}


			strip->show();
		}
	}

}

void BladeSeqAlert::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	uint8_t pixels = length / ALERT_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}
	uint8_t half = length/2;


	int start = count - pixels;
	if (start < 0)
		start = 0;
	start = start + half;
	if (start > length)
		start = length;

	int end = count + half;
	if (end > length )
		end = length;

	//printf("ON %d, %d>%d\n", count, start, end);
	for (uint i = start; i < end; i++){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}

	int rStart = length - start;
	int rEnd = length - end;
	if (rStart < 0)
		rStart = 0;
	if (rEnd < 0)
		rEnd = 0;

	//printf("ON %d, %d>%d %d<%d\n", count, start, end, rStart, rEnd);
	for (uint i = rStart; i > rEnd; i--){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
}

void BladeSeqAlert::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	strip->show();
	xBladeSeq = BladeSeqOff;
}

void BladeSeqAlert::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	strip->show();
	xBladeSeq = BladeSeqOff;
}
