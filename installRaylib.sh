#!/usr/bin/env bash

tmpDir=$(mktemp -d)

if [[ ! -d "$tmpDir" ]]; then
    echo "Failed to create temporary directory."
    exit 1
fi

pushd "$tmpDir" || exit 1

git clone https://github.com/raysan5/raylib.git
cd raylib || exit

RL_INSTALL_DIR="/opt/raylib"

cmake -S . -B buildRelease -G Ninja \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_POSITION_INDEPENDENT_CODE=ON \
-DBUILD_SHARED_LIBS=OFF \
-DCMAKE_INSTALL_PREFIX="$RL_INSTALL_DIR"

sudo cmake --build buildRelease --target install --verbose

popd || exit 1