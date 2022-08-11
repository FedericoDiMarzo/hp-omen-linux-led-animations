SCRIPT_ORIG=scripts/rgb_keyboard.sh
SCRIPT_DEST=/bin/rgb_keyboard

install: 
	cp $(SCRIPT_ORIG) $(SCRIPT_DEST)
	chmod +x $(SCRIPT_DEST)
	gcc -o src/rgb_animation src/rgb_animation.c
	# dkms install .

uninstall:
	rm -rf $(SCRIPT_DEST) $(RGB_DEFAULT)
	# dkms remove hp-omen-wmi/0.9 --all

all: install rgb_animation

