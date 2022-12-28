SUMMARY = "Creates the baseline image for the RoboDog OS"
DESCRIPTION = "Baseline image recipe for the RoboDog OS"
LICENSE = "LGPL-3.0-only"

# Inherit from the the core-image-minimal-dev image
inherit core-image

# The poky/meta/classes-recipe/core-image.bbclass implements the mapping of strings in
# the global variable IMAGE_FEATURES to predefined packagegroups. What IMAGE_FEATURES
# allows us to do is to change the content of the core-image-base.bb image without
# editing the file, and we can simply add pre-defined features to the global variable
# IMAGE_FEATURES. This also allows us to create definitions of IMAGE_FEATURES that will
# be applicable to all images that are derived from the core-image.bbclass.
IMAGE_FEATURES += "splash ssh-server-dropbear package-management"

# Install additional software packages
CORE_IMAGE_EXTRA_INSTALL += "robodog-packagegroup-testapps"

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
