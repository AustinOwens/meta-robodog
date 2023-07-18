SUMMARY = "Add machine_vision_server.py to rootfs"
DESCRIPTION = "Streams images and commands over a socket from a camera"
LICENSE = "LGPL-3.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=3000208d539ec061b899bce1d9ce9404"

SRC_URI = "git://github.com/austinowens/machine_vision_server.git;protocol=https;branch=langdale"

# TODO: AUTOREV will grab latest hash on above specified branch. Should eventually
# lock this into a specific hash when source code becomes more mature
SRCREV = "${AUTOREV}"

# S specifies the directory where the source code is checked out to. By default,
# Bitbake fetches the code to ${WORKDIR}/git for Git repositories. Hence, we 
# explicitly set S to this path so that all operations such as do_compile and 
# do_install, as well as the LIC_FILES_CHKSUM lookup, happen in the correct directory.
S = "${WORKDIR}/git"

RDEPENDS_${PN} = "opencv"

do_install() {
    install -d ${D}/usr/bin/
    install -m 0766 ${S}/machine_vision_server.py ${D}/usr/bin/
}

FILES_${PN} += "/usr/bin/machine_vision_server.py"
