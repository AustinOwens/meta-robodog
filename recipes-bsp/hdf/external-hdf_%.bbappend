FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://${XSA_FILE_NAME}"

# meta-xilinx-tools depends on below vars
HDF_BASE = "file://"
HDF_PATH = "${XSA_FILE_NAME}"
