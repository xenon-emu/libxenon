{ stdenv }:

stdenv.mkDerivation {
  name = "libxenon";
  src = ./.;
  CROSS = "${stdenv.hostPlatform.config}";
  preBuild = ''
    pwd
    export DEVKITXENON=$(realpath devkitxenon)
    cd libxenon/ports/xenon
  '';
  hardeningDisable = [ "format" ];
  preInstall = ''
    export DEVKITXENON=$out
  '';
  postInstall = ''
    pushd $out
    mv usr/lib .
    mv usr/include .
    rmdir usr
    popd

    cd $NIX_BUILD_TOP/$sourceRoot
    mkdir $out/devkitxenon
    cd devkitxenon
    cp rules app.lds $out/devkitxenon
  '';
}
