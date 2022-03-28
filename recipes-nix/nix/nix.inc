SUMMARY = "The Nix Package Manager"
DESCRIPTION = "Nix, the purely functional package manager"
LICENSE = "LGPLv2.1"

DEPENDS:append = " \
autoconf-archive-native \
automake-native \
gnulib-native \
bash \
bison-native \
boost \
brotli \
bzip2 \
curl \
editline \
flex \
tar \
gzip \
jq-native \
zlib \
libarchive \
libsodium \
nlohmann-json \
openssl \
perl \
pkgconfig \
sqlite \
xz \
googletest \
lowdown-native \
lowdown \
"

#TODO(nigel) fix out of tree autotools
inherit autotools-brokensep pkgconfig useradd systemd

# TODO(nigel) disable bdwgc since it is optional and they seem to have moved the includes around
# between 8.0.6 and master
CONFIGUREOPTS:append = " --disable-cpuid --disable-gc "

FILES:${PN}-zsh = " ${datadir_native}/zsh/site-functions/_nix "
FILES:${PN}-bash = " ${datadir_native}/bash-completion/completions/nix "
FILES:${PN}-fish = " ${datadir_native}/fish/vendor_completions.d/nix.fish "
FILES:${PN} += " ${systemd_system_unitdir}/nix-daemon.service ${systemd_system_unitdir}/nix-daemon.socket "
FILES:${PN} += " /nix/* "
CONFFILES:${PN} = "${sysconfdir}/nix/nix.conf"

PACKAGE_BEFORE_PN = " ${PN}-zsh ${PN}-bash ${PN}-fish "

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "nix-daemon.service nix-daemon.socket"

USERADD_PACKAGES = "nix"

GROUPADD_PARAM:${PN} = "-g 30000 nixbld"
USERADD_PARAM:${PN} = " \
    -u 30001 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld1; \
    -u 30002 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld2; \
    -u 30003 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld3; \
    -u 30004 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld4; \
    -u 30005 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld5; \
    -u 30006 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld6; \
    -u 30007 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld7; \
    -u 30008 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld8; \
    -u 30009 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld9; \
    -u 30010 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld10; \
    -u 30011 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld11; \
    -u 30012 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld12; \
    -u 30013 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld13; \
    -u 30014 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld14; \
    -u 30015 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld15; \
    -u 30016 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld16; \
    -u 30017 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld17; \
    -u 30018 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld18; \
    -u 30019 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld19; \
    -u 30020 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld20; \
    -u 30021 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld21; \
    -u 30022 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld22; \
    -u 30023 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld23; \
    -u 30024 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld24; \
    -u 30025 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld25; \
    -u 30026 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld26; \
    -u 30027 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld27; \
    -u 30028 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld28; \
    -u 30029 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld29; \
    -u 30030 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld30; \
    -u 30031 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld31; \
    -u 30032 -d /var/empty -s /sbin/nologin -g nixbld -G nixbld nixbld32; \
    "

do_install:append() {
    install -dv -m 0755 ${D}nix ${D}/nix/var ${D}/nix/var/log ${D}/nix/var/log/nix
    install -dv -m 0755 ${D}/nix/var/log/nix/drvs 
    install -dv -m 0755 ${D}/nix/var/nix/db ${D}/nix/var/nix/gcroots
    install -dv -m 0755 ${D}/nix/var/nix/profiles ${D}/nix/var/nix/temproots
    install -dv -m 0755 ${D}/nix/var/nix/userpool ${D}/nix/var/nix/daemon-socket
    install -dv -m 0755 ${D}/nix/var/nix/gcroots/per-user ${D}/nix/var/nix/profiles/per-user
    install -dv -g nixbld -m 1775 ${D}/nix/store
    install -dv -m 0555 ${D}/etc/nix

    cat <<EOF >${WORKDIR}/nix.conf
build-users-group = nixbld
EOF
    install -m 0664 "${WORKDIR}/nix.conf" ${D}/etc/nix/nix.conf
}
