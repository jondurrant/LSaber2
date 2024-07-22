/*
 * BladeSeqAnim.cpp
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqAnim.h"
#include "pico/stdlib.h"


BladeSeqAnim::BladeSeqAnim() {
	// TODO Auto-generated constructor stub

}

BladeSeqAnim::~BladeSeqAnim() {
	// TODO Auto-generated destructor stub
}

void BladeSeqAnim::on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	xBladeSeq = BladeSeqTurnOn;
	xStartTime = nowMS();
}

void BladeSeqAnim::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	xBladeSeq = BladeSeqTurnOff;
	xStartTime = nowMS();
}

void BladeSeqAnim::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){

	if ((xBladeSeq == BladeSeqTurnOn) || (xBladeSeq == BladeSeqTurnOff)){
		uint32_t sinceStart = nowMS() - xStartTime;

		if (sinceStart > BLADESEQ_ANIM_MS){
			if (xBladeSeq == BladeSeqTurnOff){
				bladeOff(length, c, strip, length);
				xBladeSeq = BladeSeqOff;
			} else {
				bladeOn(length, c, strip, length);
				xBladeSeq = BladeSeqOn;
			}
			lastStep = 0;
			strip->show();
		} else {
			float fStep = (float)sinceStart / ((float)BLADESEQ_ANIM_MS / (float)length );
			//uint32_t step =  sinceStart / (BLADESEQ_ANIM_MS / length);
			uint32_t step = (int)fStep;
			if (step > length){
				step = length;
			}
			if (step > lastStep){
				if (xBladeSeq == BladeSeqTurnOff){
					bladeOff(step, c, strip, length);
				} else {
					bladeOn(step, c, strip, length);
				}
				lastStep = step;
				strip->show();
			}
		}
	}
}

uint32_t BladeSeqAnim::nowMS(){
	return to_ms_since_boot(get_absolute_time());
}
