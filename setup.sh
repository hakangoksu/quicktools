#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

CACHE_DIR="$HOME/.cache"
PKG_DIR="$CACHE_DIR/quicktools_build"
REPO_URL="https://github.com/hakangoksu/quicktools.git"

echo -e "${GREEN}### QuickTools Setup ###${NC}"

if ! command -v pacman &> /dev/null; then
    echo -e "${RED}Error: This script only works on Arch Linux based systems.${NC}"
    exit 1
fi

echo "Checking dependencies..."
if ! command -v git &> /dev/null || ! command -v makepkg &> /dev/null; then
    echo "Installing base-devel and git..."
    sudo pacman -S --needed base-devel git --noconfirm
fi

mkdir -p "$CACHE_DIR"

if [ -d "$PKG_DIR" ]; then
    echo "Cleaning up existing build directory: $PKG_DIR"
    rm -rf "$PKG_DIR"
fi

echo "Cloning repository into $PKG_DIR..."
git clone "$REPO_URL" "$PKG_DIR"
cd "$PKG_DIR"

echo "Cleaning up files within $PKG_DIR..."
rm -rf pkg/ src/ *.pkg.tar.zst

echo -e "${GREEN}Building and installing...${NC}"
makepkg -si --noconfirm

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✔ Setup complete!${NC}"
    echo "Run 'quicktools' to start."
else
    echo -e "${RED}✖ Setup failed.${NC}"
    exit 1
fi
