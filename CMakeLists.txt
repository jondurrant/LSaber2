cmake_minimum_required(VERSION 3.5)


set(NAME LSaber2) # 
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)


project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)


pico_sdk_init()

include("lwip_import.cmake")

include("json-maker.cmake")
include("tiny-json.cmake")

SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include(${FREERTOS_CONFIG_FILE_DIRECTORY}/freertos_config.cmake)
include("FreeRTOS_Kernel_import.cmake")

set(COREMQTT_PORT_PATH      ${CMAKE_CURRENT_LIST_DIR}/port/CoreMQTT)
set(CORE_MQTT_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/coreMQTT" CACHE STRING "Course Common Lib")
include("FreeRTOS-coreMQTT.cmake")

set(CORE_MQTT_AGENT_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/coreMQTT-Agent" CACHE STRING "Course Common Lib")
set(COREMQTT_AGENT_PORT_PATH      ${CMAKE_CURRENT_LIST_DIR}/port/CoreMQTT-Agent)
include("FreeRTOS-coreMQTT-Agent.cmake")



set(WOLFSSL_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/wolfssl" CACHE STRING "Course Common Lib")
include("wolfssl_import.cmake")

set(TWIN_THING_PICO_CONFIG_PATH ${CMAKE_CURRENT_LIST_DIR}/port/TwinThing)
include("lib/twinThingPicoW/twinThingPicoW.cmake")

include("lib/PicoLED/PicoLed.cmake")

include("lib/RtosComponents/RTOSComponents.cmake")

add_subdirectory(src)

#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
