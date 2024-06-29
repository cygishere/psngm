{
  pkgs ? import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/53df5c008d74bdaca6da6c50f43649f7211d1dc6.tar.gz";
    sha256 = "sha256:00c0vngxp8p24glihhr91b19zjmfbng4khvsgxy5m78jy6pbcj9d";
  }) {}
}:
pkgs.stdenv.mkDerivation rec {
  pname = "psngm";
  version = "0.0.0";

  src = ./.;

  buildInputs = with pkgs; [
    automake
    autoconf
    pkg-config
    ncurses
  ];

  configurePhase = ''
    autoreconf -i && ./configure --prefix=$out 
  '';

  buildPhase = ''
    make
  '';

  installPhase = ''
    make install
  '';
}
