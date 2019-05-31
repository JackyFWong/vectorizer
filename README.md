# vectorizer
Calculates distance between two points in real time via OpenCV on Rasperry Pi with C++.

## Dependencies
Install `OpenCV`. Install `hdf5` and `vtk` if your system doesn't it (e.g. Arch Linux).

Instructions for Arch Linux...
```
pacman -S opencv
pacman -S hdf5
pacman -S vtk
```

Instructions for doing it manually on the RPi are located [here](https://www.pyimagesearch.com/2018/09/26/install-opencv-4-on-your-raspberry-pi/) and 
[here](https://docs.opencv.org/4.1.0/d7/d9f/tutorial_linux_install.html)
```
sudo apt-get update && sudo apt-get upgrade
sudo apt-get install build-essential cmake unzip pkg-config
sudo apt-get install libjpeg-dev libtiff-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev
sudo apt-get install libgtk-3-dev
sudo apt-get install libcanberra-gtk*
sudo apt-get install libatlas-base-dev gfortran
```
Now we get OpenCV itself...
```
cd ~
wget -0 opencv.zip https://github.com/opencv/opencv/archive/4.0.0.zip
wget -0 opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.0.0.zip
unzip opencv.zip
unzip opencv_contrib.zip
mv opencv-4.0.0 opencv
mv opencv_contrib-4.0.0 opencv_contrib
```
Compiling OpenCV
```
cd ~/opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
    -D ENABLE_NEON=ON \
    -D ENABLE_VFPV3=ON \
    -D BUILD_TESTS=OFF \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D INSTALL_PYTHON_EXAMPLES=OFF \
    -D BUILD_EXAMPLES=OFF ..
```
The guide mentions increasing the swap size on your RPi. Do so.

`sudo vim /etc/dphys-swapfile`

Then, set `CONF_SWAPSIZE` to `2048` and restart swap
```
sudo /etc/init.d/dphys-swapfile stop
sudo /etc/init.d/dphys-swapfile start
```
Finally, for the multi-hour part,
```
make
sudo make install
sudo ldconfig
```
Reset the swap size back to `100` once you're finished.


Install the `raspicam` library afterwards.
```
git clone https://github.com/cedricve/raspicam .
cd raspicam
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```
