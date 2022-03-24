

SUMMARY = "Editline"
DESCRIPTION = "A small replacement for GNU readline() for UNIX"
LICENSE = "Turner-Salz-93"

DEPENDS:append = ""

SRC_URI = "https://github.com/troglobit/editline/releases/download/${PV}/editline-${PV}.tar.xz \
"

SRC_URI[sha256sum] = "df223b3333a545fddbc67b49ded3d242c66fadf7a04beb3ada20957fcd1ffc0e"

LIC_FILES_CHKSUM = "file://LICENSE;md5=f2a8150e04f36fb9d499fbb6495244ea "

inherit autotools pkgconfig