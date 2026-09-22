#!/bin/bash

# Ensure script is run with root privileges
if [ "$EUID" -ne 0 ]; then
  echo "[-] Please run this script with sudo: sudo ./setup_network.sh"
  exit 1
fi

echo "=========================================="
echo "   Raspberry Pi Direct Ethernet Setup     "
echo "=========================================="
echo "1. Server (IP: 192.168.2.1)"
echo "2. Client (IP: 192.168.2.2)"
read -p "Select the role for THIS Raspberry Pi [1 or 2]: " ROLE

# Automatically detect the wired ethernet interface (e.g., eth0 or enxb8...)
INTERFACE=$(ip -o link show | awk -F': ' '{print $2}' | grep -E '^(eth|en)' | head -n 1)

if [ -z "$INTERFACE" ]; then
  echo "[-] Error: No Ethernet interface found. Please check your cable connection."
  exit 1
fi

echo "[+] Found Ethernet interface: $INTERFACE"

# Find or create the NetworkManager connection profile for this interface
CON_NAME=$(nmcli -t -f NAME,DEVICE connection show | grep "$INTERFACE" | cut -d: -f1)

if [ -z "$CON_NAME" ]; then
  CON_NAME="Wired connection 1"
  echo "[*] Creating new NetworkManager profile '$CON_NAME'..."
  nmcli connection add type ethernet ifname "$INTERFACE" con-name "$CON_NAME"
else
  echo "[+] Found existing connection profile: $CON_NAME"
fi

# Set IP variables based on role selection
if [ "$ROLE" == "1" ]; then
    IP="192.168.2.1/24"
    TARGET="192.168.2.2"
    echo "[*] Configuring Pi as SERVER ($IP)..."
elif [ "$ROLE" == "2" ]; then
    IP="192.168.2.2/24"
    TARGET="192.168.2.1"
    echo "[*] Configuring Pi as CLIENT ($IP)..."
else
    echo "[-] Invalid selection. Please restart the script and choose 1 or 2."
    exit 1
fi

# Apply static IP settings
nmcli connection modify "$CON_NAME" ipv4.addresses "$IP"
nmcli connection modify "$CON_NAME" ipv4.method manual
nmcli connection up "$CON_NAME"

echo "[+] Static IP configuration applied successfully!"
echo "[*] Waiting 3 seconds for the link to stabilize..."
sleep 3

# Run Ping Test
echo "[*] Running ping test to peer ($TARGET)..."
if ping -c 3 "$TARGET" > /dev/null 2>&1; then
    echo ""
    echo "=========================================="
    echo " [SUCCESS] Ping to $TARGET successful!    "
    echo " Network is fully configured and ready.   "
    echo "=========================================="
else
    echo ""
    echo "=========================================="
    echo " [WARNING] Ping to $TARGET failed.        "
    echo " Check if the other Pi is powered on and  "
    echo " has its script executed as the opposite  "
    echo " role.                                    "
    echo "=========================================="
fi
