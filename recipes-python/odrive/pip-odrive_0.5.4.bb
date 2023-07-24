SUMMARY = "Installs odrive python lib to rootfs"
HOMEPAGE ="https://pypi.org/project/odrive"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://PKG-INFO;md5=e81a4ba016f6cb7685c746d2e130509d"

RDEPENDS:${PN} = "libusb1"

# Skip "already-stripped" QA check
INSANE_SKIP:${PN} += "already-stripped"
INSANE_SKIP:${PN} += "installed-vs-shipped"

inherit pypi setuptools3

PYPI_PACKAGE = "odrive"

SRC_URI[md5sum] = "de6215dc9ef74085348d7917a9cbcb3f"
SRC_URI[sha256sum] = "af005eeaf8ab95d04ca3b5a09b952330a70d382920454a4a3de62b0e25b05277"

# Odrive lib includes libs/files for aarch64, amd64 and armhf. Because there 
# are two other archs that are not associated with armhf, QA checks will throw
# an error so manually install packages so the following libs will not be 
# installed:
#
# odrivetool.bat
# libfibre-linux-aarch64.so
# libfibre-linux-amd64.so
# libwinpthread-1.dll
# libfibre-macos-x86.dylib
# libfibre-windows-amd64.dll
# 
# This is so Yocto QA system doesn't throw 'arch' and 'file-rdeps' errors
# because it thinks it needs the dependancies for the above lib when in
# reality, those libs will never be running on an zybo (armhf) platform

FILES:${PN} = "${bindir}/odrive_demo.py"
FILES:${PN} += "${bindir}/odrivetool"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive-0.5.4.dist-info/*"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/utils.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/shell.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/configuration.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/enums.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/version.txt"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/version.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/dfu.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/__init__.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/__pycache__/*"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/dfuse/*"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/utils.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/shell.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/libfibre-linux-armhf.so"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/protocol.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/libfibre.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/__init__.py"
FILES:${PN} += "${PYTHON_SITEPACKAGES_DIR}/odrive/pyfibre/fibre/__pycache__/*"

