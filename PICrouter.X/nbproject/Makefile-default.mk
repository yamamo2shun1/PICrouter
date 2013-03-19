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
MKDIR=mkdir -p
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

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED="${OBJECTDIR}/TCPIP Stack/ARP.o" "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o" "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o" "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o" "${OBJECTDIR}/TCPIP Stack/Helpers.o" "${OBJECTDIR}/TCPIP Stack/ICMP.o" "${OBJECTDIR}/TCPIP Stack/IP.o" "${OBJECTDIR}/TCPIP Stack/StackTsk.o" "${OBJECTDIR}/TCPIP Stack/Tick.o" "${OBJECTDIR}/TCPIP Stack/DHCP.o" "${OBJECTDIR}/TCPIP Stack/UDP.o" "${OBJECTDIR}/TCPIP Stack/NBNS.o" "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o" "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o" "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o" ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_hid.o ${OBJECTDIR}/USB/usb_host.o ${OBJECTDIR}/USB/usb_host_midi.o ${OBJECTDIR}/myDelay.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/picrouter.o ${OBJECTDIR}/button.o ${OBJECTDIR}/analog.o ${OBJECTDIR}/osc.o ${OBJECTDIR}/iosetting.o ${OBJECTDIR}/encoder.o ${OBJECTDIR}/usb_config.o "${OBJECTDIR}/TCPIP Stack/DHCPs.o"
POSSIBLE_DEPFILES="${OBJECTDIR}/TCPIP Stack/ARP.o.d" "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o.d" "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o.d" "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o.d" "${OBJECTDIR}/TCPIP Stack/Helpers.o.d" "${OBJECTDIR}/TCPIP Stack/ICMP.o.d" "${OBJECTDIR}/TCPIP Stack/IP.o.d" "${OBJECTDIR}/TCPIP Stack/StackTsk.o.d" "${OBJECTDIR}/TCPIP Stack/Tick.o.d" "${OBJECTDIR}/TCPIP Stack/DHCP.o.d" "${OBJECTDIR}/TCPIP Stack/UDP.o.d" "${OBJECTDIR}/TCPIP Stack/NBNS.o.d" "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o.d" "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o.d" "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o.d" ${OBJECTDIR}/USB/usb_device.o.d ${OBJECTDIR}/USB/usb_function_hid.o.d ${OBJECTDIR}/USB/usb_host.o.d ${OBJECTDIR}/USB/usb_host_midi.o.d ${OBJECTDIR}/myDelay.o.d ${OBJECTDIR}/usb_descriptors.o.d ${OBJECTDIR}/picrouter.o.d ${OBJECTDIR}/button.o.d ${OBJECTDIR}/analog.o.d ${OBJECTDIR}/osc.o.d ${OBJECTDIR}/iosetting.o.d ${OBJECTDIR}/encoder.o.d ${OBJECTDIR}/usb_config.o.d "${OBJECTDIR}/TCPIP Stack/DHCPs.o.d"

# Object Files
OBJECTFILES=${OBJECTDIR}/TCPIP\ Stack/ARP.o ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhy.o ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.o ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32IntMac.o ${OBJECTDIR}/TCPIP\ Stack/Helpers.o ${OBJECTDIR}/TCPIP\ Stack/ICMP.o ${OBJECTDIR}/TCPIP\ Stack/IP.o ${OBJECTDIR}/TCPIP\ Stack/StackTsk.o ${OBJECTDIR}/TCPIP\ Stack/Tick.o ${OBJECTDIR}/TCPIP\ Stack/DHCP.o ${OBJECTDIR}/TCPIP\ Stack/UDP.o ${OBJECTDIR}/TCPIP\ Stack/NBNS.o ${OBJECTDIR}/TCPIP\ Stack/ZeroconfHelper.o ${OBJECTDIR}/TCPIP\ Stack/ZeroconfLinkLocal.o ${OBJECTDIR}/TCPIP\ Stack/ZeroconfMulticastDNS.o ${OBJECTDIR}/USB/usb_device.o ${OBJECTDIR}/USB/usb_function_hid.o ${OBJECTDIR}/USB/usb_host.o ${OBJECTDIR}/USB/usb_host_midi.o ${OBJECTDIR}/myDelay.o ${OBJECTDIR}/usb_descriptors.o ${OBJECTDIR}/picrouter.o ${OBJECTDIR}/button.o ${OBJECTDIR}/analog.o ${OBJECTDIR}/osc.o ${OBJECTDIR}/iosetting.o ${OBJECTDIR}/encoder.o ${OBJECTDIR}/usb_config.o ${OBJECTDIR}/TCPIP\ Stack/DHCPs.o


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/TCPIP\ Stack/ARP.o: TCPIP\ Stack/ARP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ARP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ARP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ARP.o.d" -o "${OBJECTDIR}/TCPIP Stack/ARP.o" "TCPIP Stack/ARP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhy.o: TCPIP\ Stack/ETHPIC32ExtPhy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhy.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o" "TCPIP Stack/ETHPIC32ExtPhy.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.o: TCPIP\ Stack/ETHPIC32ExtPhyDP83848.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o" "TCPIP Stack/ETHPIC32ExtPhyDP83848.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32IntMac.o: TCPIP\ Stack/ETHPIC32IntMac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32IntMac.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o" "TCPIP Stack/ETHPIC32IntMac.c"   
	
${OBJECTDIR}/TCPIP\ Stack/Helpers.o: TCPIP\ Stack/Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/Helpers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/Helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/Helpers.o.d" -o "${OBJECTDIR}/TCPIP Stack/Helpers.o" "TCPIP Stack/Helpers.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ICMP.o: TCPIP\ Stack/ICMP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ICMP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ICMP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ICMP.o.d" -o "${OBJECTDIR}/TCPIP Stack/ICMP.o" "TCPIP Stack/ICMP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/IP.o: TCPIP\ Stack/IP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/IP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/IP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/IP.o.d" -o "${OBJECTDIR}/TCPIP Stack/IP.o" "TCPIP Stack/IP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/StackTsk.o: TCPIP\ Stack/StackTsk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/StackTsk.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/StackTsk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/StackTsk.o.d" -o "${OBJECTDIR}/TCPIP Stack/StackTsk.o" "TCPIP Stack/StackTsk.c"   
	
${OBJECTDIR}/TCPIP\ Stack/Tick.o: TCPIP\ Stack/Tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/Tick.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/Tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/Tick.o.d" -o "${OBJECTDIR}/TCPIP Stack/Tick.o" "TCPIP Stack/Tick.c"   
	
${OBJECTDIR}/TCPIP\ Stack/DHCP.o: TCPIP\ Stack/DHCP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/DHCP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/DHCP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/DHCP.o.d" -o "${OBJECTDIR}/TCPIP Stack/DHCP.o" "TCPIP Stack/DHCP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/UDP.o: TCPIP\ Stack/UDP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/UDP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/UDP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/UDP.o.d" -o "${OBJECTDIR}/TCPIP Stack/UDP.o" "TCPIP Stack/UDP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/NBNS.o: TCPIP\ Stack/NBNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/NBNS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/NBNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/NBNS.o.d" -o "${OBJECTDIR}/TCPIP Stack/NBNS.o" "TCPIP Stack/NBNS.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfHelper.o: TCPIP\ Stack/ZeroconfHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfHelper.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o" "TCPIP Stack/ZeroconfHelper.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfLinkLocal.o: TCPIP\ Stack/ZeroconfLinkLocal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfLinkLocal.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o" "TCPIP Stack/ZeroconfLinkLocal.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfMulticastDNS.o: TCPIP\ Stack/ZeroconfMulticastDNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfMulticastDNS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o" "TCPIP Stack/ZeroconfMulticastDNS.c"   
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/usb_device.o USB/usb_device.c   
	
${OBJECTDIR}/USB/usb_function_hid.o: USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_function_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/USB/usb_function_hid.o USB/usb_function_hid.c   
	
${OBJECTDIR}/USB/usb_host.o: USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_host.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_host.o.d" -o ${OBJECTDIR}/USB/usb_host.o USB/usb_host.c   
	
${OBJECTDIR}/USB/usb_host_midi.o: USB/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_host_midi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_host_midi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_host_midi.o.d" -o ${OBJECTDIR}/USB/usb_host_midi.o USB/usb_host_midi.c   
	
${OBJECTDIR}/myDelay.o: myDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/myDelay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/myDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/myDelay.o.d" -o ${OBJECTDIR}/myDelay.o myDelay.c   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c   
	
${OBJECTDIR}/picrouter.o: picrouter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/picrouter.o.d 
	@${FIXDEPS} "${OBJECTDIR}/picrouter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/picrouter.o.d" -o ${OBJECTDIR}/picrouter.o picrouter.c   
	
${OBJECTDIR}/button.o: button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/button.o.d 
	@${FIXDEPS} "${OBJECTDIR}/button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/button.o.d" -o ${OBJECTDIR}/button.o button.c   
	
${OBJECTDIR}/analog.o: analog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${FIXDEPS} "${OBJECTDIR}/analog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/analog.o.d" -o ${OBJECTDIR}/analog.o analog.c   
	
${OBJECTDIR}/osc.o: osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/osc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/osc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/osc.o.d" -o ${OBJECTDIR}/osc.o osc.c   
	
${OBJECTDIR}/iosetting.o: iosetting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/iosetting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/iosetting.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/iosetting.o.d" -o ${OBJECTDIR}/iosetting.o iosetting.c   
	
${OBJECTDIR}/encoder.o: encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/encoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/encoder.o.d" -o ${OBJECTDIR}/encoder.o encoder.c   
	
${OBJECTDIR}/usb_config.o: usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/usb_config.o.d 
	@${FIXDEPS} "${OBJECTDIR}/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/usb_config.o.d" -o ${OBJECTDIR}/usb_config.o usb_config.c   
	
${OBJECTDIR}/TCPIP\ Stack/DHCPs.o: TCPIP\ Stack/DHCPs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/DHCPs.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/DHCPs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/DHCPs.o.d" -o "${OBJECTDIR}/TCPIP Stack/DHCPs.o" "TCPIP Stack/DHCPs.c"   
	
else
${OBJECTDIR}/TCPIP\ Stack/ARP.o: TCPIP\ Stack/ARP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ARP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ARP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ARP.o.d" -o "${OBJECTDIR}/TCPIP Stack/ARP.o" "TCPIP Stack/ARP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhy.o: TCPIP\ Stack/ETHPIC32ExtPhy.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhy.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhy.o" "TCPIP Stack/ETHPIC32ExtPhy.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.o: TCPIP\ Stack/ETHPIC32ExtPhyDP83848.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32ExtPhyDP83848.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32ExtPhyDP83848.o" "TCPIP Stack/ETHPIC32ExtPhyDP83848.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ETHPIC32IntMac.o: TCPIP\ Stack/ETHPIC32IntMac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ETHPIC32IntMac.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o.d" -o "${OBJECTDIR}/TCPIP Stack/ETHPIC32IntMac.o" "TCPIP Stack/ETHPIC32IntMac.c"   
	
${OBJECTDIR}/TCPIP\ Stack/Helpers.o: TCPIP\ Stack/Helpers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/Helpers.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/Helpers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/Helpers.o.d" -o "${OBJECTDIR}/TCPIP Stack/Helpers.o" "TCPIP Stack/Helpers.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ICMP.o: TCPIP\ Stack/ICMP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ICMP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ICMP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ICMP.o.d" -o "${OBJECTDIR}/TCPIP Stack/ICMP.o" "TCPIP Stack/ICMP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/IP.o: TCPIP\ Stack/IP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/IP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/IP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/IP.o.d" -o "${OBJECTDIR}/TCPIP Stack/IP.o" "TCPIP Stack/IP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/StackTsk.o: TCPIP\ Stack/StackTsk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/StackTsk.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/StackTsk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/StackTsk.o.d" -o "${OBJECTDIR}/TCPIP Stack/StackTsk.o" "TCPIP Stack/StackTsk.c"   
	
${OBJECTDIR}/TCPIP\ Stack/Tick.o: TCPIP\ Stack/Tick.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/Tick.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/Tick.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/Tick.o.d" -o "${OBJECTDIR}/TCPIP Stack/Tick.o" "TCPIP Stack/Tick.c"   
	
${OBJECTDIR}/TCPIP\ Stack/DHCP.o: TCPIP\ Stack/DHCP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/DHCP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/DHCP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/DHCP.o.d" -o "${OBJECTDIR}/TCPIP Stack/DHCP.o" "TCPIP Stack/DHCP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/UDP.o: TCPIP\ Stack/UDP.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/UDP.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/UDP.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/UDP.o.d" -o "${OBJECTDIR}/TCPIP Stack/UDP.o" "TCPIP Stack/UDP.c"   
	
${OBJECTDIR}/TCPIP\ Stack/NBNS.o: TCPIP\ Stack/NBNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/NBNS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/NBNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/NBNS.o.d" -o "${OBJECTDIR}/TCPIP Stack/NBNS.o" "TCPIP Stack/NBNS.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfHelper.o: TCPIP\ Stack/ZeroconfHelper.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfHelper.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfHelper.o" "TCPIP Stack/ZeroconfHelper.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfLinkLocal.o: TCPIP\ Stack/ZeroconfLinkLocal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfLinkLocal.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfLinkLocal.o" "TCPIP Stack/ZeroconfLinkLocal.c"   
	
${OBJECTDIR}/TCPIP\ Stack/ZeroconfMulticastDNS.o: TCPIP\ Stack/ZeroconfMulticastDNS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/ZeroconfMulticastDNS.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o.d" -o "${OBJECTDIR}/TCPIP Stack/ZeroconfMulticastDNS.o" "TCPIP Stack/ZeroconfMulticastDNS.c"   
	
${OBJECTDIR}/USB/usb_device.o: USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_device.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_device.o.d" -o ${OBJECTDIR}/USB/usb_device.o USB/usb_device.c   
	
${OBJECTDIR}/USB/usb_function_hid.o: USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_function_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/USB/usb_function_hid.o USB/usb_function_hid.c   
	
${OBJECTDIR}/USB/usb_host.o: USB/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_host.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_host.o.d" -o ${OBJECTDIR}/USB/usb_host.o USB/usb_host.c   
	
${OBJECTDIR}/USB/usb_host_midi.o: USB/usb_host_midi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USB 
	@${RM} ${OBJECTDIR}/USB/usb_host_midi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/USB/usb_host_midi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/USB/usb_host_midi.o.d" -o ${OBJECTDIR}/USB/usb_host_midi.o USB/usb_host_midi.c   
	
${OBJECTDIR}/myDelay.o: myDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/myDelay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/myDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/myDelay.o.d" -o ${OBJECTDIR}/myDelay.o myDelay.c   
	
${OBJECTDIR}/usb_descriptors.o: usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/usb_descriptors.o.d 
	@${FIXDEPS} "${OBJECTDIR}/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/usb_descriptors.o.d" -o ${OBJECTDIR}/usb_descriptors.o usb_descriptors.c   
	
${OBJECTDIR}/picrouter.o: picrouter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/picrouter.o.d 
	@${FIXDEPS} "${OBJECTDIR}/picrouter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/picrouter.o.d" -o ${OBJECTDIR}/picrouter.o picrouter.c   
	
${OBJECTDIR}/button.o: button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/button.o.d 
	@${FIXDEPS} "${OBJECTDIR}/button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/button.o.d" -o ${OBJECTDIR}/button.o button.c   
	
${OBJECTDIR}/analog.o: analog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/analog.o.d 
	@${FIXDEPS} "${OBJECTDIR}/analog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/analog.o.d" -o ${OBJECTDIR}/analog.o analog.c   
	
${OBJECTDIR}/osc.o: osc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/osc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/osc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/osc.o.d" -o ${OBJECTDIR}/osc.o osc.c   
	
${OBJECTDIR}/iosetting.o: iosetting.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/iosetting.o.d 
	@${FIXDEPS} "${OBJECTDIR}/iosetting.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/iosetting.o.d" -o ${OBJECTDIR}/iosetting.o iosetting.c   
	
${OBJECTDIR}/encoder.o: encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/encoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/encoder.o.d" -o ${OBJECTDIR}/encoder.o encoder.c   
	
${OBJECTDIR}/usb_config.o: usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/usb_config.o.d 
	@${FIXDEPS} "${OBJECTDIR}/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/usb_config.o.d" -o ${OBJECTDIR}/usb_config.o usb_config.c   
	
${OBJECTDIR}/TCPIP\ Stack/DHCPs.o: TCPIP\ Stack/DHCPs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/TCPIP\ Stack 
	@${RM} ${OBJECTDIR}/TCPIP\ Stack/DHCPs.o.d 
	@${FIXDEPS} "${OBJECTDIR}/TCPIP Stack/DHCPs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./USB" -MMD -MF "${OBJECTDIR}/TCPIP Stack/DHCPs.o.d" -o "${OBJECTDIR}/TCPIP Stack/DHCPs.o" "TCPIP Stack/DHCPs.c"   
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=16000,--defsym=_min_stack_size=2048,--report-mem "-mperipheral-libs"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   app_32MX675F512H.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=16000,--defsym=_min_stack_size=2048,--report-mem "-mperipheral-libs"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
