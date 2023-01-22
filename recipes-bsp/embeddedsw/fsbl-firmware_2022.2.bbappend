SRC_URI:remove = "file://makefile-skip-copy_bsp.sh.patch"

COMPATIBLE_HOST = ".*-(elf|eabi|gnueabi)"
COMPATIBLE_MACHINE:zynq-generic = ".*"
APP_DIR:zynq-generic = "zynq_fsbl"

CC = "arm-none-eabi-gcc"
AR = "arm-none-eabi-ar"

BSP_DIR:zynq-generic ?= "${B}/../misc"
BSP_TARGETS_DIR ?= "${BSP_DIR}/ps7_cortexa9_0/libsrc"

# Allow FSBL to be built with DEBUG prints on
XSCTH_BUILD_DEBUG="1"
