SUMMARY = "Script to help with generating binary files to send via spidev_test"
LICENSE = "CLOSED"

# This recipe depends on python as a runtime dependency
RDEPENDS_${PN} = "python-core"

SRC_URI = "file://bin_gen.py"

do_install() {
    install -d ${D}${bindir}/
    install -m 0755 ${WORKDIR}/bin_gen.py ${D}${bindir}/
}

FILES_${PN} += "${bindir}/bin_gen.py"
