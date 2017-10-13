#include <iostream>
#include <opencv2/opencv.hpp>
#include "../src/guidedFilter.hpp"

int main()
{

	cv::Mat image;
	image = cv::imread("../test/smila.jpg");
	//image = cv::imread("../test/cat.bmp", 0);

	cv::Mat blur;
	size_t r = 4;
	double eps = 0.2 * 0.2;
	size_t s = 4;
	fgf::blur(image, blur, r, eps, s);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", image);
	cv::waitKey(0);


	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	cv::imshow("Display Image", blur);
	cv::waitKey(0);


	return 0;
}
