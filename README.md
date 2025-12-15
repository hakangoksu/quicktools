# QuickTools Script Manager

**QuickTools** is a lightweight, minimalist GTK4-based script launcher designed for Arch Linux. It dynamically reads shell scripts from a directory structure and presents them in a clean, categorized GUI with a fixed sidebar layout.

## Features

- **GTK4 & Libadwaita:** Modern and native Linux look.
- **Dynamic Categories:** Folders in the scripts directory automatically become categories.
- **Smart Metadata:** Reads `# Name:` and `# Description:` tags directly from script comments.
- **Terminal Awareness:** Automatically detects your installed terminal.
- **Fixed Sidebar:** The sidebar maintains its width while resizing the window.

## Installation

### The Quick Way (Recommended)
Run this single command in your terminal:

```bash
bash <(curl -s https://raw.githubusercontent.com/hakangoksu/quicktools/main/setup.sh)
