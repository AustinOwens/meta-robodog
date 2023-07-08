FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# u-boot-xlnx.inc depends on BASE_DTS
BASE_DTS = "${DEVICETREE_NAME}"

SRC_URI += "file://uEnv.txt"
SRC_URI += "file://${BASE_DTS}.dtb"

UBOOT_ENV = "uEnv"

DTB_PATH = "boot/devicetree"
DTB_NAME = "${BASE_DTS}.dtb"

do_configure:prepend() {
    install -d ${RECIPE_SYSROOT}/${DTB_PATH}
    install -m 0644 ${WORKDIR}/${DTB_NAME} ${RECIPE_SYSROOT}/${DTB_PATH}
}
