# vectorizer
Calculates distance between two points in real time via OpenCV

## Dependencies
Install `OpenCV`. Install `hdf5` and `vtk` if your system doesn't it (e.g. Arch Linux).
```
pacman -S opencv
pacman -S hdf5
pacman -S vtk
```

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
