#!/bin/bash
set -e

APP_DIR="$(pwd)"
KERNEL_DIR="$APP_DIR/kernel-fork"
LIB_SUBDIR="target/x86_64/debug"

cd "$KERNEL_DIR"
cargo xtask build --arch x86_64 --features "bga pc-keyboard mman"

cd "$APP_DIR"
mkdir -p build
if [ ! -f build/hermit-loader-x86_64-multiboot ]; then
  curl -Lo build/hermit-loader-x86_64-multiboot https://github.com/hermit-os/loader/releases/latest/download/hermit-loader-x86_64-multiboot
fi
docker run --rm \
  -v "$APP_DIR:/mnt" \
  -w /mnt \
  ghcr.io/hermit-os/hermit-gcc:x86_64 \
  make
if [ "$(uname)" == "Darwin" ]; then
    QEMU_DISPLAY="cocoa,zoom-to-fit=on"
else
    QEMU_DISPLAY="sdl"
fi

cd "$APP_DIR"
qemu-system-x86_64 \
  -m 128M \
  -serial stdio \
  -display $QEMU_DISPLAY \
  -vga std \
  -cpu qemu64,+fsgsbase \
  -kernel build/hermit-loader-x86_64-multiboot \
  -initrd doom-hermit \
  -no-reboot \
    -s
