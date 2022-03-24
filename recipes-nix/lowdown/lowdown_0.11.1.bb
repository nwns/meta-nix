SUMMARY = "lowdown — simple markdown translator"
DESCRIPTION = "lowdown — simple markdown translator"
LICENSE = "ISC"

DEPENDS:append = " pkgconfig \
"

SRC_URI = "https://kristaps.bsd.lv/lowdown/snapshots/lowdown-${PV}.tar.gz \
           file://0001-Don-t-use-the-full-paths-in-the-.pc.patch \
           "

SRC_URI[sha256sum] = "0dec4ef8d51d1fcbe3333009e01b916b637cc83194c6ed0bdfe944646518690f"

LIC_FILES_CHKSUM = "file://LICENSE.md;md5=ce605be8688d664dcc3829fa42783197 "


FILES:${PN} += "${nonarch_libdir}/cleargrid/macaddress.bash"

do_configure () {
    ./configure PREFIX=${D}${prefix} "LDFLAGS=${LDFLAGS}"
}

do_build() {
    make
    make regress
}

do_install() {
    set
    make install install_libs
    ls
    rm -r ${D}${prefix}/man # TODO(nigel) put docs into seprate package
}
