require nix.inc

SRC_URI = "git://github.com/NixOS/nix.git;branch=master;protcol=http \
           file://0001-No-documentation.patch \
           file://0002-Force-cast.patch \
           file://0003-Yocto-bundles-library-into-one-file.patch \
           file://0004-no-allocators-if-don-t-HAVE_BOEHMGC.patch \
           file://0005-Install-systemd-files-to-yocto-s-folder.patch \
           file://0006-Don-t-split-nix-into-shared-libs.patch \
           "

SRCREV="d5d4d980427aca3849b90bfe1694b6d1d14532fb"

SRC_URI[sha256sum] = "809d9270716a8e8727a0bffafc08dc81a3bc87cc3aed9ad07ddbecef8d4bb83e"

LIC_FILES_CHKSUM = "file://COPYING;md5=fbc093901857fcd118f065f900982c24 "

# Set S because we are using git not an extracted tarball
S = "${WORKDIR}/git"

# .SO libs are for runtime not dev
FILES_SOLIBSDEV="" 
FILES:${PN}-zsh = " ${datadir_native}/zsh/site-functions/_nix "
FILES:${PN}-bash = " ${datadir_native}/bash-completion/completions/nix "
FILES:${PN}-fish = " ${datadir_native}/fish/vendor_completions.d/nix.fish "
FILES:${PN} += " ${systemd_system_unitdir}/nix-daemon.service ${systemd_system_unitdir}/nix-daemon.socket "

# Should probably move these to the arch libdir
FILES:${PN} += " ${nonarch_libdir}/*.so "

PACKAGE_BEFORE_PN = " ${PN}-zsh ${PN}-bash ${PN}-fish "
