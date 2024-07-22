add_executable(${NAME}
        main.cpp
        StateExample.cpp
		ExampleAgentObserver.cpp
		RGBLEDMgr.cpp
		SaberState.cpp
		BladeRequest.cpp
		BladeColour.cpp
		BladeMgr.cpp
		BladeStateAgent.cpp
		BladeSeqInterface.cpp
		BladeSeqBinary.cpp
		BladeSeqNewHope.cpp
		BladeSeqAnim.cpp
		MQTTRouterSaber.cpp
		BladeSeqWand.cpp
		BladeSeqCandle.cpp
		BladeSeqSpark.cpp
		BladeSeqAlert.cpp
		BladeSeqBright.cpp
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	pico_stdlib 
	json_maker 
	tiny_json
	hardware_rtc
	RTOSComponents
	pico_multicore
	PicoLed
	FreeRTOS-Kernel-Heap4 
    freertos_config
    pico_cyw43_arch_lwip_sys_freertos
    twinThingPicoW
    LWIP_PORT
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})