/*
 * BladeSeqBright.h
 *
 *  Created on: 3 Jan 2022
 *      Author: jondurrant
 */

#ifndef BLADESEQBRIGHT_H_
#define BLADESEQBRIGHT_H_

#include "BladeSeqInterface.h"

class BladeSeqBright : public BladeSeqInterface {
public:
	BladeSeqBright();
	virtual ~BladeSeqBright();

	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQBRIGHT_H_ */
