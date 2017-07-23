# img_processing
we take images and then process them
# Table of Contents
### 1)Getting Started
### 2)Environment Setup
### 3)What is CUDA, and Do I Need It?
### 4)What to Do if I Can't Use CUDA

# Getting Started
If you just want to get started working, ignore the rest of this readme and contact team leadership about getting an account on our brand-new dev server! This means you don't need to do anything but ssh in and start working. If you want to set up your own development environment so you can work when offline, continue reading.

# Enviornment Setup
## Install Linux
I would recommend some form of Ubuntu I go for Ubuntu Gnome. Install it in its own partition, just makes some things easier.

## Install Nvidia Driver
Note: This is not possible if your main graphics driver is not NVIDIA
1. Download [Nvidia Driver](http://www.nvidia.com/object/unix.html)
2. `sudo sh <DRIVER FILE NAME HERE>`
3. reboot
4. verify everything works `nvidia-smi` should show your driver version and gpu model 

[link](http://askubuntu.com/questions/451221/ubuntu-14-04-install-nvidia-driver) for when this fails 

## Install Cuda
Note: Do not do these steps if you cannot install the NVIDIA driver
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
Note: Everyone can do this step, regardless of CUDA being installed or not.
1. Go to this link [Installing openCV](https://gist.github.com/arthurbeggs/06df46af94af7f261513934e56103b30/), dowload and run this shell script with
```
sudo sh ./install-opencv-2.4.13-in-ubuntu.sh
```
in whatever folder you saved the file.

### Or you could do it the hard way
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

if this worked then go you!!!

if not meet your new best [friend](https://google.com)

# What is CUDA, and Do I Need It?
In a few words, CUDA is a language that allows your program to run on a GPU, rather than the CPU. This gives you crazy good performance on certain things, especially image processing. When doing development, you will find that CUDA looks suspiciously like C++, and you'd be right. CUDA runs just like a C++ program would on your CPU, except on your GPU. As you will see soon, the code looks almost identical.

# What to Do if I can't use CUDA?
Either because of an incompatible graphics card, or any other reason? No worries. Code written for the CPU will compile and execute the same commands as it would on a GPU. So just go ahead and write all you code so it's CPU friendly, aka in plain C++. Take a look at these snippets from gpu_circle_video.cpp and cpu_circle_video.cpp

From gpu_circle_video.cpp
```
	gpu::GpuMat tempFrame, tempGrey, tempEdges;
	
	//Creating a Mat version of the current frame
	tempFrame.upload(cvarrToMat(frame));
	tempGrey.upload(cvarrToMat(grey));
	tempEdges.upload(cvarrToMat(edges));

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
    	gpu::cvtColor(tempFrame, tempGrey, CV_BGR2GRAY);
```

From cpu_circle_video.cpp
```
	Mat tempFrame, tempGrey, tempEdges;
	
	//Creating a Mat version of the current frame
	cvarrToMat(frame).copyTo(tempFrame);
	cvarrToMat(grey).copyTo(tempGrey);
	cvarrToMat(edges).copyTo(tempEdges);
	

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
        cvtColor(tempFrame, tempGrey, CV_BGR2GRAY);
```

As you can see, the only difference between these two is the way that objects are referenced. In the GPU file, we need to upload the images to a gpu matrix, while in the CPU file we copy the images to a CPU matrix. In order to make GPU-running code compatible with the CPU, you just have to replace references to GPU objects. Other than this, all processing logic should be the same.
