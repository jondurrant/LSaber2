/*
 * BladeStateAgent.cpp
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#include "BladeStateAgent.h"

#include "BladeRequest.h"
#include "SaberState.h"

#include "hardware/rtc.h"


BladeStateAgent::BladeStateAgent() {
	// TODO Auto-generated constructor stub

}

BladeStateAgent::~BladeStateAgent() {
	// TODO Auto-generated destructor stub
}

/***
 *  create the vtask, will get picked up by scheduler
 *
 *  */
void BladeStateAgent::start(UBaseType_t priority){
	xMessageBuffer = xMessageBufferCreate( STATE_MSG_BUF_LEN );
	if (xMessageBuffer == NULL){
		LogError( ("Create buf failed") );
		return;
	}
	if (xMessageBuffer != NULL){
		xTaskCreate(
			BladeStateAgent::vTask,
			"MQTTstate",
			512,
			( void * ) this,
			priority,
			&xHandle
		);
	}
}

/***
 * Internal function used by FreeRTOS to run the task
 * @param pvParameters
 */
void BladeStateAgent::vTask( void * pvParameters ){
	BladeStateAgent *task = (BladeStateAgent *) pvParameters;
	task->run();
}

/***
 * Internal function to run the task from within the object
 */
void BladeStateAgent::run(){

	BladeRequest req;
	SaberState *pSaberState = (SaberState *) pState;

	for (;;){
		if (xMessageBufferIsEmpty(xMessageBuffer) == pdTRUE){
			bool b = req.readFromQueue();
			if (b == true){
				if (req.getReq() == BladeOn){
					if (pSaberState->getOn() == false){
						pSaberState->setOn(true);
					}
					pubBladeState(true, req.getSource());
				}
				if (req.getReq() == BladeOff){
					if (pSaberState->getOn() == true){
						pSaberState->setOn(false);
						pubBladeState(false, req.getSource());
					}
				}
			}
			taskYIELD();
		} else {
			vTaskDelay( 20 );
			size_t size = xMessageBufferReceive(xMessageBuffer,
								xMsg, STATE_MAX_MSG_LEN, 0);
			if (size > 0){
				if (pState != NULL){
					processMsg(xMsg);
				}
			}
		}

		//Check for day end
		 datetime_t now;
		 rtc_get_datetime(&now);
		 if (now.min != xPrevMin){
			 xPrevMin = now.min;
			 if (
					 (now.hour >= pSaberState->getDayEnd()) ||
					 (now.hour < pSaberState->getDayStart())
				 ){
				 //It's night
				 if (pSaberState->isDay()){
					 pSaberState->setDay(false);
				 }
			 } else {
				 if (!pSaberState->isDay()){
					 pSaberState->setDay(true);
				 }
			 }
			 //pSaberState->updateTemp();
			 pSaberState->updateClock();

		 }



	}
}

/***
 * Notification of a change of a state item with the State object.
 * @param dirtyCode - Representation of item changed within state. Used to pull back delta
 */
void BladeStateAgent::notifyState(uint16_t dirtyCode){
	TwinTask::notifyState(dirtyCode);

}


void BladeStateAgent::setTopics(char * onTopic, char * offTopic){
	pOnTopic = onTopic;
	pOffTopic = offTopic;
}

void BladeStateAgent::pubBladeState(bool on, BladeSourceType source){
	char msg[44];
	SaberState *s = (SaberState *) pState;
	if (on){
		if (pOnTopic != NULL){
			sprintf(msg, "{\"from\":\"%s\", \"id\":%d, \"src\":%d}",
					mqttInterface->getId(), s->getId(), source
					);
			mqttInterface->pubToTopic(pOnTopic, msg, strlen(msg));
		}
	} else {
		if (pOffTopic != NULL){
			sprintf(msg, "{\"from\":\"%s\", \"id\":%d, \"src\":%d}",
					mqttInterface->getId(), s->getId(), source
					);
			mqttInterface->pubToTopic(pOffTopic, msg, strlen(msg));
		}
	}

}

/***
* Process a json message received
* @param str
*/
void BladeStateAgent::processJson(json_t const* json){
	TwinTask::processJson(json);

	json_t const* alert = json_getProperty(json, JSON_ALERT);
	if (alert){
		LogDebug(("Detected Alert"));
		if (JSON_INTEGER == json_getType(alert)){
			SaberState *s = (SaberState *) pState;
			s->alert(json_getInteger(alert));
		}
	}


}
