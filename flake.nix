{
  description = "Mandelbrot set rendering techniques explorer";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
  {

    packages.${system}.default = pkgs.stdenv.mkDerivation {
      name = "mandelbrot";
      src = ./.;

      nativeBuildInputs = with pkgs; [ cmake clang ninja m4 glew glfw3 ];

      cmakeFlags = [
        "-DCMAKE_CXX_COMPILER=${pkgs.clang}/bin/clang++"
        "-DCMAKE_C_COMPILER=${pkgs.clang}/bin/clang"
        "-DCMAKE_BUILD_TYPE=Release"
      ];
    };

  };
}
