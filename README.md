# modern_pybind11_opencv_numpy

An easy to use and lightweight opencv numpy converter. The goal of this project is to provide a simple and reliable converter between cv::Mat_<T> class and numpy arrays.

The design philosophy is that a simple build process must be maintained. Under Linux a:

	mkdir build && cd build
	cmake .. && cmake --build .

must be sufficient.

The converter will work under a virtual environment as well as with only native python. For this purpose modern CMake is required (>= 3.15). To use a virtual environment simply activate and run the cmake and build process.



Pre-requisites under linux (tested on Ubuntu 20.04 LTS):

	sudo apt install -y libopencv-dev libopencv-highgui-dev libopencv-imgproc-dev python3-opencv
	sudo apt install -y python-dev
	sudo apt install -y build-essential 

