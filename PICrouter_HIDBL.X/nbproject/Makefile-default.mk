#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Source/FrameWork/Framework.o ${OBJECTDIR}/Source/USB/usb_descriptors.o ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o ${OBJECTDIR}/Source/USB/usb_device.o ${OBJECTDIR}/Source/USB/usb_function_hid.o ${OBJECTDIR}/Source/BootLoader.o ${OBJECTDIR}/Source/NVMem.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Source/FrameWork/Framework.o.d ${OBJECTDIR}/Source/USB/usb_descriptors.o.d ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d ${OBJECTDIR}/Source/USB/usb_device.o.d ${OBJECTDIR}/Source/USB/usb_function_hid.o.d ${OBJECTDIR}/Source/BootLoader.o.d ${OBJECTDIR}/Source/NVMem.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Source/FrameWork/Framework.o ${OBJECTDIR}/Source/USB/usb_descriptors.o ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o ${OBJECTDIR}/Source/USB/usb_device.o ${OBJECTDIR}/Source/USB/usb_function_hid.o ${OBJECTDIR}/Source/BootLoader.o ${OBJECTDIR}/Source/NVMem.o


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX675F512H
MP_LINKER_FILE_OPTION=,--script="linker_scripts/usb_hid_btl_sk.ld"
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
${OBJECTDIR}/Source/FrameWork/Framework.o: Source/FrameWork/Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/FrameWork 
	@${RM} ${OBJECTDIR}/Source/FrameWork/Framework.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/FrameWork/Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/FrameWork/Framework.o.d" -o ${OBJECTDIR}/Source/FrameWork/Framework.o Source/FrameWork/Framework.c   
	
${OBJECTDIR}/Source/USB/usb_descriptors.o: Source/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_descriptors.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/Source/USB/usb_descriptors.o Source/USB/usb_descriptors.c   
	
${OBJECTDIR}/Source/USB/Usb_HID_tasks.o: Source/USB/Usb_HID_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d" -o ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o Source/USB/Usb_HID_tasks.c   
	
${OBJECTDIR}/Source/USB/usb_device.o: Source/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_device.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_device.o.d" -o ${OBJECTDIR}/Source/USB/usb_device.o Source/USB/usb_device.c   
	
${OBJECTDIR}/Source/USB/usb_function_hid.o: Source/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_function_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Source/USB/usb_function_hid.o Source/USB/usb_function_hid.c   
	
${OBJECTDIR}/Source/BootLoader.o: Source/BootLoader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source 
	@${RM} ${OBJECTDIR}/Source/BootLoader.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/BootLoader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/BootLoader.o.d" -o ${OBJECTDIR}/Source/BootLoader.o Source/BootLoader.c   
	
${OBJECTDIR}/Source/NVMem.o: Source/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source 
	@${RM} ${OBJECTDIR}/Source/NVMem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/NVMem.o.d" -o ${OBJECTDIR}/Source/NVMem.o Source/NVMem.c   
	
else
${OBJECTDIR}/Source/FrameWork/Framework.o: Source/FrameWork/Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/FrameWork 
	@${RM} ${OBJECTDIR}/Source/FrameWork/Framework.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/FrameWork/Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/FrameWork/Framework.o.d" -o ${OBJECTDIR}/Source/FrameWork/Framework.o Source/FrameWork/Framework.c   
	
${OBJECTDIR}/Source/USB/usb_descriptors.o: Source/USB/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_descriptors.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_descriptors.o.d" -o ${OBJECTDIR}/Source/USB/usb_descriptors.o Source/USB/usb_descriptors.c   
	
${OBJECTDIR}/Source/USB/Usb_HID_tasks.o: Source/USB/Usb_HID_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/Usb_HID_tasks.o.d" -o ${OBJECTDIR}/Source/USB/Usb_HID_tasks.o Source/USB/Usb_HID_tasks.c   
	
${OBJECTDIR}/Source/USB/usb_device.o: Source/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_device.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_device.o.d" -o ${OBJECTDIR}/Source/USB/usb_device.o Source/USB/usb_device.c   
	
${OBJECTDIR}/Source/USB/usb_function_hid.o: Source/USB/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source/USB 
	@${RM} ${OBJECTDIR}/Source/USB/usb_function_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/USB/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/USB/usb_function_hid.o.d" -o ${OBJECTDIR}/Source/USB/usb_function_hid.o Source/USB/usb_function_hid.c   
	
${OBJECTDIR}/Source/BootLoader.o: Source/BootLoader.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source 
	@${RM} ${OBJECTDIR}/Source/BootLoader.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/BootLoader.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/BootLoader.o.d" -o ${OBJECTDIR}/Source/BootLoader.o Source/BootLoader.c   
	
${OBJECTDIR}/Source/NVMem.o: Source/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/Source 
	@${RM} ${OBJECTDIR}/Source/NVMem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/Source/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"./" -I"./Include" -I"./Include/HardwareProfile" -Os -MMD -MF "${OBJECTDIR}/Source/NVMem.o.d" -o ${OBJECTDIR}/Source/NVMem.o Source/NVMem.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=_min_heap_size=30000,--gc-sections,--report-mem "-mperipheral-libs"
else
dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=30000,--gc-sections,--report-mem "-mperipheral-libs"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/PICrouter_HIDBL.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
