SUMMARY = "Creates the baseline image for the RoboDog OS"
DESCRIPTION = "Baseline image recipe for the RoboDog OS"
LICENSE = "LGPL-3.0-only"

# Inherit from the the core-image-minimal-dev image
require recipes-core/images/core-image-minimal-dev.bb

# The poky/meta/classes-recipe/core-image.bbclass implements the mapping of strings in
# the global variable IMAGE_FEATURES to predefined packagegroups. What IMAGE_FEATURES
# allows us to do is to change the content of the core-image-base.bb image without
# editing the file, and we can simply add pre-defined features to the global variable
# IMAGE_FEATURES. This also allows us to create definitions of IMAGE_FEATURES that will
# be applicable to all images that are derived from the core-image.bbclass.
IMAGE_FEATURES += "ssh-server-dropbear"
IMAGE_FEATURES += "package-management"

# Add a package feed URI for the package-management IMAGE_FEATURE
PACKAGE_CLASSES = "package_rpm"
PACKAGE_FEED_URIS = "http://127.0.0.1/zybo-z7/yocto_pkg_repo/rpm"

# Packages to install
IMAGE_INSTALL:append = " packagegroup-core-buildessential"
IMAGE_INSTALL:append = " devmem2"
IMAGE_INSTALL:append = " memtester"
IMAGE_INSTALL:append = " ethtool"
IMAGE_INSTALL:append = " i2c-tools"
IMAGE_INSTALL:append = " spidev-test"
IMAGE_INSTALL:append = " spitools"
IMAGE_INSTALL:append = " vim"
IMAGE_INSTALL:append = " usbutils"
IMAGE_INSTALL:append = " v4l-utils"
IMAGE_INSTALL:append = " opencv"
IMAGE_INSTALL:append = " pip-odrive"

# Custom apps to install
IMAGE_INSTALL:append = " machine-vision-server"
#IMAGE_INSTALL:append = " rgb-led-dance"

# Kernel
IMAGE_INSTALL:append = " kernel-devsrc"
IMAGE_INSTALL:append = " kernel-module-axi2spi-periph"

# Banner output
python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*                ROBODOG IMAGE                *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

# Add banner output before build step
addtask display_banner before do_build
