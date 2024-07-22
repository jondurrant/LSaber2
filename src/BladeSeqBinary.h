/*
 * BladeSeqBinary.h
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQBINARY_H_
#define BLADESEQBINARY_H_

//#include <PicoLed.hpp>
//#include <stdint.h>


#include "BladeSeqInterface.h"



class BladeSeqBinary : public BladeSeqInterface {
public:
	BladeSeqBinary();
	virtual ~BladeSeqBinary();

	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);


};

#endif /* BLADESEQBINARY_H_ */
