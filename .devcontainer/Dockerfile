FROM mcr.microsoft.com/devcontainers/base:noble


# Install dependencies
RUN git submodule update --init &&\
    sudo apt-get update &&\
    sudo apt-get install -y --no-install-recommends \
         build-essential \
         linux-headers-$(uname -r) \
         cmake

# Install SDL 2.24
RUN git clone --branch release-2.24.0 https://github.com/libsdl-org/SDL.git &&\
    mkdir -p SDL/build &&\
    cd SDL/build &&\
    cmake .. &&\
    make -j $(nproc) &&\
    sudo make install

# Compile VVVVVV
RUN mkdir -p VVVVVV/desktop_version/build &&\
    cd VVVVVV/desktop_version/build &&\
    cmake .. &&\
    make -j $(nproc) &&\
    # Get data.zip file
    wget https://thelettervsixtim.es/makeandplay/data.zip
