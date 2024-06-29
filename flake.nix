{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        name = "psngm";
        version = "0.0.0";
        src = ./.;
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = {
          default = pkgs.stdenv.mkDerivation rec {
            inherit name src;

            buildInputs = with pkgs; [ automake autoconf pkg-config ncurses ];

            configurePhase = ''
              autoreconf -i && ./configure --prefix=$out 
            '';

            buildPhase = ''
              make
            '';

            installPhase = ''
              make install
            '';
          };
        };
      });
}
