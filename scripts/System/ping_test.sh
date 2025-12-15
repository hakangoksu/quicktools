#!/bin/bash
# Description: Pings Google DNS
echo "Pinging 8.8.8.8 (Google DNS)..."
ping -c 4 8.8.8.8
echo "--------------------------"
read -p "Press Enter to close..."
