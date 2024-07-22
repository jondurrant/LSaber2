#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <array>
#include <stdint.h>


#include "pico/multicore.h"

#include <PicoLed.hpp>




//Local include file not in git repository, defining the credentials
#include "Credentials.env"
#ifndef SID
#define SID "sid"
#define PASSWD "passwd"
#endif
#ifndef MQTTHOST
#define MQTTHOST "MQTTSERVER"
#define MQTTPORT 1883
#define MQTTUSER "USER"
#define MQTTPASSWD "PASSWD"
#endif


#include "MQTTAgent.h"
#include "MQTTRouterPing.h"
#include "MQTTRouterTwin.h"

#include <WifiHelper.h>

#include "StateExample.h"
#include "ExampleAgentObserver.h"
#include "RGBLEDMgr.h"
#include "RGBLEDAgent.h"
#include "SaberState.h"
#include "BladeMgr.h"
#include "BladeStateAgent.h"
#include "MQTTRouterSaber.h"


void core1_entry() {
	BladeMgr blade;
	blade.loopForever();

}

void runTimeStats( ){
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime, ulStatsAsPercentage;

   /* Take a snapshot of the number of tasks in case it changes while this
   function is executing. */
   uxArraySize = uxTaskGetNumberOfTasks();

   /* Allocate a TaskStatus_t structure for each task.  An array could be
   allocated statically at compile time. */
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
      /* Generate raw status information about each task. */
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );

      /* For percentage calculations. */
      ulTotalRunTime /= 100UL;

	 /* For each populated position in the pxTaskStatusArray array,
	 format the raw data as human readable ASCII data. */
	 for( x = 0; x < uxArraySize; x++ )
	 {
		/* What percentage of the total run time has the task used?
		This will always be rounded down to the nearest integer.
		ulTotalRunTimeDiv100 has already been divided by 100. */
		if( ulTotalRunTime > 0 ) {
			ulStatsAsPercentage =
				  pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;
		} else {
			ulStatsAsPercentage = 0;
		}

		if( ulStatsAsPercentage > 0UL )
		{
		   printf("%s tt:%l utt:%l u%%r\n",
							 pxTaskStatusArray[ x ].pcTaskName,
							 pxTaskStatusArray[ x ].ulRunTimeCounter,
							 ulStatsAsPercentage );
		}
		else
		{
		   /* If the percentage is zero here then the task has
		   consumed less than 1% of the total run time. */
		   printf("%s tt:%l utt<1%% bp:%d cp:%d state:%d\n",
							 pxTaskStatusArray[ x ].pcTaskName,
							 pxTaskStatusArray[ x ].ulRunTimeCounter,
							 pxTaskStatusArray[ x ].uxBasePriority,
							 pxTaskStatusArray[ x ].uxCurrentPriority,
							 pxTaskStatusArray[ x ].eCurrentState
							 );
		}
	 }

      /* The array is no longer needed, free the memory it consumes. */
      vPortFree( pxTaskStatusArray );
   }else {
 	  printf("Malloc failed\n");
   }
}




void
init_thread(void* pvParameters) {
	char mqttTarget[] = MQTTHOST;
	lwesp_port_t mqttPort = MQTTPORT;
	char mqttUser[] = MQTTUSER;
	char * pMqttUser = mqttUser;
	char mqttPwd[] = MQTTPASSWD;
	char * pMqttPwd = mqttPwd;
	char macs[15];
	bool ssl= false;


	//WatchdogBlinkAgent watchdog;
	//watchdog.start(tskIDLE_PRIORITY+1);

	MQTTRouterSaber mqttRouter;
	SaberState state;

	MQTTAgent mqttAgent(512, 512);
	BladeStateAgent xTwin;
	MQTTPingTask xPing;

	RGBLEDAgent xLed(3, 4, 5);
	xLed.start(tskIDLE_PRIORITY+1);
	xLed.set(RGBModeOn, 0xFF, 0xFF, 0xFF);
	RGBLEDMgr xLedMgr(&xLed);

	//Connect to WiFi
	if (WifiHelper::connectToAp(SID, PASSWD)){
		char ips[16];
		WifiHelper::getIPAddressStr(ips);
		printf("WIFI IP %s\n", ips);
		if (WifiHelper::getMACAddressStr(macs)){
			printf("MAC %s\n", macs);
			if (strcmp(mqttUser, "MAC") == 0){
				pMqttUser = macs;
			}
			if (strcmp(mqttPwd, "MAC") == 0){
				pMqttPwd = macs;
			}
		} else {
			printf("MAC NOT AVAILABLE\n");
		}
		WifiHelper::syncRTCwithSNTP();


		//Set up the credentials so we have an ID for our thing
		mqttAgent.credentials(pMqttUser, pMqttPwd);
		mqttRouter.init(mqttAgent.getId(), &mqttAgent);

		//Twin agent to manage the state
		xTwin.setStateObject(&state);
		xTwin.setMQTTInterface(&mqttAgent);
		xTwin.start(tskIDLE_PRIORITY+1);
		xTwin.setTopics(mqttRouter.getGroupTopicOn(), mqttRouter.getGroupTopicOff());

		//Start up a Ping agent to mange ping requests
		xPing.setInterface(&mqttAgent);
		xPing.start(tskIDLE_PRIORITY+1);

		//Give the router the twin and ping agents
		mqttRouter.setTwin(&xTwin);
		mqttRouter.setPingTask(&xPing);

		//Setup and start the mqttAgent
		mqttAgent.setObserver(&xLedMgr);
		mqttAgent.setRouter(&mqttRouter);

#ifdef MQTTSSL
		ssl = true;
#endif
		mqttAgent.connect(mqttTarget, mqttPort, true, ssl);

		mqttAgent.start(tskIDLE_PRIORITY+1);

	} else {
		xLed.set(RGBModeSlow, 0xFF, 0xff, 0xff);
	}

	//xLed.start(tskIDLE_PRIORITY);

    for (;;){
    	vTaskDelay(5000);

    	if (! WifiHelper::isJoined()){
    		while (! WifiHelper::connectToAp(SID, PASSWD)){
    			vTaskDelay(5000);
    		}
    		char ips[16];
			WifiHelper::getIPAddressStr(ips);
			printf("WIFI IP %s\n", ips);
    	}

    	//runTimeStats();
    }
}



int main() {

	stdio_init_all();

	sleep_ms(3000);
	printf("******GO******\n");

	multicore_reset_core1 ();
	multicore_launch_core1(core1_entry);


    BaseType_t atReturned;
	TaskHandle_t atHandle = NULL;

	atReturned = xTaskCreate(
					init_thread,
					"Init task",
					512,
					( void * ) 1,
					tskIDLE_PRIORITY+1,
					&atHandle );




    vTaskStartScheduler();
    while(1)
    {
        configASSERT(0);    /* We should never get here */
    }

}
