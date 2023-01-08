SRC_URI:remove = "file://makefile-skip-copy_bsp.sh.patch"

COMPATIBLE_HOST = ".*-(elf|eabi|gnueabi)"
COMPATIBLE_MACHINE:zynq-generic = ".*"
APP_DIR:zynq-generic = "zynq_fsbl"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://0001-Add-Debug-Flag-to-Makefile.patch"

CC = "arm-none-eabi-gcc"
AR = "arm-none-eabi-ar"

BSP_DIR:zynq-generic ?= "${B}/../misc"
BSP_TARGETS_DIR ?= "${BSP_DIR}/ps7_cortexa9_0/libsrc"
