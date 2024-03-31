{
  description = "Mandelbrot set rendering techniques explorer";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    nixgl.url = "github:nix-community/nixGL";
  };

  outputs = { self, nixpkgs, nixgl }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        system = system;
        overlays = [ nixgl.overlay ];
      };
    in
  {

    packages.${system}.default = pkgs.clangStdenv.mkDerivation {
      name = "mandelbrot";
      src = ./.;

      nativeBuildInputs = with pkgs; [
        clang-tools

        cmake
        ninja

        m4
        makeWrapper

        glew
        glfw3
      ];

      cmakeFlags = [ "-DCMAKE_BUILD_TYPE=Release" ];

      postFixup = ''
        cp -r $src/res $out
        wrapProgram $out/bin/mandelbrot --set MANDELBROT_SHADER_HOME $out/res
      '';
    };

  };
}
