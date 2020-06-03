sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq

sudo apt-get install -qq g++-9 # g++ with C++17 install
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90

# CMake update
CMAKE_VERSION=3.15.0
CMAKE_VERSION_DIR=v3.15

CMAKE_OS=Linux-x86_64
CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
CMAKE_DIR=$(pwd)/cmake-$CMAKE_VERSION

wget --quiet $CMAKE_URL
mkdir -p "$CMAKE_DIR"
tar --strip-components=1 -xzf $CMAKE_TAR -C "$CMAKE_DIR"
export PATH=$CMAKE_DIR/bin:$PATH

# Qt libraries install
sudo apt-get install --yes qt5-default

# Ninja install
sudo apt-get --yes install ninja-build

# video librarries install
sudo apt-get install --yes libavcodec-dev libavformat-dev libswscale-dev

# Google tests install
sudo apt-get install --yes libgtest-dev
cd /usr/src/gtest
sudo env "PATH=$PATH" cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
cd -
