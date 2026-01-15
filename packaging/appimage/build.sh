#!/bin/bash
set -e

# Configurações
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="/workspace/showbox-studio"
SHOWBOX_ROOT="/workspace/showbox"
BUILD_DIR="$PROJECT_ROOT/build-pkg"
DIST_DIR="$PROJECT_ROOT/dist"
APPDIR="$BUILD_DIR/AppDir"

echo "=== Showbox Studio AppImage Builder ==="

# 1. Preparar Diretórios
rm -rf "$BUILD_DIR" "$APPDIR"
mkdir -p "$BUILD_DIR" "$APPDIR" "$DIST_DIR"

# 2. Compilar
echo "[1/4] Compiling Showbox Studio..."
cd "$BUILD_DIR"
cmake "$PROJECT_ROOT" \
    -DSHOWBOX_ROOT="$SHOWBOX_ROOT" \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=Release

make -j$(nproc)

# 3. Instalar no AppDir
echo "[2/4] Installing to AppDir..."
# Instalação manual simplificada (já que não definimos 'install' no CMake ainda)
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/share/applications"
mkdir -p "$APPDIR/usr/share/icons/hicolor/scalable/apps"

cp "$BUILD_DIR/showbox-studio" "$APPDIR/usr/bin/"
cp "$SCRIPT_DIR/showbox-studio.desktop" "$APPDIR/usr/share/applications/"
cp "$SCRIPT_DIR/showbox-studio.svg" "$APPDIR/usr/share/icons/hicolor/scalable/apps/showbox-studio.svg"

# 4. Baixar LinuxDeploy
echo "[3/4] Downloading deployment tools..."
cd "$BUILD_DIR"
wget -q -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -q -nc https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy*.AppImage

# 5. Gerar AppImage
echo "[4/4] Generating AppImage..."
export VERSION="0.1.0"
export QMAKE=/usr/bin/qmake6

./linuxdeploy-x86_64.AppImage \
    --appdir "$APPDIR" \
    --plugin qt \
    --output appimage \
    --icon-file "$APPDIR/usr/share/icons/hicolor/scalable/apps/showbox-studio.svg" \
    --desktop-file "$APPDIR/usr/share/applications/showbox-studio.desktop"

# Mover para dist
mv Showbox_Studio*.AppImage "$DIST_DIR/"

echo "=== Success! Package available in dist/ ==="
ls -lh "$DIST_DIR"
