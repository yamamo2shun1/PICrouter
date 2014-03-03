#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../tkrworks-libs/Common/analog.c ../tkrworks-libs/Common/button.c ../tkrworks-libs/Common/encoder.c ../tkrworks-libs/Common/iosetting.c ../tkrworks-libs/Common/lcd.c ../tkrworks-libs/Common/osc.c ../tkrworks-libs/Common/sram.c ../tkrworks-libs/Common/TimeDelay.c ../tkrworks-libs/Common/usb_config.c ../tkrworks-libs/Common/usb_descriptors.c ../tkrworks-libs/Common/usb_callback.c "../tkrworks-libs/TCPIP Stack/ARP.c" "../tkrworks-libs/TCPIP Stack/DHCP.c" "../tkrworks-libs/TCPIP Stack/DHCPs.c" "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhy.c" "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhyDP83848.c" "../tkrworks-libs/TCPIP Stack/ETHPIC32IntMac.c" "../tkrworks-libs/TCPIP Stack/Helpers.c" "../tkrworks-libs/TCPIP Stack/ICMP.c" "../tkrworks-libs/TCPIP Stack/IP.c" "../tkrworks-libs/TCPIP Stack/NBNS.c" "../tkrworks-libs/TCPIP Stack/StackTsk.c" "../tkrworks-libs/TCPIP Stack/Tick.c" "../tkrworks-libs/TCPIP Stack/UDP.c" "../tkrworks-libs/TCPIP Stack/ZeroconfHelper.c" "../tkrworks-libs/TCPIP Stack/ZeroconfLinkLocal.c" "../tkrworks-libs/TCPIP Stack/ZeroconfMulticastDNS.c" ../tkrworks-libs/USB/usb_device.c ../tkrworks-libs/USB/usb_function_hid.c ../tkrworks-libs/USB/usb_host.c ../tkrworks-libs/USB/usb_host_cdc.c ../tkrworks-libs/USB/usb_host_cdc_interface.c ../tkrworks-libs/USB/usb_host_hid.c ../tkrworks-libs/USB/usb_host_hid_parser.c ../tkrworks-libs/USB/usb_host_midi.c picrouter.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/454704213/analog.o ${OBJECTDIR}/_ext/454704213/button.o ${OBJECTDIR}/_ext/454704213/encoder.o ${OBJECTDIR}/_ext/454704213/iosetting.o ${OBJECTDIR}/_ext/454704213/lcd.o ${OBJECTDIR}/_ext/454704213/osc.o ${OBJECTDIR}/_ext/454704213/sram.o ${OBJECTDIR}/_ext/454704213/TimeDelay.o ${OBJECTDIR}/_ext/454704213/usb_config.o ${OBJECTDIR}/_ext/454704213/usb_descriptors.o ${OBJECTDIR}/_ext/454704213/usb_callback.o ${OBJECTDIR}/_ext/1340694758/ARP.o ${OBJECTDIR}/_ext/1340694758/DHCP.o ${OBJECTDIR}/_ext/1340694758/DHCPs.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o ${OBJECTDIR}/_ext/1340694758/Helpers.o ${OBJECTDIR}/_ext/1340694758/ICMP.o ${OBJECTDIR}/_ext/1340694758/IP.o ${OBJECTDIR}/_ext/1340694758/NBNS.o ${OBJECTDIR}/_ext/1340694758/StackTsk.o ${OBJECTDIR}/_ext/1340694758/Tick.o ${OBJECTDIR}/_ext/1340694758/UDP.o ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o ${OBJECTDIR}/_ext/1798056102/usb_device.o ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o ${OBJECTDIR}/_ext/1798056102/usb_host.o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o ${OBJECTDIR}/picrouter.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/454704213/analog.o.d ${OBJECTDIR}/_ext/454704213/button.o.d ${OBJECTDIR}/_ext/454704213/encoder.o.d ${OBJECTDIR}/_ext/454704213/iosetting.o.d ${OBJECTDIR}/_ext/454704213/lcd.o.d ${OBJECTDIR}/_ext/454704213/osc.o.d ${OBJECTDIR}/_ext/454704213/sram.o.d ${OBJECTDIR}/_ext/454704213/TimeDelay.o.d ${OBJECTDIR}/_ext/454704213/usb_config.o.d ${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d ${OBJECTDIR}/_ext/454704213/usb_callback.o.d ${OBJECTDIR}/_ext/1340694758/ARP.o.d ${OBJECTDIR}/_ext/1340694758/DHCP.o.d ${OBJECTDIR}/_ext/1340694758/DHCPs.o.d ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d ${OBJECTDIR}/_ext/1340694758/Helpers.o.d ${OBJECTDIR}/_ext/1340694758/ICMP.o.d ${OBJECTDIR}/_ext/1340694758/IP.o.d ${OBJECTDIR}/_ext/1340694758/NBNS.o.d ${OBJECTDIR}/_ext/1340694758/StackTsk.o.d ${OBJECTDIR}/_ext/1340694758/Tick.o.d ${OBJECTDIR}/_ext/1340694758/UDP.o.d ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d ${OBJECTDIR}/_ext/1798056102/usb_device.o.d ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d ${OBJECTDIR}/_ext/1798056102/usb_host.o.d ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d ${OBJECTDIR}/picrouter.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/454704213/analog.o ${OBJECTDIR}/_ext/454704213/button.o ${OBJECTDIR}/_ext/454704213/encoder.o ${OBJECTDIR}/_ext/454704213/iosetting.o ${OBJECTDIR}/_ext/454704213/lcd.o ${OBJECTDIR}/_ext/454704213/osc.o ${OBJECTDIR}/_ext/454704213/sram.o ${OBJECTDIR}/_ext/454704213/TimeDelay.o ${OBJECTDIR}/_ext/454704213/usb_config.o ${OBJECTDIR}/_ext/454704213/usb_descriptors.o ${OBJECTDIR}/_ext/454704213/usb_callback.o ${OBJECTDIR}/_ext/1340694758/ARP.o ${OBJECTDIR}/_ext/1340694758/DHCP.o ${OBJECTDIR}/_ext/1340694758/DHCPs.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o ${OBJECTDIR}/_ext/1340694758/Helpers.o ${OBJECTDIR}/_ext/1340694758/ICMP.o ${OBJECTDIR}/_ext/1340694758/IP.o ${OBJECTDIR}/_ext/1340694758/NBNS.o ${OBJECTDIR}/_ext/1340694758/StackTsk.o ${OBJECTDIR}/_ext/1340694758/Tick.o ${OBJECTDIR}/_ext/1340694758/UDP.o ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o ${OBJECTDIR}/_ext/1798056102/usb_device.o ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o ${OBJECTDIR}/_ext/1798056102/usb_host.o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o ${OBJECTDIR}/picrouter.o

# Source Files
SOURCEFILES=../tkrworks-libs/Common/analog.c ../tkrworks-libs/Common/button.c ../tkrworks-libs/Common/encoder.c ../tkrworks-libs/Common/iosetting.c ../tkrworks-libs/Common/lcd.c ../tkrworks-libs/Common/osc.c ../tkrworks-libs/Common/sram.c ../tkrworks-libs/Common/TimeDelay.c ../tkrworks-libs/Common/usb_config.c ../tkrworks-libs/Common/usb_descriptors.c ../tkrworks-libs/Common/usb_callback.c ../tkrworks-libs/TCPIP Stack/ARP.c ../tkrworks-libs/TCPIP Stack/DHCP.c ../tkrworks-libs/TCPIP Stack/DHCPs.c ../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhy.c ../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhyDP83848.c ../tkrworks-libs/TCPIP Stack/ETHPIC32IntMac.c ../tkrworks-libs/TCPIP Stack/Helpers.c ../tkrworks-libs/TCPIP Stack/ICMP.c ../tkrworks-libs/TCPIP Stack/IP.c ../tkrworks-libs/TCPIP Stack/NBNS.c ../tkrworks-libs/TCPIP Stack/StackTsk.c ../tkrworks-libs/TCPIP Stack/Tick.c ../tkrworks-libs/TCPIP Stack/UDP.c ../tkrworks-libs/TCPIP Stack/ZeroconfHelper.c ../tkrworks-libs/TCPIP Stack/ZeroconfLinkLocal.c ../tkrworks-libs/TCPIP Stack/ZeroconfMulticastDNS.c ../tkrworks-libs/USB/usb_device.c ../tkrworks-libs/USB/usb_function_hid.c ../tkrworks-libs/USB/usb_host.c ../tkrworks-libs/USB/usb_host_cdc.c ../tkrworks-libs/USB/usb_host_cdc_interface.c ../tkrworks-libs/USB/usb_host_hid.c ../tkrworks-libs/USB/usb_host_hid_parser.c ../tkrworks-libs/USB/usb_host_midi.c picrouter.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX675F512H
MP_LINKER_FILE_OPTION=,--script="app_32MX675F512H.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/454704213/analog.o: ../tkrworks-libs/Common/analog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/analog.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/analog.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/analog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/analog.o.d" -o ${OBJECTDIR}/_ext/454704213/analog.o ../tkrworks-libs/Common/analog.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/button.o: ../tkrworks-libs/Common/button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/button.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/button.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/button.o.d" -o ${OBJECTDIR}/_ext/454704213/button.o ../tkrworks-libs/Common/button.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/encoder.o: ../tkrworks-libs/Common/encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/encoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/encoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/encoder.o.d" -o ${OBJECTDIR}/_ext/454704213/encoder.o ../tkrworks-libs/Common/encoder.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/iosetting.o: ../tkrworks-libs/Common/iosetting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/iosetting.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/iosetting.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/iosetting.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/iosetting.o.d" -o ${OBJECTDIR}/_ext/454704213/iosetting.o ../tkrworks-libs/Common/iosetting.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/lcd.o: ../tkrworks-libs/Common/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/lcd.o.d" -o ${OBJECTDIR}/_ext/454704213/lcd.o ../tkrworks-libs/Common/lcd.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/osc.o: ../tkrworks-libs/Common/osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/osc.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/osc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/osc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/osc.o.d" -o ${OBJECTDIR}/_ext/454704213/osc.o ../tkrworks-libs/Common/osc.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/sram.o: ../tkrworks-libs/Common/sram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/sram.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/sram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/sram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/sram.o.d" -o ${OBJECTDIR}/_ext/454704213/sram.o ../tkrworks-libs/Common/sram.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/TimeDelay.o: ../tkrworks-libs/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/TimeDelay.o.d" -o ${OBJECTDIR}/_ext/454704213/TimeDelay.o ../tkrworks-libs/Common/TimeDelay.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_config.o: ../tkrworks-libs/Common/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_config.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_config.o ../tkrworks-libs/Common/usb_config.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_descriptors.o: ../tkrworks-libs/Common/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_descriptors.o ../tkrworks-libs/Common/usb_descriptors.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_callback.o: ../tkrworks-libs/Common/usb_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_callback.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_callback.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_callback.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_callback.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_callback.o ../tkrworks-libs/Common/usb_callback.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ARP.o: ../tkrworks-libs/TCPIP\ Stack/ARP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ARP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ARP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ARP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ARP.o.d" -o ${OBJECTDIR}/_ext/1340694758/ARP.o "../tkrworks-libs/TCPIP Stack/ARP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/DHCP.o: ../tkrworks-libs/TCPIP\ Stack/DHCP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/DHCP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/DHCP.o.d" -o ${OBJECTDIR}/_ext/1340694758/DHCP.o "../tkrworks-libs/TCPIP Stack/DHCP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/DHCPs.o: ../tkrworks-libs/TCPIP\ Stack/DHCPs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCPs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCPs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/DHCPs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/DHCPs.o.d" -o ${OBJECTDIR}/_ext/1340694758/DHCPs.o "../tkrworks-libs/TCPIP Stack/DHCPs.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32ExtPhy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhy.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhyDP83848.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32IntMac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o "../tkrworks-libs/TCPIP Stack/ETHPIC32IntMac.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/Helpers.o: ../tkrworks-libs/TCPIP\ Stack/Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Helpers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Helpers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/Helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/Helpers.o.d" -o ${OBJECTDIR}/_ext/1340694758/Helpers.o "../tkrworks-libs/TCPIP Stack/Helpers.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ICMP.o: ../tkrworks-libs/TCPIP\ Stack/ICMP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ICMP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ICMP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ICMP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ICMP.o.d" -o ${OBJECTDIR}/_ext/1340694758/ICMP.o "../tkrworks-libs/TCPIP Stack/ICMP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/IP.o: ../tkrworks-libs/TCPIP\ Stack/IP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/IP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/IP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/IP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/IP.o.d" -o ${OBJECTDIR}/_ext/1340694758/IP.o "../tkrworks-libs/TCPIP Stack/IP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/NBNS.o: ../tkrworks-libs/TCPIP\ Stack/NBNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/NBNS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/NBNS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/NBNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/NBNS.o.d" -o ${OBJECTDIR}/_ext/1340694758/NBNS.o "../tkrworks-libs/TCPIP Stack/NBNS.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/StackTsk.o: ../tkrworks-libs/TCPIP\ Stack/StackTsk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/StackTsk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/StackTsk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/StackTsk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/StackTsk.o.d" -o ${OBJECTDIR}/_ext/1340694758/StackTsk.o "../tkrworks-libs/TCPIP Stack/StackTsk.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/Tick.o: ../tkrworks-libs/TCPIP\ Stack/Tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Tick.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Tick.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/Tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/Tick.o.d" -o ${OBJECTDIR}/_ext/1340694758/Tick.o "../tkrworks-libs/TCPIP Stack/Tick.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/UDP.o: ../tkrworks-libs/TCPIP\ Stack/UDP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/UDP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/UDP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/UDP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/UDP.o.d" -o ${OBJECTDIR}/_ext/1340694758/UDP.o "../tkrworks-libs/TCPIP Stack/UDP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o "../tkrworks-libs/TCPIP Stack/ZeroconfHelper.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfLinkLocal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o "../tkrworks-libs/TCPIP Stack/ZeroconfLinkLocal.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfMulticastDNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o "../tkrworks-libs/TCPIP Stack/ZeroconfMulticastDNS.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_device.o: ../tkrworks-libs/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_device.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_device.o ../tkrworks-libs/USB/usb_device.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_function_hid.o: ../tkrworks-libs/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o ../tkrworks-libs/USB/usb_function_hid.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host.o: ../tkrworks-libs/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host.o ../tkrworks-libs/USB/usb_host.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o: ../tkrworks-libs/USB/usb_host_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o ../tkrworks-libs/USB/usb_host_cdc.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o: ../tkrworks-libs/USB/usb_host_cdc_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o ../tkrworks-libs/USB/usb_host_cdc_interface.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_hid.o: ../tkrworks-libs/USB/usb_host_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o ../tkrworks-libs/USB/usb_host_hid.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o: ../tkrworks-libs/USB/usb_host_hid_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o ../tkrworks-libs/USB/usb_host_hid_parser.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_midi.o: ../tkrworks-libs/USB/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o ../tkrworks-libs/USB/usb_host_midi.c   -save-temps=obj
	
${OBJECTDIR}/picrouter.o: picrouter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/picrouter.o.d 
	@${RM} ${OBJECTDIR}/picrouter.o 
	@${FIXDEPS} "${OBJECTDIR}/picrouter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/picrouter.o.d" -o ${OBJECTDIR}/picrouter.o picrouter.c   -save-temps=obj
	
else
${OBJECTDIR}/_ext/454704213/analog.o: ../tkrworks-libs/Common/analog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/analog.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/analog.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/analog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/analog.o.d" -o ${OBJECTDIR}/_ext/454704213/analog.o ../tkrworks-libs/Common/analog.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/button.o: ../tkrworks-libs/Common/button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/button.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/button.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/button.o.d" -o ${OBJECTDIR}/_ext/454704213/button.o ../tkrworks-libs/Common/button.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/encoder.o: ../tkrworks-libs/Common/encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/encoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/encoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/encoder.o.d" -o ${OBJECTDIR}/_ext/454704213/encoder.o ../tkrworks-libs/Common/encoder.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/iosetting.o: ../tkrworks-libs/Common/iosetting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/iosetting.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/iosetting.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/iosetting.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/iosetting.o.d" -o ${OBJECTDIR}/_ext/454704213/iosetting.o ../tkrworks-libs/Common/iosetting.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/lcd.o: ../tkrworks-libs/Common/lcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/lcd.o.d" -o ${OBJECTDIR}/_ext/454704213/lcd.o ../tkrworks-libs/Common/lcd.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/osc.o: ../tkrworks-libs/Common/osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/osc.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/osc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/osc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/osc.o.d" -o ${OBJECTDIR}/_ext/454704213/osc.o ../tkrworks-libs/Common/osc.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/sram.o: ../tkrworks-libs/Common/sram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/sram.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/sram.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/sram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/sram.o.d" -o ${OBJECTDIR}/_ext/454704213/sram.o ../tkrworks-libs/Common/sram.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/TimeDelay.o: ../tkrworks-libs/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/TimeDelay.o.d" -o ${OBJECTDIR}/_ext/454704213/TimeDelay.o ../tkrworks-libs/Common/TimeDelay.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_config.o: ../tkrworks-libs/Common/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_config.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_config.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_config.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_config.o ../tkrworks-libs/Common/usb_config.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_descriptors.o: ../tkrworks-libs/Common/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_descriptors.o ../tkrworks-libs/Common/usb_descriptors.c   -save-temps=obj
	
${OBJECTDIR}/_ext/454704213/usb_callback.o: ../tkrworks-libs/Common/usb_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/454704213 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_callback.o.d 
	@${RM} ${OBJECTDIR}/_ext/454704213/usb_callback.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/454704213/usb_callback.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/454704213/usb_callback.o.d" -o ${OBJECTDIR}/_ext/454704213/usb_callback.o ../tkrworks-libs/Common/usb_callback.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ARP.o: ../tkrworks-libs/TCPIP\ Stack/ARP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ARP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ARP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ARP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ARP.o.d" -o ${OBJECTDIR}/_ext/1340694758/ARP.o "../tkrworks-libs/TCPIP Stack/ARP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/DHCP.o: ../tkrworks-libs/TCPIP\ Stack/DHCP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/DHCP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/DHCP.o.d" -o ${OBJECTDIR}/_ext/1340694758/DHCP.o "../tkrworks-libs/TCPIP Stack/DHCP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/DHCPs.o: ../tkrworks-libs/TCPIP\ Stack/DHCPs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCPs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/DHCPs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/DHCPs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/DHCPs.o.d" -o ${OBJECTDIR}/_ext/1340694758/DHCPs.o "../tkrworks-libs/TCPIP Stack/DHCPs.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32ExtPhy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhy.o "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhy.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32ExtPhyDP83848.o "../tkrworks-libs/TCPIP Stack/ETHPIC32ExtPhyDP83848.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o: ../tkrworks-libs/TCPIP\ Stack/ETHPIC32IntMac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o.d" -o ${OBJECTDIR}/_ext/1340694758/ETHPIC32IntMac.o "../tkrworks-libs/TCPIP Stack/ETHPIC32IntMac.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/Helpers.o: ../tkrworks-libs/TCPIP\ Stack/Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Helpers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Helpers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/Helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/Helpers.o.d" -o ${OBJECTDIR}/_ext/1340694758/Helpers.o "../tkrworks-libs/TCPIP Stack/Helpers.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ICMP.o: ../tkrworks-libs/TCPIP\ Stack/ICMP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ICMP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ICMP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ICMP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ICMP.o.d" -o ${OBJECTDIR}/_ext/1340694758/ICMP.o "../tkrworks-libs/TCPIP Stack/ICMP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/IP.o: ../tkrworks-libs/TCPIP\ Stack/IP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/IP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/IP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/IP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/IP.o.d" -o ${OBJECTDIR}/_ext/1340694758/IP.o "../tkrworks-libs/TCPIP Stack/IP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/NBNS.o: ../tkrworks-libs/TCPIP\ Stack/NBNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/NBNS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/NBNS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/NBNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/NBNS.o.d" -o ${OBJECTDIR}/_ext/1340694758/NBNS.o "../tkrworks-libs/TCPIP Stack/NBNS.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/StackTsk.o: ../tkrworks-libs/TCPIP\ Stack/StackTsk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/StackTsk.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/StackTsk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/StackTsk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/StackTsk.o.d" -o ${OBJECTDIR}/_ext/1340694758/StackTsk.o "../tkrworks-libs/TCPIP Stack/StackTsk.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/Tick.o: ../tkrworks-libs/TCPIP\ Stack/Tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Tick.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/Tick.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/Tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/Tick.o.d" -o ${OBJECTDIR}/_ext/1340694758/Tick.o "../tkrworks-libs/TCPIP Stack/Tick.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/UDP.o: ../tkrworks-libs/TCPIP\ Stack/UDP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/UDP.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/UDP.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/UDP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/UDP.o.d" -o ${OBJECTDIR}/_ext/1340694758/UDP.o "../tkrworks-libs/TCPIP Stack/UDP.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfHelper.o "../tkrworks-libs/TCPIP Stack/ZeroconfHelper.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfLinkLocal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfLinkLocal.o "../tkrworks-libs/TCPIP Stack/ZeroconfLinkLocal.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o: ../tkrworks-libs/TCPIP\ Stack/ZeroconfMulticastDNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1340694758 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o.d" -o ${OBJECTDIR}/_ext/1340694758/ZeroconfMulticastDNS.o "../tkrworks-libs/TCPIP Stack/ZeroconfMulticastDNS.c"   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_device.o: ../tkrworks-libs/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_device.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_device.o ../tkrworks-libs/USB/usb_device.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_function_hid.o: ../tkrworks-libs/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_function_hid.o ../tkrworks-libs/USB/usb_function_hid.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host.o: ../tkrworks-libs/USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host.o ../tkrworks-libs/USB/usb_host.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o: ../tkrworks-libs/USB/usb_host_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc.o ../tkrworks-libs/USB/usb_host_cdc.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o: ../tkrworks-libs/USB/usb_host_cdc_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_cdc_interface.o ../tkrworks-libs/USB/usb_host_cdc_interface.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_hid.o: ../tkrworks-libs/USB/usb_host_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_hid.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_hid.o ../tkrworks-libs/USB/usb_host_hid.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o: ../tkrworks-libs/USB/usb_host_hid_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_hid_parser.o ../tkrworks-libs/USB/usb_host_hid_parser.c   -save-temps=obj
	
${OBJECTDIR}/_ext/1798056102/usb_host_midi.o: ../tkrworks-libs/USB/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1798056102 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/_ext/1798056102/usb_host_midi.o.d" -o ${OBJECTDIR}/_ext/1798056102/usb_host_midi.o ../tkrworks-libs/USB/usb_host_midi.c   -save-temps=obj
	
${OBJECTDIR}/picrouter.o: picrouter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/picrouter.o.d 
	@${RM} ${OBJECTDIR}/picrouter.o 
	@${FIXDEPS} "${OBJECTDIR}/picrouter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -ffunction-sections -fdata-sections -mips16 -O3 -I"." -I"../tkrworks-libs/Include" -I"../tkrworks-libs/USB" -MMD -MF "${OBJECTDIR}/picrouter.o.d" -o ${OBJECTDIR}/picrouter.o picrouter.c   -save-temps=obj
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    app_32MX675F512H.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)    -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -mips16 -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -save-temps=obj -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=_min_heap_size=16000,--defsym=_min_stack_size=2048,--gc-sections,--report-mem "-mperipheral-libs"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   app_32MX675F512H.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O3 -mips16 -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -save-temps=obj -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=16000,--defsym=_min_stack_size=2048,--gc-sections,--report-mem "-mperipheral-libs"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
