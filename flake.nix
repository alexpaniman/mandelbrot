{
  description = "Mandelbrot set rendering techniques explorer";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];

      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f system
        (import nixpkgs { inherit system; }));
    in
  {

    packages = forAllSystems (system: pkgs: {
      default = pkgs.clangStdenv.mkDerivation {
        name = "mandelbrot";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          clang-tools
          cmake
          ninja
          m4
          makeWrapper
          glfw
          mpfr
        ] ++ pkgs.lib.optionals (!pkgs.stdenv.isDarwin) [ glew ];

        buildInputs = with pkgs; [ glfw mpfr ]
          ++ pkgs.lib.optionals (!pkgs.stdenv.isDarwin) [ glew ]
          ++ pkgs.lib.optionals pkgs.stdenv.isDarwin (with pkgs.darwin.apple_sdk.frameworks; [
            OpenGL Cocoa IOKit CoreFoundation
          ]);

        cmakeFlags = [ "-DCMAKE_BUILD_TYPE=Release" ];

        postFixup = ''
          cp -r $src/res $out
          wrapProgram $out/bin/mandelbrot --set MANDELBROT_SHADER_HOME $out/res
        '';
      };
    });

  };
}
