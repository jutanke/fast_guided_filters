# Fast Guided Filters

Headers-only OpenCV3 implementation of the [Fast Guided Filter](https://arxiv.org/pdf/1505.00996.pdf). This implementation is more faithful to the original papers parameters than the current OpenCV version (OCV 3.3.0) as it mirrors the original Matlab code. The runtime of the unscaled guided filter (s=1) and the OpenCV guided filter implementation are equivalent. 

![smila_animation](https://user-images.githubusercontent.com/831215/31575901-bb16c344-b0f0-11e7-9c08-d73e99ee0379.gif)

The animation starts at the original image and then increases the subsampling size by 4 every round: Subsampling sizes: [1, 4, 8, 12, 16, 20]

## Performance

Measuring was perfomed on Ubuntu 16.04 with an Intel(R) Core(TM) i7-4500U CPU @ 1.80GHz and 8GB of RAM
![perf](https://user-images.githubusercontent.com/831215/32043935-65753344-ba3c-11e7-97e1-325ec58c8567.png)

## Usage

Simply add the header file "src/guidedFilter.hpp" whereever you like and point to it from your source code.

```c++
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../src/guidedFilter.hpp"

int main()
{
  cv::Mat image;
  image = cv::imread("../test/cat.bmp", 0);
  
  cv::Mat blur;
  int r = 8;
  double eps = 0.4 * 0.4;
  size_t s = 4;
  
  fgf::blur(image, blur, r, eps, s);
  
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", image);
  cv::waitKey(0);
  
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image (own gf)", blur);
  cv::waitKey(0);
}

```

### Minimal CMake

```cmake
cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable( Sample test/sample.cpp )
target_link_libraries( Sample ${OpenCV_LIBS} )
```

## Samples

Bluring an image with radius 4, epsilon 0.2^2 and sumsampling-size 4:

<div>
<img width='300px' style='float=left' src='https://user-images.githubusercontent.com/831215/31560087-472a3b34-b053-11e7-931d-a18b2dc5a4ee.png'/>
<img width='300px' src='https://user-images.githubusercontent.com/831215/31560078-43150e84-b053-11e7-9abb-8535fec5ed7d.png'>
</div>
