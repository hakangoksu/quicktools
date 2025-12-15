#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}### QuickTools Setup ###${NC}"

if ! command -v pacman &> /dev/null; then
    echo -e "${RED}Error: This script only works on Arch Linux based systems.${NC}"
    exit 1
fi

echo "Updating system..."
sudo pacman -Syu --noconfirm

echo "Checking dependencies..."
if ! command -v git &> /dev/null || ! command -v makepkg &> /dev/null; then
    echo "Installing base-devel and git..."
    sudo pacman -S --needed base-devel git --noconfirm
fi

if [ -f "PKGBUILD" ]; then
    echo "PKGBUILD found in current directory."
else
    if [ -d "quicktools" ]; then
        echo "Directory 'quicktools' exists. Updating..."
        cd quicktools
        git pull
    else
        echo "Cloning repository..."
        git clone https://github.com/hakangoksu/quicktools.git
        cd quicktools
    fi
fi

echo "Cleaning up..."
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
