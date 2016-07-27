# img_processing
we take images and then process them

#Enviornment Setup
## Install Linux
I would recommend some form of Ubuntu I go for Ubuntu Gnome. Install it in it's own partition, just makes some things easier.

## Install Nvidia Driver
1. Download [Nvidia Driver](http://www.nvidia.com/object/unix.html)
2. `sudo sh <DRIVER FILE NAME HERE>`
3. reboot
4. verify everything works `nvidia-smi` should show your driver version and gpu model 

[link](http://askubuntu.com/questions/451221/ubuntu-14-04-install-nvidia-driver) for when this fails 

## Install Cuda
1. [Downoad Cuda](https://developer.nvidia.com/cuda-toolkit) at the time of writing we are using 8 so get that
2. run 
```
sudo dpkg -i <DOWNLOADED FILE NAME HERE>
sudo apt-get update
sudo apt-get install cuda-toolkit-8-0
```
3.set up enviroment variables
```
export PATH=/usr/local/cuda-8.0/bin${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```
4.Verify
```
cuda-install-samples-8.0.sh <dir>
cd <dir>
make
cd bin/<hit tab a bunch>/release
./deviceQuery
```
>This should pass and give you stats on your gpu. You can run a few more programs to verify the install

[When this fails](https://developer.nvidia.com/cuda-downloads)

For errors related to gcc or g++ versions

install older version: `sudo apt-get install gcc-4.9 g++-4.9`
create a link to cuda installation: `sudo ln -s /usr/bin/gcc-4.9 /usr/local/cuda/bin/gcc` and repeat with `g++` instead of `gcc`

## Install OpenCV
1. make sure we got the required libraries
```
sudo apt-get update

sudo apt-get install libopencv-dev build-essential checkinstall cmake pkg-config yasm libtiff5-dev libjpeg-dev libjasper-dev libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libxine2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev python-dev python-numpy libtbb-dev libqt4-dev libgtk2.0-dev libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev x264 v4l-utils

sudo add-apt-repository ppa:jon-severinsson/ffmpeg  
sudo apt-get update  
sudo apt-get install ffmpeg  
sudo apt-get install frei0r-plugins  
```
2.Download OpenCV
```
mkdir OpenCV  
cd OpenCV  
git clone https://github.com/Itseez/opencv.git  
```
>This can go whever

3.Build it the First time
```
cd opencv  
mkdir release  
cd release  
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON -D ENABLE_FAST_MATH=1 -D CUDA_FAST_MATH=1 -D WITH_CUBLAS=1 ..  
```
4.Compile It
```
make  
sudo make install  
```
##Gcc too new error
if you get an error about gcc not being supported run the following
```
cd /user/bin  
sudo rm -rf gcc-5
sudo rm -rf g++-5
```
5.Configure Library Search Path
```
echo '/usr/local/lib' | sudo tee -a /etc/ld.so.conf.d/opencv.conf  
sudo ldconfig  
printf '# OpenCV\nPKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig\nexport PKG_CONFIG_PATH\n' >> ~/.bashrc  
source ~/.bashrc  
```

6.Optional Thrust Library Fix
Only do this if your compiliation failed on something about thrust was mentioned.

1. Download Thrust 
```
mkdir thrust
cd thrust
git clone git@github.com:thrust/thrust.git
```
2.Replace Cuda Thrust
```
sudo rm -rf /usr/local/cuda-8.0/include/thrust
cd thrust 
sudo cp thrust /usr/local/cuda-8.0/include/
```
3.try the compilation again

if this worked then go you!!!

if not meet your new best [friend](https://google.com)
