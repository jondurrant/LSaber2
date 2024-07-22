/*
 * BladeSeqAnim.h
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQANIM_H_
#define BLADESEQANIM_H_

#include "BladeSeqInterface.h"

#define BLADESEQ_ANIM_MS 1000

class BladeSeqAnim : public BladeSeqInterface {
public:
	BladeSeqAnim();
	virtual ~BladeSeqAnim();

	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

protected:

	BladeSeqEnum xBladeSeq = BladeSeqOff;
	uint32_t xStartTime = 0; //MS
	uint32_t lastStep = 0;

	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length) = 0;
	virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length) = 0;

	uint32_t nowMS();
};

#endif /* BLADESEQANIM_H_ */
