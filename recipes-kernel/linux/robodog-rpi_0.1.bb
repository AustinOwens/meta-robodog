SUMMARY = "Creates"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "LGPL-3.0-only"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*           RPI LINUX KERNEL RECIPE           *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build
