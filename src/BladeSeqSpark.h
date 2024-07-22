/*
 * BladeSeqSpark.h
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQSPARK_H_
#define BLADESEQSPARK_H_

#include "BladeSeqAnim.h"

#define SPARK_FRACTION 20

class BladeSeqSpark : public BladeSeqAnim {
public:
	BladeSeqSpark();
	virtual ~BladeSeqSpark();

protected:

	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
	virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQSPARK_H_ */
