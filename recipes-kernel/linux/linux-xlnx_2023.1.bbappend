FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://robodog_defconfig"

# Specify user who is building the linux kernel
KBUILD_BUILD_USER = "aowens"

# Remove linux-xlnx.inc do_kernel_metadata:prepend function because it would delete any defconfigs in the workspace
do_kernel_metadata:prepend() {
}

KBUILD_DEFCONFIG:zynq = ""
