# Install SDL2 dependency
SDL_SOURCE_DIR := .sdl
SDL_GIT_URL := https://github.com/libsdl-org/SDL.git
SDL_GIT_BRANCH := release-2.24.0
SDL_WAYLAND := ON

# If the README.md file is not present, it means the submodule is not initialized
VVVVVV_SOURCE_DIR := src/cpp/VVVVVV/README.md

# This file is necessary for the game to run
DATA_ZIP := data.zip
DATA_ZIP_URL := https://thelettervsixtim.es/makeandplay/data.zip

CPP_SRC := src/cpp
BUILD_DIR := build
NPROC := $(shell nproc)

# Targets
.PHONY: build .dependencies

all: build

build: .dependecies
	@mkdir -p $(CPP_SRC)/$(BUILD_DIR)
	@cd $(CPP_SRC)/$(BUILD_DIR) && cmake .. && make -j $(NPROC)
	@ln -sf $(abspath $(DATA_ZIP)) $(CPP_SRC)/$(BUILD_DIR)/data.zipm

.dependecies: $(SDL_SOURCE_DIR) $(VVVVVV_SOURCE_DIR) $(DATA_ZIP)
	@echo "Dependencies ready."

$(SDL_SOURCE_DIR):
	@git clone $(SDL_GIT_URL) $(SDL_SOURCE_DIR) --branch $(SDL_GIT_BRANCH)
	@mkdir -p $(SDL_SOURCE_DIR)/$(BUILD_DIR)
	@cd $(SDL_SOURCE_DIR)/$(BUILD_DIR) && cmake .. -DSDL_WAYLAND=$(SDL_WAYLAND) && make -j $(NPROC)
	@sudo make -C $(SDL_SOURCE_DIR)/$(BUILD_DIR) install

$(VVVVVV_SOURCE_DIR):
	@git submodule update --init VVVVVV

$(DATA_ZIP):
	@wget -O $(DATA_ZIP) $(DATA_ZIP_URL)

play:
	@cd $(CPP_SRC)/$(BUILD_DIR) && ./VVVVVV

clean:
	@rm -rf $(CPP_SRC)/$(BUILD_DIR)

purge: clean
	@rm -f $(DATA_ZIP)
	@rm -rf $(SDL_SOURCE_DIR)
	@git submodule deinit -f --all
	@echo "All dependencies removed."
