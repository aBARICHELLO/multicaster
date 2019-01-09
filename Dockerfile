FROM ubuntu:18.10
LABEL author="artur@barichello.me"

# Dependencies required to build on linux and to cross compile for windows 64bits
RUN apt-get update && apt-get install -y \
    binutils-dev \
    build-essential \
    clang \
    g++ \
    g++-mingw-w64-x86-64 \
    libsfml-dev \
    scons \
    unzip \
    wget

RUN wget -S --tries=3 -O sfml.zip https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip \
    && unzip sfml.zip
