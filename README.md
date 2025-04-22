# LibXenon
[![GitHub Workflow - Docker](https://img.shields.io/github/actions/workflow/status/Free60Project/libxenon/docker.yml?label=docker)](https://github.com/Free60Project/libxenon/actions?query=workflow%3Adocker)
[![GitHub pages - documentation](https://img.shields.io/github/actions/workflow/status/Free60Project/libxenon/doxygen.yml?label=documentation)](https://free60project.github.io/libxenon/)
[![Docker pulls (LibXenon)](https://img.shields.io/docker/pulls/free60/libxenon?label=docker%20pulls%20%28libxenon%29)](https://hub.docker.com/r/free60/libxenon)
[![Docker pulls (Toolchain)](https://img.shields.io/docker/pulls/free60/toolchain?label=docker%20pulls%20%28toolchain%29)](https://hub.docker.com/r/free60/toolchain)

LibXenon provides a bare-metal homebrew library for the Xbox 360 gaming console.

## Prebuilt images

Check out (https://hub.docker.com/u/free60)

Example usage

```
host $ cd libxenon-homebrew-app/
host $ docker run -it -v $PWD:/app free60/libxenon:latest
docker $ cd /app
docker $ make
```

## Manual build

### Requirements

Dependencies for Linux distributions using the `dnf` or `apt` package managers will automatically be installed for you. These include:

- flex
- bison
- gcc-multilib
- libgmp3-dev
- libmpfr-dev
- libmpc-dev
- texinfo
- git-core
- build-essential
- wget
- file

The main driving script to setup libxenon is at `toolchain/build-xenon-toolchain`. Please see below on how to use it.

### Environment variables

After installation of the toolchain, the following environment variables need to be populated.

`DEVKITXENON` is depends on your chosen installation prefix location. The default is `/usr/local/xenon`:

```
DEVKITXENON=/usr/local/xenon
PATH="${PATH:+${PATH}:}$DEVKITXENON/bin:$DEVKITXENON/usr/bin"
```

You may edit your ~/.bashrc to set these. Alternatively, you may execute `./build-xenon-toolchain env-cmd` to install a command named `xenon-env`. When you run that command, it will set those variables in a new shell.

### Prefix

If you want to choose your own prefix, prepend it to the `./build-xenon-toolchain` invocation.

e.g. `PREFIX=/home/username/xenon ./build-xenon-toolchain toolchain`

### Installing toolchain and libxenon library

```
./build-xenon-toolchain toolchain
```

### Installing libxenon library (useful for updating just libxenon)

```
./build-xenon-toolchain libxenon
```

### Installing auxiliary libs (libxenon, bin2s, zlib, libpng, bzip2, freetype, filesystems)

```
./build-xenon-toolchain libs
```
