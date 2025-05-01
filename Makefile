# Variables
SDL_DIR := SDL
VVVVVV_DIR := VVVVVV/desktop_version
DATA_ZIP_URL := https://thelettervsixtim.es/makeandplay/data.zip
BUILD_DIR := build
NPROC := $(shell nproc)
USE_WAYLAND := ON

# Targets
.PHONY: all sdl vvvvvv

all: vvvvvv

install_dependecies: git_submodule sdl2
	@echo "Dependencies installed."

git_submodule:
	git submodule update --init --recursive

sdl2:
	mkdir -p $(SDL_DIR)/$(BUILD_DIR)
	cd $(SDL_DIR)/$(BUILD_DIR) && cmake .. -DSDL_WAYLAND=$(USE_WAYLAND) && make -j $(NPROC)
	sudo make -C $(SDL_DIR)/$(BUILD_DIR) install

vvvvvv:
	mkdir -p $(VVVVVV_DIR)/$(BUILD_DIR)
	cd $(VVVVVV_DIR)/$(BUILD_DIR) && cmake .. && make -j $(NPROC)
	wget -O $(VVVVVV_DIR)/$(BUILD_DIR)/data.zip $(DATA_ZIP_URL)

launch_game:
	cd $(VVVVVV_DIR)/$(BUILD_DIR) && ./VVVVVV

clean:
	rm -rf $(SDL_DIR)/$(BUILD_DIR) $(VVVVVV_DIR)/$(BUILD_DIR)
