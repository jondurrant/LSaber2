/*
 * MQTTRouterSaber.h
 *
 *  Created on: 20 Dec 2021
 *      Author: jondurrant
 */

#ifndef MQTTROUTERSABER_H_
#define MQTTROUTERSABER_H_

#include <MQTTRouterTwin.h>

#define GROUP_NAME 	"saber"
#define OFF_TOPIC  	"off"
#define ON_TOPIC	"on"
#define GROUP_ALL	"ALL"
#define ALERT_TOPIC "ALERT"

#define SABER_JSON_BUF 5

class MQTTRouterSaber : public MQTTRouterTwin {
public:
	MQTTRouterSaber();
	virtual ~MQTTRouterSaber();

	MQTTRouterSaber(const char * id, MQTTInterface *mi);

	/***
	 * Initialise the object give the Id and MQTT Interface
	 * @param id = string ID of the Thing
	 * @param mi = MQTT Interface
	 */
	virtual void init(const char * id, MQTTInterface *mi);

	/***
	 * Use the interface to setup all the subscriptions
	 * @param interface
	 */
	virtual void subscribe(MQTTInterface *interface);

	/***
	 * Route the message the appropriate part of the application
	 * @param topic
	 * @param topicLen
	 * @param payload
	 * @param payloadLen
	 * @param interface
	 */
	virtual void route(const char *topic, size_t topicLen, const void * payload,
			size_t payloadLen, MQTTInterface *interface);

	char* getGroupTopicOff() const ;

	char* getGroupTopicOn() const ;

private:
	char * pGroupTopicOn = NULL;
	char * pGroupTopicOff = NULL;
	char * pGroupTopicAlert = NULL;

	bool fromSelf(const void * payload);
};

#endif /* MQTTROUTERSABER_H_ */
