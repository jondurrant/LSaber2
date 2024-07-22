/*
 * BladeSeqAlert.h
 *
 *  Created on: 24 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQALERT_H_
#define BLADESEQALERT_H_

#include "BladeSeqAnim.h"

#define BLADE_ALERT_SEQ_MS 10000
#define ALERT_FRACTION 6

class BladeSeqAlert : public BladeSeqAnim {
public:
	BladeSeqAlert();
	virtual ~BladeSeqAlert();

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

protected:
	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
		virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQALERT_H_ */
