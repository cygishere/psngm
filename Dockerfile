FROM debian:bookworm-slim

WORKDIR /psngm

RUN apt-get update && apt-get install --yes --no-install-recommends \
  build-essential curl gcc make automake autoconf pkg-config \
  libncurses-dev
  

COPY src/ ./src/
COPY configure.ac Makefile.am ./

RUN autoreconf -vi && ./configure && make
