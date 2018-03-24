# O3D303
the driver of libo3d303

# Install Introduction

## Dependencies

* [Boost](http://www.boost.org) (>= 1.54)  
sudo apt-get install mpi-default-dev libicu-dev python-dev python3-dev libbz2-dev zlib1g-dev
tar --bzip2 -xvf boost_1_66_0.tar.bz2
cd boost_1_66_0/
./bootstrap.sh
./b2
sudo ./b2 install

* [Gtest](https://github.com/google/googletest) (unit testing)
sudo apt-get install libgtest-dev 
cd /usr/src/gtest/
sudo mkdir build
cd build/
sudo cmake ..
sudo make
sudo cp libgtest*.a /usr/local/lib/ 

* [Glog](https://github.com/google/glog) (logging infrastructure)
git clone https://github.com/google/glog
cd glog/
./autogen.sh 
sudo apt-get install autoconf 
sudo apt-get install automake
sudo apt-get install libtool
./autogen.sh 
./configure 
make -j 24
sudo make install

* [libxmlrpc](http://xmlrpc-c.sourceforge.net/)

* [PCL](http://pointclouds.org) (>= 1.7.1)
git clone https://github.com/PointCloudLibrary/pcl.git   
sudo apt-get install cmake g++   libboost1.58-all-dev libeigen3-dev libflann-dev python libusb-1.0-0-dev libudev-dev freeglut3-dev doxygen graphviz libpng12-dev libgtest-dev libxmu-dev libxi-dev libpcap-dev libqhull-dev libvtk5-qt4-dev python-vtk libvtk-java 

