# img_proc
We take images and then process them.

Image Processing is a group within UNL's UAV team that is tasked with implementing detection algorithms, predicting movement, and communicating with onboard hardware to govern the movement of the team's drone, Momus. To do so, we use powerful tools such as OpenCV and its related CUDA libraries and simulation software called Gazebo.

# Table of Contents
### 1) Getting Started
### 2) What is OpenCV
### 3) What is CUDA

# Getting Started
So you want to process images, huh? Well - have we got a team for you.

On the UAV Team, image processing occurs on our development server and on our graphics card, Nvidia's Jetson, which is on board Momus. The dev server is an Ubuntu 16.0.4 instance with OpenCV 3.3.0 and CUDA 8.0. The dev server has this git repo cloned so most development occurs on the server. When the team is ready for the next push to competition code, we transition to the Jetson and strip the code of unnecessary dev addons, allowing us to run the onboard code faster.

To SSH in and develop on the server, you will need a dev account. Reach out to Daric Teske, Elliot Sandfort, or Jacob Koperski to request a dev account.

# What is OpenCV
Great question - OpenCV is an open source computer vision library written in C/C++. OpenCV is at the core of our image processing endeavors and allows us to filter on color, shape, movement, etc. Additionally, we use OpenCV to stream the results of our prediction calculations real-time. The OpenCV installation that exists on the dev server and the Jetson comes complete with CUDA development support. For more information on what OpenCV is and how to use it, check out its [documentation](http://docs.opencv.org/3.3.0/).

# What is CUDA
Also a great question. We don't know. But that won't stop us. Here's what we think we know - CUDA is a language that allows ours algorithms to run on a GPU, rather than the CPU. This gives us crazy good performance when processing images. CUDA looks suspiciously like C++. In fact, CUDA runs on a GPU like C++ runs on a CPU.

CUDA's speed is possible only through the power of multithreaded processing. The Jetson has 256 CUDA cores. CUDA cores are individual processing units that can perform one operation at a time. CUDA takes full advantage of these cores, meaning we can simultaneously process on 256 threads instead of the single thread we have access to on CPU.
