SIM_TEST_TARGET_STEPS ?= 60
SIM_WAIT_SECONDS ?= 60
SIM_CHAIN_T2_CALLS ?= 5
SIM_CHAIN_T1_CALLS ?= 1
SIM_CHAIN_WATCHDOG_CALLS ?= 1
SIM_TRACE_MAX_STEPS ?= 64
SIM_TRACE_PRINT_STEPS ?= $(SIM_TEST_TARGET_STEPS)
SIM_STACK_BYTES ?= 512
SIM_EXTRA_CPPFLAGS ?=
SIM_T2_TARGET_STEPS ?= 1500
SIM_T2_HOST_SAMPLES ?= 500
SIM_T2_SAMPLE_WAIT_MS ?= 1
OCP_SWEEP_VOUT_START ?= 0
OCP_SWEEP_VOUT_END ?= 7000
OCP_SWEEP_VOUT_STEP ?= 20
OCP_SWEEP_IMEAS ?= 500
OCP_SWEEP_IREF ?= 100
OCP_SWEEP_KI_INPUT ?= 30
OCP_SWEEP_ACCUM_INIT ?= 0
OCP_SWEEP_SETTLE ?= 1
OCP_SWEEP_DYNAMIC ?= 0
FLASH_UART_PORT ?=
FLASH_UART_BIN ?= ./AT45DB021E.bin
FLASH_UART_BAUD ?= 115200

.PHONY: sim sim-real sim-softstart sim-build sim-t2 sim-2p2z decomp-audit decomp-test c-unittest c-unittest-cov c-unittest-gap c-unittest-ocp-sweep flash-dump-test flash-uart2-send

sim-build:
	make TYPE_IMAGE=DEBUG_RUN MP_EXTRA_CC_PRE="-DSIMULATION_MODE -DSIM_AUTOMATION_MODE -DSIM_TEST_TARGET_STEPS=$(SIM_TEST_TARGET_STEPS) -DSIM_TRACE_MAX_STEPS=$(SIM_TRACE_MAX_STEPS) $(SIM_EXTRA_CPPFLAGS)" CONF=default -B -C HSTNS-PD44.X

sim:
	SIM_TEST_TARGET_STEPS=$(SIM_TEST_TARGET_STEPS) SIM_WAIT_SECONDS=$(SIM_WAIT_SECONDS) SIM_MODE=chain SIM_CHAIN_T2_CALLS=$(SIM_CHAIN_T2_CALLS) SIM_CHAIN_T1_CALLS=$(SIM_CHAIN_T1_CALLS) SIM_CHAIN_WATCHDOG_CALLS=$(SIM_CHAIN_WATCHDOG_CALLS) SIM_TRACE_MAX_STEPS=$(SIM_TRACE_MAX_STEPS) SIM_TRACE_PRINT_STEPS=$(SIM_TRACE_PRINT_STEPS) SIM_STACK_BYTES=$(SIM_STACK_BYTES) SIM_EXTRA_CPPFLAGS="-DSIM_MAIN_CHAIN_ONLY -DSIM_CHAIN_T2_CALLS=$(SIM_CHAIN_T2_CALLS) -DSIM_CHAIN_T1_CALLS=$(SIM_CHAIN_T1_CALLS) -DSIM_CHAIN_WATCHDOG_CALLS=$(SIM_CHAIN_WATCHDOG_CALLS) $(SIM_EXTRA_CPPFLAGS)" ./scripts/run_mplab_sim.sh

sim-real:
	SIM_TEST_TARGET_STEPS=$(SIM_TEST_TARGET_STEPS) SIM_WAIT_SECONDS=$(SIM_WAIT_SECONDS) SIM_MODE=chain SIM_CHAIN_T2_CALLS=50 SIM_CHAIN_T1_CALLS=5 SIM_TRACE_MAX_STEPS=$(SIM_TRACE_MAX_STEPS) SIM_TRACE_PRINT_STEPS=$(SIM_TRACE_PRINT_STEPS) SIM_STACK_BYTES=$(SIM_STACK_BYTES) SIM_EXTRA_CPPFLAGS="-DSIM_MAIN_CHAIN_ONLY -DSIM_CHAIN_T2_CALLS=50 -DSIM_CHAIN_T1_CALLS=5 $(SIM_EXTRA_CPPFLAGS)" ./scripts/run_mplab_sim.sh

sim-softstart:
	SIM_TEST_TARGET_STEPS=$(SIM_TEST_TARGET_STEPS) SIM_WAIT_SECONDS=$(SIM_WAIT_SECONDS) SIM_MODE=chain SIM_CHAIN_T2_CALLS=$(SIM_CHAIN_T2_CALLS) SIM_CHAIN_T1_CALLS=$(SIM_CHAIN_T1_CALLS) SIM_CHAIN_WATCHDOG_CALLS=$(SIM_CHAIN_WATCHDOG_CALLS) SIM_TRACE_MAX_STEPS=$(SIM_TRACE_MAX_STEPS) SIM_TRACE_PRINT_STEPS=$(SIM_TRACE_PRINT_STEPS) SIM_STACK_BYTES=$(SIM_STACK_BYTES) SIM_EXTRA_CPPFLAGS="-DSIM_MAIN_CHAIN_ONLY -DSIM_CHAIN_T2_CALLS=$(SIM_CHAIN_T2_CALLS) -DSIM_CHAIN_T1_CALLS=$(SIM_CHAIN_T1_CALLS) -DSIM_CHAIN_WATCHDOG_CALLS=$(SIM_CHAIN_WATCHDOG_CALLS) $(SIM_EXTRA_CPPFLAGS)" ./scripts/run_mplab_sim.sh

sim-t2:
	SIM_TEST_TARGET_STEPS=$(SIM_T2_TARGET_STEPS) SIM_WAIT_SECONDS=$(SIM_WAIT_SECONDS) SIM_MODE=t2 SIM_HOST_SAMPLES=$(SIM_T2_HOST_SAMPLES) SIM_SAMPLE_WAIT_MS=$(SIM_T2_SAMPLE_WAIT_MS) SIM_TRACE_MAX_STEPS=$(SIM_TRACE_MAX_STEPS) SIM_TRACE_PRINT_STEPS=$(SIM_T2_TARGET_STEPS) SIM_STACK_BYTES=$(SIM_STACK_BYTES) SIM_EXTRA_CPPFLAGS="-DSIM_T2_ONLY $(SIM_EXTRA_CPPFLAGS)" ./scripts/run_mplab_sim.sh

sim-2p2z:
	python3 ./scripts/run_2p2z_model.py 6000 ./out/sim-2p2z

decomp-audit:
	python3 ./scripts/decomp_verify.py --audit-only

decomp-test:
	python3 ./scripts/decomp_verify.py

c-unittest:
	mkdir -p ./out/c_unittest
	cc -std=c11 -Wall -Wextra -Werror -I./tests/c_unittest/include ./tests/c_unittest/test_decompiled_unittest.c -o ./out/c_unittest/test_decompiled_unittest
	./out/c_unittest/test_decompiled_unittest

c-unittest-cov:
	mkdir -p ./out/c_unittest
	cc -std=c11 -Wall -Wextra -Werror -fprofile-instr-generate -fcoverage-mapping -I./tests/c_unittest/include ./tests/c_unittest/test_decompiled_unittest.c -o ./out/c_unittest/test_decompiled_unittest_cov
	LLVM_PROFILE_FILE=./out/c_unittest/test_decompiled_unittest.profraw ./out/c_unittest/test_decompiled_unittest_cov
	xcrun llvm-profdata merge -sparse ./out/c_unittest/test_decompiled_unittest.profraw -o ./out/c_unittest/test_decompiled_unittest.profdata
	xcrun llvm-cov report ./out/c_unittest/test_decompiled_unittest_cov -instr-profile=./out/c_unittest/test_decompiled_unittest.profdata ./tests/c_unittest/test_decompiled_unittest.c ./decompiled/voltage_loop.c ./decompiled/pwm.c ./decompiled/protection.c ./decompiled/flash.c ./decompiled/adc.c

c-unittest-gap:
	python3 ./scripts/c_unittest_gap.py

flash-dump-test:
	python3 ./scripts/test_flash_dump_interface.py --dump ./AT45DB021E.bin --show-cal

flash-uart2-send:
	@test -n "$(FLASH_UART_PORT)" || (echo "FLASH_UART_PORT is required, e.g. make flash-uart2-send FLASH_UART_PORT=/dev/tty.usbserial-xxxx" && exit 2)
	python3 ./scripts/send_at45db_bin_uart2.py --port "$(FLASH_UART_PORT)" --bin "$(FLASH_UART_BIN)" --baud "$(FLASH_UART_BAUD)"

c-unittest-ocp-sweep:
	mkdir -p ./out/c_unittest
	cc -std=c11 -Wall -Wextra -Werror -I./tests/c_unittest/include ./tests/c_unittest/test_decompiled_unittest.c -o ./out/c_unittest/test_decompiled_unittest
	./out/c_unittest/test_decompiled_unittest --ocp-sweep --vout-start $(OCP_SWEEP_VOUT_START) --vout-end $(OCP_SWEEP_VOUT_END) --vout-step $(OCP_SWEEP_VOUT_STEP) --imeas $(OCP_SWEEP_IMEAS) --iref $(OCP_SWEEP_IREF) --ki-input $(OCP_SWEEP_KI_INPUT) --accum-init $(OCP_SWEEP_ACCUM_INIT) --settle $(OCP_SWEEP_SETTLE) > ./out/c_unittest/ocp_foldback_sweep.csv
ifneq ($(OCP_SWEEP_DYNAMIC),0)
	./out/c_unittest/test_decompiled_unittest --ocp-sweep --dynamic --vout-start $(OCP_SWEEP_VOUT_START) --vout-end $(OCP_SWEEP_VOUT_END) --vout-step $(OCP_SWEEP_VOUT_STEP) --imeas $(OCP_SWEEP_IMEAS) --iref $(OCP_SWEEP_IREF) --ki-input $(OCP_SWEEP_KI_INPUT) --accum-init $(OCP_SWEEP_ACCUM_INIT) --settle $(OCP_SWEEP_SETTLE) > ./out/c_unittest/ocp_foldback_sweep_dynamic.csv
endif
	python3 ./scripts/plot_ocp_foldback.py ./out/c_unittest/ocp_foldback_sweep.csv ./out/c_unittest/ocp_foldback_sweep.svg
ifneq ($(OCP_SWEEP_DYNAMIC),0)
	python3 ./scripts/plot_ocp_foldback.py ./out/c_unittest/ocp_foldback_sweep_dynamic.csv ./out/c_unittest/ocp_foldback_sweep_dynamic.svg
endif
	@echo "Trace: csv=./out/c_unittest/ocp_foldback_sweep.csv svg=./out/c_unittest/ocp_foldback_sweep.svg"
