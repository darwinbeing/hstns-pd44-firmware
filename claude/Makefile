SCL_FILE ?= ./scripts/stimulus/hstns_startup_llc.scl
SCL_HOST_SAMPLES ?= 200
SCL_SAMPLE_WAIT_MS ?= 2
SCL_POST_HALT_WAIT_MS ?= 1
SCL_MDB_RETRIES ?= 12
SCL_INCLUDE_INITIAL_SAMPLE ?= 1
SCL_INIT_WAIT_MS ?= 2
SCL_WARMUP_CYCLES ?= 1
SCL_PRINT_MDB_LOG ?= 0
SCL_EXTRA_CPPFLAGS ?=
SCL_EXTRA_SYMBOLS ?=
SCL_FLASH_DUMP ?=
SIM_COMPARE_SCL_FILE ?= ./scripts/stimulus/hstns_startup_llc.scl
SIM_COMPARE_SAMPLES ?= 0
SIM_COMPARE_SAMPLE_WAIT_MS ?= 1
SIM_COMPARE_INIT_WAIT_MS ?= 1000
SIM_COMPARE_POST_HALT_WAIT_MS ?= 1
SIM_COMPARE_WARMUP_CYCLES ?= 1
SIM_COMPARE_INCLUDE_INITIAL ?= 1
SIM_COMPARE_PMBUS_SUITE ?= all
SIM_COMPARE_PMBUS_BYTE_OBSERVABLES ?= 1
SIM_COMPARE_SFRS ?= default
SIM_COMPARE_SYMBOLS ?= none
SIM_COMPARE_FLASH_DUMP ?=
SIM_COMPARE_NO_REBUILD ?= 0
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
CUNIT_CFLAGS ?= -std=c11 -Wall -Wextra -Werror -ffunction-sections -fdata-sections -I./tests/c_unittest/include
CUNIT_LDFLAGS ?= -Wl,--gc-sections

.PHONY: sim sim-scl sim-scl-state sim-scl-llc-hw sim-compare-original sim-2p2z decomp-audit decomp-test c-unittest c-unittest-cov c-unittest-gap c-unittest-ocp-sweep flash-dump-test flash-uart2-send

sim:
	$(MAKE) sim-scl

sim-scl:
	SCL_FILE=$(SCL_FILE) SCL_HOST_SAMPLES=$(SCL_HOST_SAMPLES) SCL_SAMPLE_WAIT_MS=$(SCL_SAMPLE_WAIT_MS) SCL_POST_HALT_WAIT_MS=$(SCL_POST_HALT_WAIT_MS) SCL_MDB_RETRIES=$(SCL_MDB_RETRIES) SCL_INCLUDE_INITIAL_SAMPLE=$(SCL_INCLUDE_INITIAL_SAMPLE) SCL_INIT_WAIT_MS=$(SCL_INIT_WAIT_MS) SCL_WARMUP_CYCLES=$(SCL_WARMUP_CYCLES) SCL_PRINT_MDB_LOG=$(SCL_PRINT_MDB_LOG) SCL_EXTRA_CPPFLAGS="$(SCL_EXTRA_CPPFLAGS)" SCL_EXTRA_SYMBOLS="$(SCL_EXTRA_SYMBOLS)" SCL_FLASH_DUMP="$(SCL_FLASH_DUMP)" ./scripts/run_mplab_scl.sh

sim-scl-state: sim-scl
	python3 ./scripts/parse_state_transitions.py ./out/sim_scl/scl_trace.csv

sim-scl-llc-hw:
	$(MAKE) sim-scl-state \
		SCL_FILE=./scripts/stimulus/hstns_startup_state_nominal.scl \
		SCL_EXTRA_SYMBOLS="ptperCommand,pdc1,pdc2,pdc3,PDC1,PDC2,PDC3,PTCON,IEC3,IFS3"

sim-compare-original:
	python3 ./scripts/sim_compare_original.py \
		--scl "$(SIM_COMPARE_SCL_FILE)" \
		--samples $(SIM_COMPARE_SAMPLES) \
		--sample-wait-ms $(SIM_COMPARE_SAMPLE_WAIT_MS) \
		--init-wait-ms $(SIM_COMPARE_INIT_WAIT_MS) \
		--post-halt-wait-ms $(SIM_COMPARE_POST_HALT_WAIT_MS) \
		--warmup-cycles $(SIM_COMPARE_WARMUP_CYCLES) \
		--include-initial $(SIM_COMPARE_INCLUDE_INITIAL) \
		--pmbus-suite "$(SIM_COMPARE_PMBUS_SUITE)" \
		--pmbus-byte-observables $(SIM_COMPARE_PMBUS_BYTE_OBSERVABLES) \
		--sfrs "$(SIM_COMPARE_SFRS)" \
		--symbols "$(SIM_COMPARE_SYMBOLS)" \
		$(if $(SIM_COMPARE_FLASH_DUMP),--flash-dump "$(SIM_COMPARE_FLASH_DUMP)",) \
		$(if $(filter 1,$(SIM_COMPARE_NO_REBUILD)),--no-rebuild,)

sim-2p2z:
	python3 ./scripts/run_2p2z_model.py 6000 ./out/sim-2p2z

decomp-audit:
	python3 ./scripts/decomp_verify.py --audit-only

decomp-test:
	python3 ./scripts/decomp_verify.py

c-unittest:
	mkdir -p ./out/c_unittest
	cc $(CUNIT_CFLAGS) ./tests/c_unittest/test_decompiled_unittest.c $(CUNIT_LDFLAGS) -o ./out/c_unittest/test_decompiled_unittest
	./out/c_unittest/test_decompiled_unittest

c-unittest-cov:
	mkdir -p ./out/c_unittest
	cc $(CUNIT_CFLAGS) -fprofile-instr-generate -fcoverage-mapping ./tests/c_unittest/test_decompiled_unittest.c $(CUNIT_LDFLAGS) -o ./out/c_unittest/test_decompiled_unittest_cov
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
	cc $(CUNIT_CFLAGS) ./tests/c_unittest/test_decompiled_unittest.c $(CUNIT_LDFLAGS) -o ./out/c_unittest/test_decompiled_unittest
	./out/c_unittest/test_decompiled_unittest --ocp-sweep --vout-start $(OCP_SWEEP_VOUT_START) --vout-end $(OCP_SWEEP_VOUT_END) --vout-step $(OCP_SWEEP_VOUT_STEP) --imeas $(OCP_SWEEP_IMEAS) --iref $(OCP_SWEEP_IREF) --ki-input $(OCP_SWEEP_KI_INPUT) --accum-init $(OCP_SWEEP_ACCUM_INIT) --settle $(OCP_SWEEP_SETTLE) > ./out/c_unittest/ocp_foldback_sweep.csv
ifneq ($(OCP_SWEEP_DYNAMIC),0)
	./out/c_unittest/test_decompiled_unittest --ocp-sweep --dynamic --vout-start $(OCP_SWEEP_VOUT_START) --vout-end $(OCP_SWEEP_VOUT_END) --vout-step $(OCP_SWEEP_VOUT_STEP) --imeas $(OCP_SWEEP_IMEAS) --iref $(OCP_SWEEP_IREF) --ki-input $(OCP_SWEEP_KI_INPUT) --accum-init $(OCP_SWEEP_ACCUM_INIT) --settle $(OCP_SWEEP_SETTLE) > ./out/c_unittest/ocp_foldback_sweep_dynamic.csv
endif
	python3 ./scripts/plot_ocp_foldback.py ./out/c_unittest/ocp_foldback_sweep.csv ./out/c_unittest/ocp_foldback_sweep.svg
ifneq ($(OCP_SWEEP_DYNAMIC),0)
	python3 ./scripts/plot_ocp_foldback.py ./out/c_unittest/ocp_foldback_sweep_dynamic.csv ./out/c_unittest/ocp_foldback_sweep_dynamic.svg
endif
	@echo "Trace: csv=./out/c_unittest/ocp_foldback_sweep.csv svg=./out/c_unittest/ocp_foldback_sweep.svg"
