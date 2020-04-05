sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq

sudo apt-get install -qq g++-8 # g++ with C++17 install
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90

# CMake update
CMAKE_VERSION=3.15.0
CMAKE_VERSION_DIR=v3.15

CMAKE_OS=Linux-x86_64
CMAKE_TAR=cmake-$CMAKE_VERSION-$CMAKE_OS.tar.gz
CMAKE_URL=http://www.cmake.org/files/$CMAKE_VERSION_DIR/$CMAKE_TAR
CMAKE_DIR=$(pwd)/cmake-$CMAKE_VERSION

wget --quiet $CMAKE_URL
mkdir -p $CMAKE_DIR
tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR
export PATH=$CMAKE_DIR/bin:$PATH

# Qt libraries install
sudo add-apt-repository --yes ppa:beineri/opt-qt571-xenial
sudo apt-get -qq update
sudo apt-get install --yes qt5-default
sudo apt-get install --yes qt57base 
sudo apt-get install --yes qt57imageformats 
sudo apt-get install --yes qt57svg

# Ninja install
sudo apt-get --yes install ninja-build

# video librarries install
sudo add-apt-repository --yes ppa:mc3man/xerus-media
sudo apt-get -qq update
sudo apt-get install --yes libavcodec-dev 
sudo apt-get install --yes libavformat-dev 
sudo apt-get install --yes libswscale-dev 
sudo apt-get install --yes libavutil-dev

# Google tests install
sudo apt-get install --yes libgtest-dev
cd /usr/src/gtest
sudo env "PATH=$PATH" cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib
cd -
