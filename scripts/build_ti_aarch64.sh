#!/usr/bin/env bash

# This script builds the project for the TI J721S2 SoC platform using the
# aarch64-linux-gnu-gcc cross-compiler, provided by the TI Processor SDK.

set -euo pipefail

if [ -z "${TI_SDK_PREFIX:-}" ]; then
    echo "TI_SDK_PREFIX is not set"
    exit 1
fi

# Ensure that we are in the root directory of the project
cd "$(dirname "$0")/.."

source "$TI_SDK_PREFIX/linux-devkit/environment-setup-aarch64-oe-linux"

cmake -S . -B build -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build
