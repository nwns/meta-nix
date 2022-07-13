#!/bin/sh

# This script installs the Nix package manager on your system by
# downloading a binary distribution and running its installer script
# (which in turn creates and populates /nix).

{ # Prevent execution if this script was only partially downloaded
oops() {
    echo "$0:" "$@" >&2
    exit 1
}

umask 0022

tmpDir="$(mktemp -d -t nix-binary-tarball-unpack.XXXXXXXXXX || \
          oops "Can't create temporary directory for downloading the Nix binary tarball")"
cleanup() {
    rm -rf "$tmpDir"
}
trap cleanup EXIT INT QUIT TERM

require_util() {
    command -v "$1" > /dev/null 2>&1 ||
        oops "you do not have '$1' installed, which I need to $2"
}

case "$(uname -s).$(uname -m)" in
    Linux.x86_64)
        hash=92c0028e408414ddce00c4c8d92b51a0e9b32e460abb636cf5a968a4f345fbef
        path=7wrahzk0cqx1pvlcwh890r7vbacc07sb/nix-2.10.0-x86_64-linux.tar.xz
        system=x86_64-linux
        ;;
    Linux.i?86)
        hash=5229c4062cf100f8a926736335eda2c14adc4628568f2cbe4713eab2ef2677a7
        path=6x6xxk9vj22rhimr4faaw0vzxkzy8cmv/nix-2.10.0-i686-linux.tar.xz
        system=i686-linux
        ;;
    Linux.aarch64)
        hash=37a689456fb71425d1bcf4d7c9b324fa44518f0cd5f9d2b6778d9b377b240ebb
        path=kp4nn4031prxk55pinp460kzxq3qws0b/nix-2.10.0-aarch64-linux.tar.xz
        system=aarch64-linux
        ;;
    Linux.armv6l_linux)
        hash=9698929fa8355e38e015e74bc02d1fb7f0c642be21c290d0e39cebe7349b6e32
        path=6xd1d5fpam9s6c6bf3j5k6nhwjpixd2g/nix-2.10.0-armv6l-linux.tar.xz
        system=armv6l-linux
        ;;
    Linux.armv7l_linux)
        hash=8b4d75548d9c01c8e5ef7894cb8240e6c83bad85d2e325aabf39dedb01836119
        path=2xi1crv3wq5w42zklc6j2ii8pg2dahvm/nix-2.10.0-armv7l-linux.tar.xz
        system=armv7l-linux
        ;;
    Darwin.x86_64)
        hash=5682f717f94cf037e5f8fcede5a86555cdc68303e195740672e854fc7719aa3c
        path=dbrikgmzwxjw4n690wjbbb732q4w8fxs/nix-2.10.0-x86_64-darwin.tar.xz
        system=x86_64-darwin
        ;;
    Darwin.arm64|Darwin.aarch64)
        hash=1ba4b3c2f4358dd48dd3263ab918bdd438da7039848981c8abcbc298d0fed370
        path=wq0xaymm1bk9gxadf9mf3xb1kiny6dmi/nix-2.10.0-aarch64-darwin.tar.xz
        system=aarch64-darwin
        ;;
    *) oops "sorry, there is no binary distribution of Nix for your platform";;
esac

# Use this command-line option to fetch the tarballs using nar-serve or Cachix
if [ "${1:-}" = "--tarball-url-prefix" ]; then
    if [ -z "${2:-}" ]; then
        oops "missing argument for --tarball-url-prefix"
    fi
    url=${2}/${path}
    shift 2
else
    url=https://releases.nixos.org/nix/nix-2.10.0/nix-2.10.0-$system.tar.xz
fi

tarball=$tmpDir/nix-2.10.0-$system.tar.xz

require_util tar "unpack the binary tarball"
if [ "$(uname -s)" != "Darwin" ]; then
    require_util xz "unpack the binary tarball"
fi

if command -v curl > /dev/null 2>&1; then
    fetch() { curl --fail -L "$1" -o "$2"; }
elif command -v wget > /dev/null 2>&1; then
    fetch() { wget "$1" -O "$2"; }
else
    oops "you don't have wget or curl installed, which I need to download the binary tarball"
fi

echo "downloading Nix 2.10.0 binary tarball for $system from '$url' to '$tmpDir'..."
fetch "$url" "$tarball" || oops "failed to download '$url'"

if command -v sha256sum > /dev/null 2>&1; then
    hash2="$(sha256sum -b "$tarball" | cut -c1-64)"
elif command -v shasum > /dev/null 2>&1; then
    hash2="$(shasum -a 256 -b "$tarball" | cut -c1-64)"
elif command -v openssl > /dev/null 2>&1; then
    hash2="$(openssl dgst -r -sha256 "$tarball" | cut -c1-64)"
else
    oops "cannot verify the SHA-256 hash of '$url'; you need one of 'shasum', 'sha256sum', or 'openssl'"
fi

if [ "$hash" != "$hash2" ]; then
    oops "SHA-256 hash mismatch in '$url'; expected $hash, got $hash2"
fi

unpack=$tmpDir/unpack
mkdir -p "$unpack"
tar -xJf "$tarball" -C "$unpack" || oops "failed to unpack '$url'"

script=$(echo "$unpack"/*/install)

[ -e "$script" ] || oops "installation script is missing from the binary tarball!"
export INVOKED_FROM_INSTALL_IN=1
"$script" "$@"

} # End of wrapping
