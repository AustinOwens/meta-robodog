RECIPE_DIR := "${@os.path.dirname(d.getVar('FILE', True))}"
BIF_PARTITION_IMAGE[device-tree] = "${RECIPE_DIR}/../u-boot/files/${DEVICETREE_NAME}.dtb"
