# MNIST_net_cpp
A pre-trained, interactive neural network for handwritten digit classification written entirely in C++

The bin folder contains a .zip with the compiled binary. No other files are needed to run the program, because all of the required data (the pre-trained model among other things) is packed into the .exe as resources.

If you wish to compile the program from source code, you need C++17 or greater, OpenMP and OpenGL 4.5 or greater. You also need to link against gomp, opengl32, gdi32, user32 and glu32 and you need to include the additional compiler option -fopenmp. The neural network code is portable and cross-platform, but the resource packing and the window creation is Windows specific.

The network runs on the CPU and its code isn't optimized in the slightest and is instead supposed to be easily readable for anyone proficient in C++. However the network is so small that it should still run on any reasonably modern computer, even slow Laptops etc.
