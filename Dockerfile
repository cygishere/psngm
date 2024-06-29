FROM debian:bookworm-slim

WORKDIR /psngm

RUN apt-get update && apt-get install --yes --no-install-recommends \
  gcc make automake autoconf pkg-config \
  libncurses-dev
  
COPY . /psngm

RUN autoreconf -i && ./configure && make
