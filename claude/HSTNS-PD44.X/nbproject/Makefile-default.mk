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
FINAL_IMAGE=${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../decompiled/main.c ../decompiled/init.c ../decompiled/state_machine.c ../decompiled/monitoring.c ../decompiled/isr.c ../decompiled/isr_t1.c ../decompiled/isr_t2.c ../decompiled/voltage_loop.c ../decompiled/adc.c ../decompiled/pwm.c ../decompiled/protection.c ../decompiled/pmbus.c ../decompiled/flash.c ../decompiled/system.c ../decompiled/utilities.c ../decompiled/variables.c ../decompiled/stubs.c ../decompiled/serial.c ../decompiled/simulator.c ../decompiled/trap.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1533516737/main.o ${OBJECTDIR}/_ext/1533516737/init.o ${OBJECTDIR}/_ext/1533516737/state_machine.o ${OBJECTDIR}/_ext/1533516737/monitoring.o ${OBJECTDIR}/_ext/1533516737/isr.o ${OBJECTDIR}/_ext/1533516737/isr_t1.o ${OBJECTDIR}/_ext/1533516737/isr_t2.o ${OBJECTDIR}/_ext/1533516737/voltage_loop.o ${OBJECTDIR}/_ext/1533516737/adc.o ${OBJECTDIR}/_ext/1533516737/pwm.o ${OBJECTDIR}/_ext/1533516737/protection.o ${OBJECTDIR}/_ext/1533516737/pmbus.o ${OBJECTDIR}/_ext/1533516737/flash.o ${OBJECTDIR}/_ext/1533516737/system.o ${OBJECTDIR}/_ext/1533516737/utilities.o ${OBJECTDIR}/_ext/1533516737/variables.o ${OBJECTDIR}/_ext/1533516737/stubs.o ${OBJECTDIR}/_ext/1533516737/serial.o ${OBJECTDIR}/_ext/1533516737/simulator.o ${OBJECTDIR}/_ext/1533516737/trap.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1533516737/main.o.d ${OBJECTDIR}/_ext/1533516737/init.o.d ${OBJECTDIR}/_ext/1533516737/state_machine.o.d ${OBJECTDIR}/_ext/1533516737/monitoring.o.d ${OBJECTDIR}/_ext/1533516737/isr.o.d ${OBJECTDIR}/_ext/1533516737/isr_t1.o.d ${OBJECTDIR}/_ext/1533516737/isr_t2.o.d ${OBJECTDIR}/_ext/1533516737/voltage_loop.o.d ${OBJECTDIR}/_ext/1533516737/adc.o.d ${OBJECTDIR}/_ext/1533516737/pwm.o.d ${OBJECTDIR}/_ext/1533516737/protection.o.d ${OBJECTDIR}/_ext/1533516737/pmbus.o.d ${OBJECTDIR}/_ext/1533516737/flash.o.d ${OBJECTDIR}/_ext/1533516737/system.o.d ${OBJECTDIR}/_ext/1533516737/utilities.o.d ${OBJECTDIR}/_ext/1533516737/variables.o.d ${OBJECTDIR}/_ext/1533516737/stubs.o.d ${OBJECTDIR}/_ext/1533516737/serial.o.d ${OBJECTDIR}/_ext/1533516737/simulator.o.d ${OBJECTDIR}/_ext/1533516737/trap.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1533516737/main.o ${OBJECTDIR}/_ext/1533516737/init.o ${OBJECTDIR}/_ext/1533516737/state_machine.o ${OBJECTDIR}/_ext/1533516737/monitoring.o ${OBJECTDIR}/_ext/1533516737/isr.o ${OBJECTDIR}/_ext/1533516737/isr_t1.o ${OBJECTDIR}/_ext/1533516737/isr_t2.o ${OBJECTDIR}/_ext/1533516737/voltage_loop.o ${OBJECTDIR}/_ext/1533516737/adc.o ${OBJECTDIR}/_ext/1533516737/pwm.o ${OBJECTDIR}/_ext/1533516737/protection.o ${OBJECTDIR}/_ext/1533516737/pmbus.o ${OBJECTDIR}/_ext/1533516737/flash.o ${OBJECTDIR}/_ext/1533516737/system.o ${OBJECTDIR}/_ext/1533516737/utilities.o ${OBJECTDIR}/_ext/1533516737/variables.o ${OBJECTDIR}/_ext/1533516737/stubs.o ${OBJECTDIR}/_ext/1533516737/serial.o ${OBJECTDIR}/_ext/1533516737/simulator.o ${OBJECTDIR}/_ext/1533516737/trap.o

# Source Files
SOURCEFILES=../decompiled/main.c ../decompiled/init.c ../decompiled/state_machine.c ../decompiled/monitoring.c ../decompiled/isr.c ../decompiled/isr_t1.c ../decompiled/isr_t2.c ../decompiled/voltage_loop.c ../decompiled/adc.c ../decompiled/pwm.c ../decompiled/protection.c ../decompiled/pmbus.c ../decompiled/flash.c ../decompiled/system.c ../decompiled/utilities.c ../decompiled/variables.c ../decompiled/stubs.c ../decompiled/serial.c ../decompiled/simulator.c ../decompiled/trap.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ64GS606
MP_LINKER_FILE_OPTION=,--script=p33FJ64GS606.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1533516737/main.o: ../decompiled/main.c  .generated_files/flags/default/a9196644872121ef3ea6cf9af362b43ee1b56089 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/main.c  -o ${OBJECTDIR}/_ext/1533516737/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/main.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/init.o: ../decompiled/init.c  .generated_files/flags/default/8af8d00bbb47c47bb9d261f27f0bd801481cbd43 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/init.c  -o ${OBJECTDIR}/_ext/1533516737/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/init.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/state_machine.o: ../decompiled/state_machine.c  .generated_files/flags/default/d5ecb7b11768b02c3881abcde461926398487632 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/state_machine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/state_machine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/state_machine.c  -o ${OBJECTDIR}/_ext/1533516737/state_machine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/state_machine.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/monitoring.o: ../decompiled/monitoring.c  .generated_files/flags/default/c545ac3795e30d57e78723cc1810ea58dc6b8b74 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/monitoring.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/monitoring.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/monitoring.c  -o ${OBJECTDIR}/_ext/1533516737/monitoring.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/monitoring.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr.o: ../decompiled/isr.c  .generated_files/flags/default/54cd5f6679ad67638ab551dc11bd7c8a9ab3c9b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr.c  -o ${OBJECTDIR}/_ext/1533516737/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr_t1.o: ../decompiled/isr_t1.c  .generated_files/flags/default/c2005e317faffcf8d5acfeed424c3bc18105723e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr_t1.c  -o ${OBJECTDIR}/_ext/1533516737/isr_t1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr_t1.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr_t2.o: ../decompiled/isr_t2.c  .generated_files/flags/default/d3aba4dea8afff2111c6e1cbc7229064d4e9222a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr_t2.c  -o ${OBJECTDIR}/_ext/1533516737/isr_t2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr_t2.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/voltage_loop.o: ../decompiled/voltage_loop.c  .generated_files/flags/default/75801939fa70c5c1600f5a2b146ea10160f6923c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/voltage_loop.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/voltage_loop.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/voltage_loop.c  -o ${OBJECTDIR}/_ext/1533516737/voltage_loop.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/voltage_loop.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/adc.o: ../decompiled/adc.c  .generated_files/flags/default/9a96bbcb7d0ebf15d085ab03f2f7075b83a7bc7d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/adc.c  -o ${OBJECTDIR}/_ext/1533516737/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/adc.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/pwm.o: ../decompiled/pwm.c  .generated_files/flags/default/e323965ed4722aebca33e42029c64d2ab4779b73 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/pwm.c  -o ${OBJECTDIR}/_ext/1533516737/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/pwm.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/protection.o: ../decompiled/protection.c  .generated_files/flags/default/9111bd5423253ad9a99df5e0ab2fb0e539a96300 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/protection.c  -o ${OBJECTDIR}/_ext/1533516737/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/protection.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/pmbus.o: ../decompiled/pmbus.c  .generated_files/flags/default/ae465765964526c49504c9ec73a206fe3556b0d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pmbus.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pmbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/pmbus.c  -o ${OBJECTDIR}/_ext/1533516737/pmbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/pmbus.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/flash.o: ../decompiled/flash.c  .generated_files/flags/default/bd1dde00afb2ce7b3e9829d450802d5f421e351a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/flash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/flash.c  -o ${OBJECTDIR}/_ext/1533516737/flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/flash.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/system.o: ../decompiled/system.c  .generated_files/flags/default/7343d3db816b7df083ffeab79df24cde822247c8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/system.c  -o ${OBJECTDIR}/_ext/1533516737/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/system.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/utilities.o: ../decompiled/utilities.c  .generated_files/flags/default/bf0eda23ada4c1da6001bb9747014f1175b6fcf0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/utilities.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/utilities.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/utilities.c  -o ${OBJECTDIR}/_ext/1533516737/utilities.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/utilities.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/variables.o: ../decompiled/variables.c  .generated_files/flags/default/2d6a95d564a6432c538e50bf211c360d6fff0f97 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/variables.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/variables.c  -o ${OBJECTDIR}/_ext/1533516737/variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/variables.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/stubs.o: ../decompiled/stubs.c  .generated_files/flags/default/eb15646d02e5193b4eae50bac1e704e89dcfe293 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/stubs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/stubs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/stubs.c  -o ${OBJECTDIR}/_ext/1533516737/stubs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/stubs.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/serial.o: ../decompiled/serial.c  .generated_files/flags/default/9158f0c4fc8979dceda3345b0e9e3a5f4150b232 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/serial.c  -o ${OBJECTDIR}/_ext/1533516737/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/serial.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/simulator.o: ../decompiled/simulator.c  .generated_files/flags/default/83e05b2f105aab4a51ad187b9f795b9b82424557 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/simulator.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/simulator.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/simulator.c  -o ${OBJECTDIR}/_ext/1533516737/simulator.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/simulator.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/trap.o: ../decompiled/trap.c  .generated_files/flags/default/ec50f589a6d57848aabd5740397663dd45750e0f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/trap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/trap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/trap.c  -o ${OBJECTDIR}/_ext/1533516737/trap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/trap.o.d"      -g -D__DEBUG     -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1533516737/main.o: ../decompiled/main.c  .generated_files/flags/default/af5bfe98e3324a0be15caae88cb6991cbe920465 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/main.c  -o ${OBJECTDIR}/_ext/1533516737/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/init.o: ../decompiled/init.c  .generated_files/flags/default/67701918a4078f86ffb11733feb82f572e0870ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/init.c  -o ${OBJECTDIR}/_ext/1533516737/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/init.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/state_machine.o: ../decompiled/state_machine.c  .generated_files/flags/default/7d6e863187502d07913c741e2ad95f569f2bdc4f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/state_machine.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/state_machine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/state_machine.c  -o ${OBJECTDIR}/_ext/1533516737/state_machine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/state_machine.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/monitoring.o: ../decompiled/monitoring.c  .generated_files/flags/default/5de3cdffe1c2d850a3a55c4bffe39c6ae21f33d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/monitoring.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/monitoring.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/monitoring.c  -o ${OBJECTDIR}/_ext/1533516737/monitoring.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/monitoring.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr.o: ../decompiled/isr.c  .generated_files/flags/default/bdbf292c447e4383c2e07c678f25122ae619ba2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr.c  -o ${OBJECTDIR}/_ext/1533516737/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr_t1.o: ../decompiled/isr_t1.c  .generated_files/flags/default/dd17674dc43c4628c94276a784656fbe78f06fad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr_t1.c  -o ${OBJECTDIR}/_ext/1533516737/isr_t1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr_t1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/isr_t2.o: ../decompiled/isr_t2.c  .generated_files/flags/default/92320908706c1c3ad09c784928a941ecc6465621 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/isr_t2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/isr_t2.c  -o ${OBJECTDIR}/_ext/1533516737/isr_t2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/isr_t2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/voltage_loop.o: ../decompiled/voltage_loop.c  .generated_files/flags/default/599c4c7af09a9a12c40e7009a0f22b6bf9f2de32 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/voltage_loop.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/voltage_loop.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/voltage_loop.c  -o ${OBJECTDIR}/_ext/1533516737/voltage_loop.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/voltage_loop.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/adc.o: ../decompiled/adc.c  .generated_files/flags/default/58070caaeb3efb3206c2fa61dbb357bf0231e1fe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/adc.c  -o ${OBJECTDIR}/_ext/1533516737/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/adc.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/pwm.o: ../decompiled/pwm.c  .generated_files/flags/default/633c48043dad3df6c2c8573ffa2e0b183e45f52d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/pwm.c  -o ${OBJECTDIR}/_ext/1533516737/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/pwm.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/protection.o: ../decompiled/protection.c  .generated_files/flags/default/926e4fdf90659977db389d47680b099a9fe2aa02 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/protection.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/protection.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/protection.c  -o ${OBJECTDIR}/_ext/1533516737/protection.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/protection.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/pmbus.o: ../decompiled/pmbus.c  .generated_files/flags/default/b3ce3608ef40ab9c6e78f76dc11bf0346c12bd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pmbus.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/pmbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/pmbus.c  -o ${OBJECTDIR}/_ext/1533516737/pmbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/pmbus.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/flash.o: ../decompiled/flash.c  .generated_files/flags/default/873165a23bd87573b108189e74acb0b09c049e76 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/flash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/flash.c  -o ${OBJECTDIR}/_ext/1533516737/flash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/flash.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/system.o: ../decompiled/system.c  .generated_files/flags/default/48d330b572919777b75ceb26c5920f22b2293a55 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/system.c  -o ${OBJECTDIR}/_ext/1533516737/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/system.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/utilities.o: ../decompiled/utilities.c  .generated_files/flags/default/f684349f8b909ae835f6ff9ae6c33ba18a69a61e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/utilities.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/utilities.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/utilities.c  -o ${OBJECTDIR}/_ext/1533516737/utilities.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/utilities.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/variables.o: ../decompiled/variables.c  .generated_files/flags/default/fc6153427431a7486f9ac90c5718e5ea5c81427b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/variables.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/variables.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/variables.c  -o ${OBJECTDIR}/_ext/1533516737/variables.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/variables.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/stubs.o: ../decompiled/stubs.c  .generated_files/flags/default/97f350aa98f7d8aa7f2b0d8f386c6f424a50519a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/stubs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/stubs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/stubs.c  -o ${OBJECTDIR}/_ext/1533516737/stubs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/stubs.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/serial.o: ../decompiled/serial.c  .generated_files/flags/default/e082c7c832d260a3436f61461136324ff71fc853 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/serial.c  -o ${OBJECTDIR}/_ext/1533516737/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/serial.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/simulator.o: ../decompiled/simulator.c  .generated_files/flags/default/fd29494208983b18639bc014af2bc645fe69993b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/simulator.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/simulator.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/simulator.c  -o ${OBJECTDIR}/_ext/1533516737/simulator.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/simulator.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1533516737/trap.o: ../decompiled/trap.c  .generated_files/flags/default/d28ac82525eda374c0ac8c05b0b3181cd37d5b37 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1533516737" 
	@${RM} ${OBJECTDIR}/_ext/1533516737/trap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1533516737/trap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../decompiled/trap.c  -o ${OBJECTDIR}/_ext/1533516737/trap.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1533516737/trap.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -O1 -I"../decompiled" -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled"  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -I"../decompiled" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/HSTNS-PD44.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
