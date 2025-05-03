# Install SDL2 dependency
SDL_SOURCE_DIR := .sdl
SDL_GIT_URL := https://github.com/libsdl-org/SDL.git
SDL_GIT_BRANCH := release-2.24.0
SDL_WAYLAND := ON
SDL_IS_INSTALLED := $(SDL_SOURCE_DIR)

# Install PyBind11 dependency
PYBIND11_SOURCE_DIR := .pybind11
PYBIND11_GIT_URL := https://github.com/pybind/pybind11.git
PYBIND11_GIT_BRANCH := stable
PYBIND11_IS_INSTALLED := $(PYBIND11_SOURCE_DIR)

# If the README.md file is not present, it means the submodule is not initialized
VVVVVV_SOURCE_DIR := src/cpp/VVVVVV
VVVVVV_SOURCE_GIT_IS_INIT := $(VVVVVV_SOURCE_DIR)/README.md

# This file is necessary for the game to run
DATA_ZIP := data.zip
DATA_ZIP_URL := https://thelettervsixtim.es/makeandplay/data.zip
DATA_ZIP_IS_DOWNLOADED := $(DATA_ZIP)

CPP_SRC := src/cpp
BUILD_DIR := build
BIN_DIR := bin
LIB_DIR := lib
NPROC := $(shell nproc)

# Targets
.PHONY: build .dependencies

all: build

build: .dependecies
	@mkdir -p $(CPP_SRC)/$(BUILD_DIR)
	@cd $(CPP_SRC)/$(BUILD_DIR) && cmake .. && make -j $(NPROC)
	@ln -sf $(abspath $(DATA_ZIP)) $(CPP_SRC)/$(BIN_DIR)/$(DATA_ZIP)

.dependecies: $(SDL_IS_INSTALLED) $(PYBIND11_IS_INSTALLED) $(VVVVVV_SOURCE_GIT_IS_INIT) $(DATA_ZIP_IS_DOWNLOADED)
	@echo "Dependencies ready."

$(SDL_IS_INSTALLED):
	@git clone $(SDL_GIT_URL) $(SDL_SOURCE_DIR) --branch $(SDL_GIT_BRANCH)
	@mkdir -p $(SDL_SOURCE_DIR)/$(BUILD_DIR)
	@cd $(SDL_SOURCE_DIR)/$(BUILD_DIR) && cmake .. -DSDL_WAYLAND=$(SDL_WAYLAND) && make -j $(NPROC)
	@sudo make -C $(SDL_SOURCE_DIR)/$(BUILD_DIR) install

$(PYBIND11_IS_INSTALLED):
	@git clone $(PYBIND11_GIT_URL) $(PYBIND11_SOURCE_DIR) --branch $(PYBIND11_GIT_BRANCH)
	@cmake -S $(PYBIND11_SOURCE_DIR) -B $(PYBIND11_SOURCE_DIR)/$(BUILD_DIR)
	@cmake --build $(PYBIND11_SOURCE_DIR)/$(BUILD_DIR) -j $(NPROC)
	@sudo cmake --install $(PYBIND11_SOURCE_DIR)/$(BUILD_DIR)

$(VVVVVV_SOURCE_GIT_IS_INIT):
	@git submodule update --init --recursive $(VVVVVV_SOURCE_DIR)

$(DATA_ZIP_IS_DOWNLOADED):
	@wget -O $(DATA_ZIP) $(DATA_ZIP_URL)

play:
	@cd $(CPP_SRC)/$(BIN_DIR) && ./VVVVVV

clean:
	@rm -rf $(CPP_SRC)/$(BUILD_DIR)
	@rm -rf $(CPP_SRC)/$(BIN_DIR)
	@rm -rf $(CPP_SRC)/$(LIB_DIR)

purge: clean
	@rm -f $(DATA_ZIP)
	@rm -rf $(SDL_SOURCE_DIR)
	@rm -rf $(PYBIND11_SOURCE_DIR)
	@rm -rf .git/modules/VVVVVV
	@git submodule deinit -f --all
	@echo "All dependencies removed."
