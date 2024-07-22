/*
 * BladeSeqWand.h
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQWAND_H_
#define BLADESEQWAND_H_

#include "BladeSeqAnim.h"

#define WAND_FRACTION 20

class BladeSeqWand : public BladeSeqAnim  {
public:
	BladeSeqWand();
	virtual ~BladeSeqWand();

protected:

	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
	virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQWAND_H_ */
