#!/bin/bash
function usage {
    echo "usage: $0 [-h|-d|-s]  [<hex0> <hex1> <hex2> <hex3>]"
    echo "-h    show help"
    echo "-s    save default colors"
    echo "-d    load default colors"
    exit 1
}

function save_default {
    colorfile="~/.rgb_default/rgb_default.txt"
    echo "" > $colorfile
    echo $1 >> "$colorfile\n"
    echo $2 >> "$colorfile\n"
    echo $3 >> "$colorfile\n"
    echo $4 >> "$colorfile\n"
}

function load_default {
    echo "default keyboard colors loaded"
    exit 0
}

while getopts ":sd" o; do
    case "${o}" in
        s)
            save_default $1 $2 $3 $4;;
        d)
            load_default ;;
        *)
            usage ;;
    esac
    shift "$((OPTIND - 1))"
done

# check the input arguments
[ "$#" == "4" ] || usage

# changing the keyboard colors
sudo bash -c "echo $1 > /sys/devices/platform/hp-wmi/rgb_zones/zone00"
sudo bash -c "echo $2 > /sys/devices/platform/hp-wmi/rgb_zones/zone01"
sudo bash -c "echo $3 > /sys/devices/platform/hp-wmi/rgb_zones/zone02"
sudo bash -c "echo $4 > /sys/devices/platform/hp-wmi/rgb_zones/zone03"