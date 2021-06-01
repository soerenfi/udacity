FROM ubuntu:18.04

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y build-essential \
  gdb \
  libcairo2-dev \
  libgraphicsmagick1-dev \
  libpng-dev \
  clang-format \
  clang-tidy \
  git \ 
  apt-transport-https \
  ca-certificates \
  gnupg \ 
  software-properties-common \
  wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
  apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
  apt-get update && \
  apt-get install kitware-archive-keyring && \
  rm /etc/apt/trusted.gpg.d/kitware.gpg && \
  apt-get install -y cmake

RUN git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl && \
  cd P0267_RefImpl && \
  mkdir Debug && \
  cd Debug && \
  cmake -DCMAKE_BUILD_TYPE=Debug .. && \
  make -j && \
  make install

RUN wget https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2 \
  && tar -xvjf wxWidgets-3.1.0.tar.bz2 \
  && cd wxWidgets-3.1.0 && mkdir build_w64 && cd build_w64 && \
  ../configure --prefix=/usr/x86_64-w64-mingw32/ --host=x86_64-w64-mingw32 --build=x86_64-linux --disable-unicode --enable-monolithic --disable-shared CFLAGS=-m64 CXXFLAGS=-m64 LDFLAGS=-m64 && make

