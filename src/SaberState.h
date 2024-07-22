/*
 * SaberState.h
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#ifndef SABERSTATE_H_
#define SABERSTATE_H_

#include <MQTTConfig.h>
#include "StateTemp.h"
#include <stdbool.h>
#include <stdint.h>

#define SABER_DAY_RGB_SLOT 		3
#define SABER_NIGHT_RGB_SLOT 	4
#define SABER_ON_SLOT  			5
#define SABER_ID_SLOT 			6
#define SABER_DAY_START_SLOT	7
#define SABER_DAY_END_SLOT		8
#define SABER_DAY_SEQ_SLOT		9
#define SABER_NIGHT_SEQ_SLOT	10
#define SABER_DAY_SLOT			11
#define SABER_TIMER_SLOT		12
#define SABER_CLOCK_SLOT		13



class SaberState : public StateTemp{
public:
	SaberState();
	virtual ~SaberState();

	/***
	 * Get RGB as an array
	 * @return unsigned char[]
	 */
	const uint8_t* getDayRGB() const;

	/***
	 * Get RGB as an array
	 * @return unsigned char[]
	 */
	const uint8_t* getNightRGB() const;

	/***
	 * Sets RGB based on array provided.
	 * Copies array so no need to allocate parameter
	 * @param newRGB - uint8_t[3]
	 */
	void setDayRGB(uint8_t* newRGB);

	/***
	 * Sets RGB based on array provided.
	 * Copies array so no need to allocate parameter
	 * @param newRGB - uint8_t[3]
	 */
	void setNightRGB(uint8_t* newRGB);

	/***
	 * Sets RGB based on three parameters
	 * @param r
	 * @param g
	 * @param b
	 */
	void setDayRGB(uint8_t r,  uint8_t g, uint8_t  b);

	/***
	 * Sets RGB based on three parameters
	 * @param r
	 * @param g
	 * @param b
	 */
	void setNightRGB(uint8_t r,  uint8_t g, uint8_t  b);

	/***
	 * Gets the On status of the light
	 * @return
	 */
	bool getOn() const;

	/***
	 * Sets the on status of the light
	 * @param b
	 */
	void setOn(bool b);

	/***
	 * Update state data from a json structure
	 * @param json
	 */
	virtual void updateFromJson(json_t const *json);

	/***
	 * Retrieve state of object in JSON format
	 * @param buf - buffer to write to
	 * @param len - length of buffer
	 * @return length of json or zero if we ran out of space
	 */
	virtual unsigned int state(char *buf, unsigned int len) ;

	/***
	 * Get Unit ID
	 * @return
	 */
	uint8_t getId() const ;

	/***
	 * Set Unit ID
	 * @param id
	 */
	void setId(uint8_t id = 0);

	/***
	 * Returns hour at which Day Ends
	 * @return
	 */
	uint8_t getDayEnd() const ;

	/***
	 * Sets hour at which day ends
	 * @param DayEnd
	 */
	void setDayEnd(uint8_t xDayEnd = 21) ;

	/***
	 * Get Start Day Hour
	 * @return start hour
	 */
	uint8_t getDayStart() const ;

	/***
	 * Sets Day start hour
	 * @param xDayStart
	 */
	void setDayStart(uint8_t xDayStart = 7) ;

	/***
	 * Get the sequence for the blade
	 * int number of the sequence.
	 * @return
	 */
	uint8_t getDaySeq() const ;

	/***
	 * Set the sequence number for the blade
	 * @param xSeq
	 */
	void setDaySeq(uint8_t xSeq = 0) ;

	/***
	 * Get the sequence for the blade
	 * int number of the sequence.
	 * @return
	 */
	uint8_t getNightSeq() const ;

	/***
	 * Set the sequence number for the blade
	 * @param xSeq
	 */
	void setNightSeq(uint8_t xSeq = 0) ;

	/***
	 * Is in day mode
	 * @return
	 */
	bool isDay() const ;

	/***
	 * Set day mode
	 * @param xDay
	 */
	void setDay(bool xDay = true) ;

	/***
	 * Get ON timer  in seconds
	 * @return
	 */
	uint16_t getTimerS() const ;

	/***
	 * set ON timer in seconds
	 * @param xTimerS
	 */
	void setTimerS(uint16_t xTimerS = 30) ;

	/***
	 * alert patturn
	 */
	void alert(uint8_t lvl);

	/***
	 * Update time and temp and trigger state update
	 */
	void updateClock();

protected:
	/***
	 * Retrieve RGB in json format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonDayRGB(char *buf, unsigned int len);

	/***
	 * Retrieve RGB in json format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonNightRGB(char *buf, unsigned int len);

	/***
	 * Retried On status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonOn(char *buf, unsigned int len);

	/***
	 * Retried Id status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonId(char *buf, unsigned int len);


	/***
	 * Retried Day Start Hour status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonDayStart(char *buf, unsigned int len);

	/***
	 * Retried Day End Hour status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonDayEnd(char *buf, unsigned int len);

	/***
	 * Retried Seq number in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonDaySeq(char *buf, unsigned int len);

	/***
	 * Retried Seq number in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonNightSeq(char *buf, unsigned int len);

	/***
	 * Retried Day in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonDay(char *buf, unsigned int len);

	/***
	 * Retried Day in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonTimer(char *buf, unsigned int len);

	/***
	 * Retried Time in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonClock(char *buf, unsigned int len);

private:
	//Light colour in RGB format, one bite per element
	uint8_t xDayRGB[3] = {0, 0, 0};
	uint8_t xNightRGB[3] = {0, 0, 0};

	//Is light on
	bool xOn = false;

	//ID could be used to group lights for delay turn on
	uint8_t xId = 0;

	//Dat start and end
	uint8_t xDayStart = 7;
	uint8_t xDayEnd = 21;
	bool xDay = true;

	//Sequence for Blade
	uint8_t xDaySeq = 4;
	uint8_t xNightSeq = 3;

	uint16_t xTimerS = 30;
};

#endif /* SABERSTATE_H_ */
