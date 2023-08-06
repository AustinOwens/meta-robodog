SUMMARY = "Makes RGB LEDs dance"
LICENSE = "CLOSED"

# This recipe depends on python as a runtime dependency
RDEPENDS_${PN} = "python-core"

SRC_URI = "file://rgb_led_dance.py"

do_install() {
    install -d ${D}${bindir}/
    install -m 0755 ${WORKDIR}/rgb_led_dance.py ${D}${bindir}/rgb_led_dance.py
}

FILES_${PN} += "${bindir}/rgb_led_dance.py"
