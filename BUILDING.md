CMake and vcpkg are used for building. On Linux, make sure to use `clang` as the
compiler and have `ninja` installed as the build generator.

# How to Build
1. Install build tools

Install [CMake](https://cmake.org/download/) and
[Clang](https://releases.llvm.org/download.html) or MSVC.

On Linux, install required build packages (see end).

2. Setup `vcpkg`
```bash
git submodule update --init --recursive
cd vcpkg
.\bootstrap-vcpkg.bat # on Windows
./bootstrap-vcpkg.sh # on Linux/Mac
```

3. Build
```bash
# change directory back to hookline/ first

# cd ..

# Make build directory
mkdir build
cd build

# Configure
cmake .. # Windows
cmake -G "Ninja Multi-Config" .. # Linux

# Build
cmake --build . -- -j 10
```

4. More builds - just rerun the build command
```bash
# cmake .. # if you add new source files, need to reconfigure
cmake --build . -j 10
```

Alternatively if you use CMakeTools Extension in VS Code or you use Visual Studio it will
detect the CMake project automatically and can build for you.

# Problems

The first build will take a while since `vcpkg` might compile a lot of things
from source at first. After that it's fast.

If you have problems during the build (probably missing libraries or OpenGL)
try to read the build output and see what missing library there is and download
them so you can build.

You probably need these libraries on Ubuntu:
```
build-essential \
git \
clang \
clang-tools \
cmake \
doxygen \
make \
ninja-build \
zip \
libxmu-dev \
libxi-dev \
libgl1-mesa-dev \
autoconf \
automake \
libtool \
pkg-config \
perl \
perl \
python3-jinja2 \
libwayland-dev \
libxkbcommon-dev \
libegl1-mesa-dev \
libx11-dev \
libxft-dev \
libxext-dev \
libibus-1.0-dev \
libglu1-mesa-dev \
libglu1-mesa \
libasound2-dev \
```