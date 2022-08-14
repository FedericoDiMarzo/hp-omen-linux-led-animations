TARGET_NAME := _rgb_keyboard_animation

# tool macros
CC ?= gcc
CCFLAGS :=
DBGFLAGS := -g 
CCOBJFLAGS := $(CCFLAGS) -c
CCOPTIMIZATION := -O3
LIBFLAGS := -lm

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug
SCRIPT_ORIG := scripts/rgb_keyboard.sh
SCRIPT_DEST := /bin/rgb_keyboard
VAR_DATA_PATH := /var/lib/rgb_keyboard
TARGET_ORIG := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEST := /bin/$(TARGET_NAME)


# compile macros
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CCFLAGS) $(CCOPTIMIZATION) $(LIBFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $< 

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@ $(LIBFLAGS)

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)

install: all
	mkdir $(VAR_DATA_PATH)
	cp $(SCRIPT_ORIG) $(SCRIPT_DEST)
	cp $(TARGET_ORIG) $(TARGET_DEST)
	chmod +x $(SCRIPT_DEST)
	# dkms install .

uninstall: clean
	rm -rf $(TARGET_DEST) $(SCRIPT_DEST) $(RGB_DEFAULT) $(VAR_DATA_PATH)
	# dkms remove hp-omen-wmi/0.9 --all