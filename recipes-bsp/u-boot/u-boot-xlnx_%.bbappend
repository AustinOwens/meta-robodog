FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
#SRC_URI += "file://uenv_xilinx_zynq_virt_defconfig.patch"
SRC_URI += "file://uEnv.txt"

UBOOT_ENV = "uEnv"
