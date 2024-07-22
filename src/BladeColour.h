/*
 * BladeColour.h
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADECOLOUR_H_
#define BLADECOLOUR_H_
#include <stdint.h>




class BladeColour {
public:
	BladeColour();
	virtual ~BladeColour();

	uint8_t getBlue() const ;

	void setBlue(uint8_t blue = 0);

	uint8_t getGreen() const ;

	void setGreen(uint8_t green = 0) ;

	uint8_t getRed() const ;

	void setRed(uint8_t red = 0) ;

	void set(uint8_t red, uint8_t green, uint8_t blue);

	void get(uint8_t &r, uint8_t &g, uint8_t &b,
			uint8_t led = 0, uint8_t length = 1, float bright = 1.0);

	bool isBlack();

private:
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
};

#endif /* BLADECOLOUR_H_ */
