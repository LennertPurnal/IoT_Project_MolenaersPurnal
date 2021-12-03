################################################
# Auto-generated as part of running getlibs 
################################################

# List of local libraries 
SEARCH_TARGET_CY8CPROTO-062-4343W=./libs/TARGET_CY8CPROTO-062-4343W

# List of shared libraries 
SEARCH_http-client=../mtb_shared/http-client/release-v1.1.1
SEARCH_mqtt=../mtb_shared/mqtt/release-v3.2.0
SEARCH_retarget-io=../mtb_shared/retarget-io/release-v1.3.0
SEARCH_wifi-connection-manager=../mtb_shared/wifi-connection-manager/release-v2.0.4
SEARCH_abstraction-rtos=../mtb_shared/abstraction-rtos/release-v1.5.0
SEARCH_aws-iot-device-sdk-embedded-C=../mtb_shared/aws-iot-device-sdk-embedded-C/202103.00
SEARCH_aws-iot-device-sdk-port=../mtb_shared/aws-iot-device-sdk-port/release-v2.0.0
SEARCH_capsense=../mtb_shared/capsense/release-v2.10.0
SEARCH_clib-support=../mtb_shared/clib-support/release-v1.1.0
SEARCH_connectivity-utilities=../mtb_shared/connectivity-utilities/release-v3.0.3
SEARCH_core-lib=../mtb_shared/core-lib/release-v1.3.0
SEARCH_core-make=../mtb_shared/core-make/release-v1.9.0
SEARCH_freertos=../mtb_shared/freertos/release-v10.4.3
SEARCH_lwip=../mtb_shared/lwip/STABLE-2_1_2_RELEASE
SEARCH_mbedtls=../mtb_shared/mbedtls/mbedtls-2.25.0
SEARCH_mtb-hal-cat1=../mtb_shared/mtb-hal-cat1/release-v1.6.0
SEARCH_mtb-pdl-cat1=../mtb_shared/mtb-pdl-cat1/release-v2.3.1
SEARCH_psoc6cm0p=../mtb_shared/psoc6cm0p/release-v2.1.4
SEARCH_recipe-make-cat1a=../mtb_shared/recipe-make-cat1a/release-v1.9.0
SEARCH_secure-sockets=../mtb_shared/secure-sockets/release-v2.3.0
SEARCH_whd-bsp-integration=../mtb_shared/whd-bsp-integration/release-v2.0.0
SEARCH_wifi-host-driver=../mtb_shared/wifi-host-driver/release-v2.0.0
SEARCH_wifi-mw-core=../mtb_shared/wifi-mw-core/release-v3.1.1

# Shared libraries added to build 
SEARCH+=$(SEARCH_http-client)
SEARCH+=$(SEARCH_mqtt)
SEARCH+=$(SEARCH_retarget-io)
SEARCH+=$(SEARCH_wifi-connection-manager)
SEARCH+=$(SEARCH_abstraction-rtos)
SEARCH+=$(SEARCH_aws-iot-device-sdk-embedded-C)
SEARCH+=$(SEARCH_aws-iot-device-sdk-port)
SEARCH+=$(SEARCH_capsense)
SEARCH+=$(SEARCH_clib-support)
SEARCH+=$(SEARCH_connectivity-utilities)
SEARCH+=$(SEARCH_core-lib)
SEARCH+=$(SEARCH_core-make)
SEARCH+=$(SEARCH_freertos)
SEARCH+=$(SEARCH_lwip)
SEARCH+=$(SEARCH_mbedtls)
SEARCH+=$(SEARCH_mtb-hal-cat1)
SEARCH+=$(SEARCH_mtb-pdl-cat1)
SEARCH+=$(SEARCH_psoc6cm0p)
SEARCH+=$(SEARCH_recipe-make-cat1a)
SEARCH+=$(SEARCH_secure-sockets)
SEARCH+=$(SEARCH_whd-bsp-integration)
SEARCH+=$(SEARCH_wifi-host-driver)
SEARCH+=$(SEARCH_wifi-mw-core)
-include $(CY_INTERNAL_APP_PATH)/importedbsp.mk
