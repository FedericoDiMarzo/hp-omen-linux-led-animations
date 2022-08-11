#!/bin/bash
function usage {
    echo "usage: $0 [-h|-d|-s]  [<hex0> <hex1> <hex2> <hex3>]"
    echo "-h    show help"
    echo "-s    save default colors"
    echo "-d    load default colors"
    exit 1
}

# dirs
colordir="$HOME/.rgb_default"
colorfile="$colordir/rgb_default.txt"


function save_default {
    mkdir -p "$colordir"
    echo $2 > "$colorfile"
    echo $3 >> "$colorfile"
    echo $1 >> "$colorfile"
    echo $4 >> "$colorfile"
    echo "default keyboard color configuration saved"
}

function load_default {
    if [ -f "$colorfile" ]; then
    count=0
        for color in $(cat "$colorfile"); do
            sudo bash -c "echo $color > /sys/devices/platform/hp-wmi/rgb_zones/zone0$count"
            count=$(($count+1))
        done
        echo "default keyboard color configuration loaded"
    else
        echo "no default configuration was found"
    fi
    exit 0
}

while getopts ":sd" o; do
    case "${o}" in
        s)
            save_default $2 $3 $4 $5;;
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
