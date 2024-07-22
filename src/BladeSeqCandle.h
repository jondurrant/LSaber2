/*
 * BladeSeqCandle.h
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQCANDLE_H_
#define BLADESEQCANDLE_H_

#include "BladeSeqInterface.h"

#define CANDLE_FRACTION 20

class BladeSeqCandle : public BladeSeqInterface {
public:
	BladeSeqCandle();
	virtual ~BladeSeqCandle();

	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQCANDLE_H_ */
