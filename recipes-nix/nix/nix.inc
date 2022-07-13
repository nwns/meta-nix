SUMMARY = "The Nix Package Manager"
DESCRIPTION = "Nix, the purely functional package manager"
LICENSE = "LGPLv2.1"

DEPENDS:append = " \
tar \
xz \
"

#TODO(nigel) fix out of tree autotools
inherit base useradd systemd


FILES:${PN}-zsh = " ${datadir_native}/zsh/site-functions/_nix "
FILES:${PN}-bash = " ${datadir_native}/bash-completion/completions/nix "
FILES:${PN}-fish = " ${datadir_native}/fish/vendor_completions.d/nix.fish "
FILES:${PN} += " ${systemd_system_unitdir}/nix-daemon.service ${systemd_system_unitdir}/nix-daemon.socket "
FILES:${PN} += " /nix/* /root/.nix-channels /usr/share/.reginfo"
CONFFILES:${PN} = "${sysconfdir}/nix/nix.conf"

SRC_URI += "file://firstrun-nix file://firstrun-nix.service"

PACKAGE_BEFORE_PN = " ${PN}-zsh ${PN}-bash ${PN}-fish "

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "nix-daemon.service nix-daemon.socket firstrun-nix.service"

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
    NIX_INSTALLED_NIX=$(find ${S} -name '*-nix-*')
    # Make basic directory structure of Nix (part 1)
    install -dv -m 0755 ${D}nix ${D}/nix/var ${D}/nix/var/log ${D}/nix/var/log/nix
    install -dv -m 0755 ${D}/nix/var/log/nix/drvs 
    install -dv -m 0755 ${D}/nix/var/nix/db ${D}/nix/var/nix/gcroots
    install -dv -m 0755 ${D}/nix/var/nix/profiles ${D}/nix/var/nix/temproots
    install -dv -m 0755 ${D}/nix/var/nix/userpool ${D}/nix/var/nix/daemon-socket
    install -dv -m 0755 ${D}/nix/var/nix/gcroots/per-user ${D}/nix/var/nix/profiles/per-user
    # Make basic directory structure of Nix (part 2)
    install -dv -g nixbld -m 1775 ${D}/nix/store
    # Make place the default nix daemon configuration (part 1)
    install -dv -m 0555 ${D}/etc/nix

    # place_channel_configuration
    echo "https://nixos.org/channels/nixos-unstable nixpkgs" > ${WORKDIR}/.nix-channels
    install -dv -m 0755 ${D}${ROOT_HOME}
    install -m 0664 "${WORKDIR}/.nix-channels" "${D}${ROOT_HOME}/.nix-channels"

    # First run tasks for nix are in the firstrun-nix service

    # install_from_extracted_nix
    cp -RPp ./store/* ${D}/nix/store/
    chmod -R ugo-w ${D}/nix/store/
    chown root:root -R ${D}/nix/store/

    cat <<EOF >${WORKDIR}/nix.sh
# Nix
if [ -e /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh ]; then
  . /nix/var/nix/profiles/default/etc/profile.d/nix-daemon.sh
fi
# End Nix
EOF
    install -m 0664 -D -t ${D}${sysconfdir}/profile.d ${WORKDIR}/nix.sh
    install -m 0664 -D -t ${D}${systemd_system_unitdir} \
        ${NIX_INSTALLED_NIX}/lib/systemd/system/nix-daemon.service \
        ${NIX_INSTALLED_NIX}/lib/systemd/system/nix-daemon.socket

    install -m 0644 -D -t ${D}${systemd_unitdir}/system ${WORKDIR}/firstrun-nix.service
    install -m 0755 -D -t ${D}${libexecdir} ${WORKDIR}/firstrun-nix
    install -m 0644 -D ${S}/.reginfo ${D}${datadir}/nix/reginfo


    #TODO(nwns) Do we need to configure systemd tempfiles?
    # /etc/tmpfiles.d/nix-daemon.conf

    cat <<EOF >${WORKDIR}/nix.conf
build-users-group = nixbld
EOF
    install -m 0664 "${WORKDIR}/nix.conf" ${D}/etc/nix/nix.conf
}


INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
do_package_qa[noexec] = "1"
EXCLUDE_FROM_SHLIBS = "1"
