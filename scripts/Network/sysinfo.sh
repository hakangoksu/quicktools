#!/bin/bash
# Name: System Info
# Description: Shows system kernel and uptime
echo "--- SYSTEM INFORMATION ---"
echo "Kernel: $(uname -sr)"
echo "Uptime: $(uptime -p)"
echo "Host:   $HOSTNAME"
echo "--------------------------"
read -p "Press Enter to close..."
