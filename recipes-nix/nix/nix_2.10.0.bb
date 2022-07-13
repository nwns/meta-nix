require nix.inc

python () {
    target_sys = d.getVar('TARGET_SYS')
    pn = d.getVar('PN')
    pv = d.getVar('PV')
    workdir = d.getVar('WORKDIR')
    if target_sys.startswith("x86_64"):
        d.appendVar("SRC_URI",
         " https://releases.nixos.org/nix/nix-2.10.0/nix-2.10.0-x86_64-linux.tar.xz;sha256sum=92c0028e408414ddce00c4c8d92b51a0e9b32e460abb636cf5a968a4f345fbef")
        # extracted tarball has folder of same name
        d.setVar('S', f"{workdir}/{pn}-{pv}-x86_64-linux")
    else:
        bb.fatal("Unknown target system", target_sys)
}

LIC_FILES_CHKSUM = "file://.reginfo;md5=59002385c4e4ea47e7f7a2816f96ebe3 "


