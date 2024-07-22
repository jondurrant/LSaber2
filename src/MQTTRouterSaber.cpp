/*
 * MQTTRouterSaber.cpp
 *
 *  Created on: 20 Dec 2021
 *      Author: jondurrant
 */

#include "MQTTRouterSaber.h"
#include <MQTTTopicHelper.h>
#include "SaberState.h"

#define STATE_ON "{\"delta\": {\"on\": true}}"
#define STATE_OFF "{\"delta\": {\"on\": false}}"

MQTTRouterSaber::MQTTRouterSaber() {
	// NOP

}

MQTTRouterSaber::~MQTTRouterSaber() {
	if (pGroupTopicOn != NULL){
		vPortFree(pGroupTopicOn);
		pGroupTopicOn = NULL;
	}
	if (pGroupTopicOff != NULL){
		vPortFree(pGroupTopicOff);
		pGroupTopicOff = NULL;
	}
	if (pGroupTopicAlert != NULL){
		vPortFree(pGroupTopicAlert);
		pGroupTopicAlert = NULL;
	}
}

/***
 * Initialise the object give the Id and MQTT Interface
 * @param id = string ID of the Thing
 * @param mi = MQTT Interface
 */
void MQTTRouterSaber::init(const char * id, MQTTInterface *mi){
	MQTTRouterTwin::init(id, mi);

	if (pGroupTopicOn == NULL){
		pGroupTopicOn = (char *)pvPortMalloc(
				MQTTTopicHelper::lenGroupTopic(GROUP_NAME, ON_TOPIC)
				);
		if (pGroupTopicOn != NULL){
			MQTTTopicHelper::genGroupTopic(pGroupTopicOn, GROUP_NAME, ON_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}

	if (pGroupTopicOff == NULL){
		pGroupTopicOff = (char *)pvPortMalloc(
				MQTTTopicHelper::lenGroupTopic(GROUP_NAME, OFF_TOPIC)
				);
		if (pGroupTopicOff != NULL){
			MQTTTopicHelper::genGroupTopic(pGroupTopicOff, GROUP_NAME, OFF_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}

	if (pGroupTopicAlert == NULL){
		pGroupTopicAlert = (char *)pvPortMalloc(
				MQTTTopicHelper::lenGroupTopic(GROUP_ALL, ALERT_TOPIC)
				);
		if (pGroupTopicAlert != NULL){
			MQTTTopicHelper::genGroupTopic(pGroupTopicAlert, GROUP_ALL, ALERT_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}
}

/***
 * Use the interface to setup all the subscriptions
 * @param interface
 */
void MQTTRouterSaber::subscribe(MQTTInterface *interface){
	MQTTRouterTwin::subscribe(interface);

	interface->subToTopic(pGroupTopicOn, 1);
	interface->subToTopic(pGroupTopicOff, 1);
	interface->subToTopic(pGroupTopicAlert, 1);
}

/***
 * Route the message the appropriate part of the application
 * @param topic
 * @param topicLen
 * @param payload
 * @param payloadLen
 * @param interface
 */
void MQTTRouterSaber::route(const char *topic, size_t topicLen, const void * payload,
		size_t payloadLen, MQTTInterface *interface){
	MQTTRouterTwin::route(topic, topicLen, payload, payloadLen, interface);

	if (pTwin == NULL){
		LogError(("Twin not defined"));
		return;
	}


	if (strlen(pGroupTopicOn) == topicLen){
		if (memcmp(topic, pGroupTopicOn, topicLen)==0){
			if (!fromSelf(payload)){
				//Filter to do only in day mode
				SaberState * pSaber = (SaberState*) pTwin->getStateObject();
				if (pSaber->isDay()){
					LogDebug(("Blade ON"));
					pTwin->addMessage(STATE_ON, strlen(STATE_ON));
				} else {
					LogDebug(("Blade ON request filtered as night"));
				}
			}
		}
	}
	if (strlen(pGroupTopicOff) == topicLen){
		if (memcmp(topic, pGroupTopicOff, topicLen)==0){
			if (!fromSelf(payload)){
				LogDebug(("Blade OFF"));
				pTwin->addMessage(STATE_OFF,
						strlen(STATE_OFF)
						);
			}
		}
	}
	if (strlen(pGroupTopicAlert) == topicLen){
		if (memcmp(topic, pGroupTopicAlert, topicLen)==0){
			pTwin->addMessage((const char*)payload, payloadLen);
		}
	}
}

char* MQTTRouterSaber::getGroupTopicOff() const {
	return pGroupTopicOff;
}

char* MQTTRouterSaber::getGroupTopicOn() const {
	return pGroupTopicOn;
}


bool MQTTRouterSaber::fromSelf(const void * payload) {
	json_t jsonBuf[SABER_JSON_BUF];

	json_t const* json = json_create( ( char *)payload, jsonBuf, SABER_JSON_BUF );
	if ( !json ) {
		LogError( ("json create error") );
		return false;
	}

	json_t const* from = json_getProperty(json, "from");
	if (!from){
		return false;
	}

	if (json_getType(from) != JSON_TEXT){
		return false;
	}

	if ( strcmp(json_getValue(from), pInterface->getId()) == 0){
		return true;
	}

	return false;

}
