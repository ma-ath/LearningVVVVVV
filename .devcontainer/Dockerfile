FROM ubuntu:noble

# Add vscode user with same UID and GID as your host system
ARG USERNAME
ARG USER_UID
ARG USER_GID

# Set default UID and GID if not provided
ENV USER_UID=${USER_UID:-1001}
ENV USER_GID=${USER_GID:-$USER_UID}

# Set USER_GID to USER_UID if not provided.
# If USER_ID already exists on image, delete it and setup a new user
RUN if id -u $USER_UID > /dev/null 2>&1; then userdel --remove $(getent passwd $USER_UID | cut -d: -f1); fi \
    && groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo "$USERNAME ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# Switch from root to user
USER $USERNAME

# Install dependencies
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Tokyo
RUN sudo apt-get update && \
    sudo apt-get upgrade -y && \
    sudo apt-get install -y --no-install-recommends \
        autoconf\
        automake\
        build-essential\
        ca-certificates\
        cmake\
        git\
        gnupg2\
        libdrm-dev\
        libegl1-mesa-dev\
        libxkbcommon-dev\
        libwayland-dev\
        libwayland-client0\
        libwayland-server0\
        libxext-dev\
        linux-headers-$(uname -r)\
        libpulse-dev\
        libavif-dev\
        libjpeg-dev\
        libtiff-dev\
        libasound2-dev\
        libpipewire-0.3-dev\
        openssh-client\
        pkg-config\
        wayland-protocols\
        wget\
        xdg-desktop-portal\
        xdg-desktop-portal-gtk\
        python3-venv\
        python-is-python3\

    
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH