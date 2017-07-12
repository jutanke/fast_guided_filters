#include <iostream>
#include <opencv2/opencv.hpp>
#include "../src/guidedFilter.hpp"

int main()
{

	cv::Mat image;
	image = cv::imread("../test/smila.jpg");

	cv::Mat blur;
	size_t s = 4;
	double eps = 0.4;
	size_t scale = 1;
	fgf::blur(image, blur, s, eps, scale);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", blur);


  cv::waitKey(0);

	return 0;
}
