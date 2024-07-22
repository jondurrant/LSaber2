/*
 * BladeRequest.cpp
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#include "BladeRequest.h"
#include <stdio.h>
#include "pico/multicore.h"

BladeRequest::BladeRequest() {
	// TODO Auto-generated constructor stub

}

BladeRequest::~BladeRequest() {
	// TODO Auto-generated destructor stub
}

void BladeRequest::setColour(uint8_t red, uint8_t green, uint8_t blue){
	setRed(red);
	setGreen(green);
	setBlue(blue);
}

uint8_t BladeRequest::getBlue() const {
	return blue;
}

void BladeRequest::setBlue(uint8_t blue) {
	this->blue = blue;
}

uint8_t BladeRequest::getGreen() const {
	return green;
}

void BladeRequest::setGreen(uint8_t green) {
	this->green = green;
}

uint8_t BladeRequest::getRed() const {
	return red;
}

void BladeRequest::setRed(uint8_t red) {
	this->red = red;
}

BladeReqType BladeRequest::getReq() const {
	return req;
}

void BladeRequest::setReq(BladeReqType req) {
	this->req = req;
}

bool BladeRequest::writeToQueue(){
	uint32_t mesg = 0;

	mesg = red;
	mesg = mesg << 8;
	mesg = mesg | green;
	mesg = mesg << 8;
	mesg = mesg | blue;
	mesg = mesg << 8;
	mesg = mesg | (uint8_t)req;
	//printf("Req %x [%x, %x, %x]\n", req, red, green, blue);
	//printf("QUEUE WRITE %x\n", mesg);

	if (multicore_fifo_wready()){
		multicore_fifo_push_timeout_us(mesg, 0);
	} else {
		printf("FIFO FULL\n");
		return false;
	}

	BladeRequest r;
	r.set(mesg);
	return true;

}

bool BladeRequest::readFromQueue(){
	uint32_t mesg;

	if (!multicore_fifo_rvalid()){
		return false;
	}

	if (multicore_fifo_pop_timeout_us(0, &mesg)){
		set(mesg);
	} else {
		return false;
	}

	return true;
}


uint32_t BladeRequest::get(){
	uint32_t mesg = red;
	mesg = mesg << 8;
	mesg = mesg | green;
	mesg = mesg << 8;
	mesg = mesg | blue;
	mesg = mesg << 8;
	mesg = mesg | (uint8_t)req;

	return mesg;
}

void BladeRequest::set(uint32_t mesg){
	req = (BladeReqType) (mesg & 0xFF);
	uint32_t m = mesg >> 8;
	blue = (uint8_t)(m & 0xFF);
	m = m >> 8;
	green = (uint8_t)(m & 0xFF);
	m = m >> 8;
	red = (uint8_t)(m & 0xFF);
	//printf("Set %x [%x, %x, %x]\n", req, red, green, blue);
}

uint8_t BladeRequest::getSeq()  const{
	return getRed();
}

void BladeRequest::setSeq(uint8_t seq){
	setRed(seq);
}

void BladeRequest::setSource(BladeSourceType source){
	setRed((uint8_t)source);
}

BladeSourceType BladeRequest::getSource (){
	return (BladeSourceType) getRed();
}


void BladeRequest::setTimer(uint16_t seconds){
	uint8_t b;

	b = seconds & 0xFF;
	setRed(b);
	b = seconds << 8;
	setGreen(b);
}

uint16_t BladeRequest::getTimer(){
	uint16_t res;
	res = getGreen() >> 8;
	res = res | getRed();
	return res;
}
