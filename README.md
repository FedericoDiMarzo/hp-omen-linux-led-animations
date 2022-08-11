HP Omen keyboard color animation
-----------------------------------------

This software enables LED keyboard animations for HP Omen PCs in Linux. In order to communicate with the hardware and change the RGB level of the different LED zones of the keyboard, this project forks from the Linux porting of the hp-omen drivers. As stated in the original repository, the drivers are still *experimental*, not widely tested, and could crash your machine. **You should thus use them at your own risk**.

The software was tested on my personal machine with the following configuration:
* machine: HP OMEN Laptop 15-ek
* os: Pop!_OS 22.04 LTS

*THE REPOSITORY IS A WORK IN PROGRESS*


## Installation

To install the module you should first install dkms and kernel headers if needed (already present on Ubuntu).
After doing that you can run 
```
sudo make install
```

Module will be built and installed, and DKMS will manage rebuilding it on kernel updates.
If you wish to uninstall the module you can execute
```
sudo make uninstall
```

## Basic usage
To change the color zone of the keyboard statically, you can pass the hex code of the different zones as follows
```
rgb_keyboard <hex0> <hex1> <hex2> <hex3>
```
Pro tip: you can find nice hex-ready color palettes [on this website](https://coolors.co/palettes/trending).

Save and load your favorite color configuration using the `-s` and `-d` options. The configuration will be saved in `~/rgb-zones/rgb_default.txt`.
```
# saves the default color setting 
rgb_keyboard -s <hex0> <hex1> <hex2> <hex3>

# loads the default color setting
rgb_keyboard -d
```

