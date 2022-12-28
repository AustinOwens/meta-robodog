DESCRIPTION = "RoboDog test application packagegroup"
SUMMARY = "RoboDog packagegroup - tools/testapps"
LICENSE = "LGPL-3.0-only"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

RDEPENDS_${PN} = " \
    ethtool \
    i2c-tools \
    memtester \
"
