# Install SDL2 dependency
SDL_SOURCE_DIR := .sdl
SDL_GIT_URL := https://github.com/libsdl-org/SDL.git
SDL_GIT_BRANCH := release-2.26.0
SDL_WAYLAND := ON
SDL_IS_INSTALLED := $(SDL_SOURCE_DIR)

# Install SDL2_image dependency
SDL_IMAGE_SOURCE_DIR := .sdl_image
SDL_IMAGE_GIT_URL := https://github.com/libsdl-org/SDL_image.git
SDL_IMAGE_GIT_BRANCH := release-2.8.8
SDL_IMAGE_IS_INSTALLED := $(SDL_IMAGE_SOURCE_DIR)

# Install ASIO dependency
ASIO_SOURCE_DIR := .asio
ASIO_GIT_URL := https://github.com/chriskohlhoff/asio.git
ASIO_GIT_BRANCH := asio-1-34-2
ASIO_IS_INSTALLED := $(ASIO_SOURCE_DIR)

# If the README.md file is not present, it means the submodule is not initialized
VVVVVV_SOURCE_DIR := src/cpp/VVVVVV
VVVVVV_SOURCE_GIT_IS_INIT := $(VVVVVV_SOURCE_DIR)/README.md

# This file is necessary for the game to run
DATA_ZIP := data.zip
DATA_ZIP_URL := https://thelettervsixtim.es/makeandplay/data.zip
DATA_ZIP_IS_DOWNLOADED := $(DATA_ZIP)

# venv and pip dependencies
VENV_DIR := .venv
PYTHON := python
PIP := pip
PIP_REQUIREMENTS := requirements.txt
VENV_IS_INSTALLED := $(VENV_DIR)/bin/activate

# Variables
CPP_SRC := src/cpp
BUILD_DIR := build
BIN_DIR := bin
LIB_DIR := lib
NPROC := $(shell nproc)

# Targets
.PHONY: build .dependencies

all: build

build: .dependencies
	@mkdir -p $(CPP_SRC)/$(BUILD_DIR)
	@cd $(CPP_SRC)/$(BUILD_DIR) && cmake .. && make -j $(NPROC)
	@ln -sf $(abspath $(DATA_ZIP)) $(CPP_SRC)/$(BIN_DIR)/$(DATA_ZIP)

.dependencies: $(SDL_IS_INSTALLED) $(SDL_IMAGE_IS_INSTALLED) $(ASIO_IS_INSTALLED) $(VVVVVV_SOURCE_GIT_IS_INIT) $(DATA_ZIP_IS_DOWNLOADED)
	@echo "Dependencies ready."

$(SDL_IS_INSTALLED):
	@git clone $(SDL_GIT_URL) $(SDL_SOURCE_DIR) --branch $(SDL_GIT_BRANCH)
	@mkdir -p $(SDL_SOURCE_DIR)/$(BUILD_DIR)
	@cd $(SDL_SOURCE_DIR)/$(BUILD_DIR) && cmake .. -DSDL_WAYLAND=$(SDL_WAYLAND) && make -j $(NPROC)
	@sudo make -C $(SDL_SOURCE_DIR)/$(BUILD_DIR) install

$(SDL_IMAGE_IS_INSTALLED):
	@git clone $(SDL_IMAGE_GIT_URL) $(SDL_IMAGE_SOURCE_DIR) --branch $(SDL_IMAGE_GIT_BRANCH)
	@mkdir -p $(SDL_IMAGE_SOURCE_DIR)/$(BUILD_DIR)
	@cd $(SDL_IMAGE_SOURCE_DIR)/$(BUILD_DIR) && cmake .. && make -j $(NPROC)
	@sudo make -C $(SDL_IMAGE_SOURCE_DIR)/$(BUILD_DIR) install

$(ASIO_IS_INSTALLED):
	@git clone $(ASIO_GIT_URL) $(ASIO_SOURCE_DIR) --branch $(ASIO_GIT_BRANCH)
	@cd $(ASIO_SOURCE_DIR)/asio && bash autogen.sh
	@cd $(ASIO_SOURCE_DIR)/asio && ./configure --without-boost
	@cd $(ASIO_SOURCE_DIR)/asio && make -j $(NPROC)
	@cd $(ASIO_SOURCE_DIR)/asio && sudo make install

$(VVVVVV_SOURCE_GIT_IS_INIT):
	@git submodule update --init --recursive $(VVVVVV_SOURCE_DIR)

$(DATA_ZIP_IS_DOWNLOADED):
	@wget -O $(DATA_ZIP) $(DATA_ZIP_URL)

$(VENV_IS_INSTALLED):
	@$(PYTHON) -m venv $(VENV_DIR)
	$(PIP) install -r $(PIP_REQUIREMENTS)

play:
	@cd $(CPP_SRC)/$(BIN_DIR) && ./VVVVVV

clean:
	@rm -rf $(CPP_SRC)/$(BUILD_DIR)
	@rm -rf $(CPP_SRC)/$(BIN_DIR)
	@rm -rf $(CPP_SRC)/$(LIB_DIR)

purge: clean
	@rm -f $(DATA_ZIP)
	@rm -rf $(SDL_SOURCE_DIR)
	@rm -rf $(SDL_IMAGE_SOURCE_DIR)
	@rm -rf $(ASIO_SOURCE_DIR)
	@rm -rf $(VENV_DIR)
	@rm -rf .git/modules/VVVVVV
	@git submodule deinit -f --all
	@echo "All dependencies removed."
