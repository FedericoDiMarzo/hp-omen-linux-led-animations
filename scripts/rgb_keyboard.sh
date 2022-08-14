#!/bin/bash

# paths
colordir="$HOME/.rgb_keyboard"
colorfile="$colordir/rgb_default"
daemonfile="/var/lib/rgb_keyboard/daemon_pid"

function usage {
    echo "usage: $0 [-h|-d|-s] [-a stop|<animation-type> <animation-speed>] [<hex0> <hex1> <hex2> <hex3>]"
    echo "-h    show help"
    echo "-s    save default colors"
    echo "-d    load default colors"
    echo "-a    stop or start a keyboard rgb animation"
    exit 1
}

function save_default {
    sudo mkdir -p "$colordir"
    echo $1 > "$colorfile"
    echo $2 >> "$colorfile"
    echo $3 >> "$colorfile"
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

function animations {
    [ -f "$colorfile" ] || $(echo "save default colors first" ; exit 1)

    case $1 in
        "stop")
            if [ -f "$daemonfile" ]; then
                sudo kill $(cat "$daemonfile")
                sudo rm "$daemonfile"
                echo "keyboard rgb animation stopped"
                rgb_keyboard -d
            else
                echo "no keyboard rgb animation running"
            fi ;;
        "steps")
            sudo _rgb_keyboard_animation $1 $2 ;;
        *)
            echo "the following keyboard rgb animations are supported:"
            echo "steps" ;;
    esac
    exit 0
}

while getopts ":sda:" o; do
    case "${o}" in
        s)
            save_default $2 $3 $4 $5;;
        d)
            load_default ;;
        a) 
            animations ${OPTARG};;
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
