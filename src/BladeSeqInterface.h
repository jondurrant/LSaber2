/*
 * BladeSeqInterface.h
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQINTERFACE_H_
#define BLADESEQINTERFACE_H_

#include <PicoLed.hpp>
#include <stdint.h>

#include "BladeColour.h"

enum BladeSeqEnum {BladeSeqOff, BladeSeqTurnOn, BladeSeqOn, BladeSeqTurnOff};

class BladeSeqInterface {
public:
	BladeSeqInterface();
	virtual ~BladeSeqInterface();


	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length)=0;

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length)=0;

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length)=0;



};

#endif /* BLADESEQINTERFACE_H_ */
