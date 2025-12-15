# QuickTools Script Manager

**QuickTools** is a lightweight, minimalist GTK4-based script launcher designed for Arch Linux. It dynamically reads shell scripts from a directory structure and presents them in a clean, categorized GUI with a fixed sidebar layout.

## Features

* **GTK4 & Libadwaita** – Modern, native Linux look and feel
* **Dynamic Categories** – Folders automatically become categories
* **Smart Metadata** – Reads `# Name:` and `# Description:` directly from script comments
* **Terminal Awareness** – Automatically detects your installed terminal (Kitty, Alacritty, GNOME Terminal, etc.)
* **Fixed Sidebar** – Sidebar maintains its width while resizing the window

## Installation

Run the following command in your terminal:

```bash
curl -s https://raw.githubusercontent.com/hakangoksu/quicktools/main/setup.sh | bash
```

The installer will:

* Install required dependencies
* Clone the repository
* Build the project
* Install it as a pacman package

## Updating

QuickTools can be updated via itself o repeating of the installiation script.

## Uninstallation

Since QuickTools is installed via **pacman**, you can remove it cleanly using:

```bash
sudo pacman -Rns quicktools-git
```

## How to Add Scripts

QuickTools looks for scripts in:

```text
/usr/share/quicktools/scripts/
```

### 1. Create a Category

Create a new folder inside the scripts directory. Each folder becomes a category in the UI.

```bash
/usr/share/quicktools/scripts/System/
```

### 2. Add a Script

Place your `.sh` file inside the category folder.

### 3. Add Metadata (Optional but Recommended)

Add `# Name:` and `# Description:` comments at the top of your script to improve how it appears in the UI.

### Example Script

```bash
#!/bin/bash
# Name: System Update
# Description: Full system update using pacman.

echo "Updating system..."
sudo pacman -Syu
read -p "Press Enter to exit..."
```

## Dependencies

* `gtk4`
* `libadwaita`
* `base-devel` (installed automatically during setup)

## License

MIT License
