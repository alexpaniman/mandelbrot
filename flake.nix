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
