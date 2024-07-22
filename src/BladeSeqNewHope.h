/*
 * BladeSeqNewHope.h
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQNEWHOPE_H_
#define BLADESEQNEWHOPE_H_

#include "BladeSeqAnim.h"

#define NEWHOPEMS 500
#define SPARK_FRACTION 20

class BladeSeqNewHope : public BladeSeqAnim {
public:
	BladeSeqNewHope();
	virtual ~BladeSeqNewHope();

protected:

	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
	virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQNEWHOPE_H_ */
