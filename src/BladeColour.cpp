/*
 * BladeColour.cpp
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#include "BladeColour.h"
#include <ColorUtils.h>
#include <stdio.h>

BladeColour::BladeColour() {
	// TODO Auto-generated constructor stub

}

BladeColour::~BladeColour() {
	// TODO Auto-generated destructor stub
}

uint8_t BladeColour::getBlue() const {
	return blue;
}

void BladeColour::setBlue(uint8_t blue) {
	this->blue = blue;
}

uint8_t BladeColour::getGreen() const {
	return green;
}

void BladeColour::setGreen(uint8_t green) {
	this->green = green;
}

uint8_t BladeColour::getRed() const {
	return red;
}

void BladeColour::setRed(uint8_t red) {
	this->red = red;
}

void BladeColour::set(uint8_t red, uint8_t green, uint8_t blue){
	setRed(red);
	setGreen(green);
	setBlue(blue);

}

void BladeColour::get(uint8_t &r, uint8_t &g, uint8_t &b,
			uint8_t led, uint8_t length, float bright){

	if (isBlack()){
		if (length > 0){
			double val = ((double)led) / ((double) length - 1);
			ColorUtils::valToRGB (val, r, g, b);
			//printf("colour %d/%d = %f (%d, %d, %d)\n", led, length , val, r, g, b);
		}
	} else {
		r = getRed();
		g = getGreen();
		b = getBlue();
	}

	if (bright != 1.0){
		float threshold = 255;
		float fr = (float)r * bright;
		float fg = (float)g * bright;
		float fb = (float)b * bright;
		//MAX
		float m = (fr>fg)? fr:fb ;
		m = (m>fb)? m: fb;

		//printf("Bright %f Max(%f) [%f,%f,%f]\n", bright, m, fr, fg, fb);

		if (m > threshold){
			float total = fr + fg + fb;
			if (total > (threshold * 3)){
				fr= threshold;
				fg=threshold;
				fb=threshold;

				//printf("100% [%f,%f,%f]\n", fr, fg, fb);
			} else {
				float x = (3 * threshold - total) / (3 * m - total);
				float gray = threshold - x * m;
				fr = (gray + x * r);
				fg = (gray + x * g);
				fb =(gray + x * b);

				//printf("Grey [%f,%f,%f]\n", fr, fg, fb);
			}
		}

		//printf("Colour[%f,%f,%f]\n", fr, fg, fb);

		r= (uint8_t)fr;
		g= (uint8_t)fg;
		b= (uint8_t)fb;

	}


}

bool BladeColour::isBlack(){
	if ((red == 0) && (green == 0) && (blue == 0)){
		return true;
	} else {
		return false;
	}
}
