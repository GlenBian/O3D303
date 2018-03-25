# O3D303
the driver of libo3d303

# Install Introduction

## Dependencies

### [Boost](http://www.boost.org) (>= 1.54)  
```
sudo apt-get install mpi-default-dev libicu-dev python-dev python3-dev libbz2-dev zlib1g-dev
tar --bzip2 -xvf boost_1_66_0.tar.bz2
cd boost_1_66_0/
./bootstrap.sh
./b2
sudo ./b2 install
```

### [Gtest](https://github.com/google/googletest) (unit testing)
```
sudo apt-get install libgtest-dev 
cd /usr/src/gtest/
sudo mkdir build
cd build/
sudo cmake ..
sudo make
sudo cp libgtest*.a /usr/local/lib/ 
```

### [Glog](https://github.com/google/glog) (logging infrastructure)
```
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
```

### [libxmlrpc](http://xmlrpc-c.sourceforge.net/)
```
cd xmlrpc-c-1.39.12/
./configure
make
sudo make install
```

### [PCL](http://pointclouds.org) (>= 1.7.1)
```
cd pcl-pcl-1.7.2/
mkdir build
cd build/
cmake ..
make -j4
sudo make -j2 install
```
## Fast install method (Not recommended)

### Building the libo3d3xx

Assuming you are starting from the top-level directory of this source
distribution:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ sudo make
    
## Make three modules (Recommended)

### Building the camera module

Assuming you are starting from the top-level directory of this source
distribution:

    $ cd modules/camera
    $ mkdir build
    $ cd build
    $ cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
    $ sudo make
    $ sudo make package
    $ sudo apt-get -f install
    $ sudo dpkg -i libo3d3xx-camera_0.4.0_amd64.deb
    
### Building the framegrabber module

*(You must ensure that you have already installed the camera module prior to
 building and installing the framegrabber module.)*

Assuming you are starting from the top-level directory of this source
distribution:

    $ cd modules/framegrabber
    $ mkdir build
    $ cd build
    $ cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
    $ sudo make
    $ sudo make package
    $ sudo dpkg -i libo3d3xx-framegrabber_0.4.0_amd64.deb

