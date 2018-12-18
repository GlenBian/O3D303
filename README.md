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

> * libo3d3xx-camera 依赖于 libxmlrpc-c++8-dev (>= 1.33)；然而： 未安装软件包 libxmlrpc-c++8-dev。解决方法：`sudo apt-get install libxmlrpc-c++8-dev` 如果安装过程报错，输入`sudo apt-get -f install` ，再继续安装.

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

#### Building the image module

Assuming you are starting from the top-level directory of this source
distribution:

    $ cd modules/image
    $ mkdir build
    $ cd build
    $ cmake -DBUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr ..
    $ sudo make
    $ sudo make check
    $ sudo make package
    $ sudo dpkg -i libo3d3xx-image_0.4.0_amd64.deb
    
 When you dpkg this deb,if maybe have this error:
 
 ```
 libo3d3xx-image 依赖于 libopencv-core-dev；然而：未安装软件包 libopencv-core-dev。
 ```
 
 So you want to sudo apt-get install libopencv-*, but still have error,the solution is:
 
 ```
 sudo apt-get -f install
 ```
 
 When you sudo make , you will have o3d3_viewer error:
 
 ```
 cd ~/libo3d3xx-master/modules/image/src/bin
 sudo gedit CMakeLists.txt
 ```
 
 Make some change:
 
 ```
 #if(BUILD_EXE_VIEWER)
#  add_executable(o3d3xx-viewer o3d3xx-viewer.cpp)

#  if(BUILD_SHARED_LIBS)
#    target_link_libraries(o3d3xx-viewer
#      o3d3xx_image_shared
#      ${O3D3XX_CAMERA_LIBRARIES}
#      ${O3D3XX_FRAMEGRABBER_LIBRARIES}
#      ${PCL_VISUALIZATION_LIBRARIES}
#      ${OpenCV_LIBRARIES}
#      ${LIB_boost_program_options}
#      )
#  else()
#    target_link_libraries(o3d3xx-viewer
#      o3d3xx_image_static
#      ${O3D3XX_CAMERA_LIBRARIES}
##      ${O3D3XX_FRAMEGRABBER_LIBRARIES}
#      ${PCL_VISUALIZATION_LIBRARIES}
#      ${OpenCV_LIBRARIES}
#      ${LIB_boost_program_options}
#      )
#  endif(BUILD_SHARED_LIBS)

#  set_target_properties(o3d3xx-viewer PROPERTIES
#    AUTOMOC TRUE
#    COMPILE_FLAGS "-Wno-deprecated"
#    )

#  install(TARGETS o3d3xx-viewer DESTINATION bin)
#
#  list(APPEND O3D3XX_EXE_TARGETS o3d3xx-viewer)
#else()
#  message(STATUS "Skipping o3d3xx-viewer")
#endif(BUILD_EXE_VIEWER)
 ```

## catkin_make o3d3xx-ros pkg

```
cd ~/catkin_ws/src
git clone https://github.com/lovepark/o3d3xx-ros.git
cd ..
catkin_make
```

```
roslaunch o3d3xx camera.launch
roslaunch o3d3xx rviz.launch
```
