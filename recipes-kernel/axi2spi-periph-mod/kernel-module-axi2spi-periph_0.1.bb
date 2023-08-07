DESCRIPTION = "Linux driver for the AXI Quad SPI v3.2 Xilinx IP core"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-or-later;md5=fed54355545ffd980b814dab4a3b312c"

inherit module

SRC_URI = "file://axi2spi_periph.c"
SRC_URI += "file://Makefile"

S = "${WORKDIR}"

DEPENDS += "virtual/kernel"

# The output directory for the kernel module
KERNEL_OUTPUT = "${D}/lib/modules/${KERNEL_VERSION}/kernel/drivers/axi2spi_periph"

# Set the output directory in the Makefile environment
EXTRA_OEMAKE += "KERNEL_OUTPUT=${KERNEL_OUTPUT}"

