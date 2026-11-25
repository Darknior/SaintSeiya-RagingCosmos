#!/bin/sh
set -eu
cd -- "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
chmod +x ./OpenBOR-Linux-x64.AppImage
export APPIMAGE_EXTRACT_AND_RUN=1
exec ./OpenBOR-Linux-x64.AppImage
