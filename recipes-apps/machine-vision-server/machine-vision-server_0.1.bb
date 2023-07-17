SUMMARY = "Add machine_vision_server.py to rootfs"
DESCRIPTION = "Streams images and commands over a socket from a camera"
LICENSE = "LGPL-3.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/LGPL-3.0-only;md5=bfccfe952269fff2b407dd11f2f3083b"

SRC_URI = "file://machine_vision_server.py"

do_install() {
    install -d ${D}/usr/bin/
    install -m 0766 ${WORKDIR}/machine_vision_server.py ${D}/usr/bin/
}

FILES_${PN} += "/usr/bin/machine_vision_server.py"
