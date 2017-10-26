#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include "../src/guidedFilter.hpp"

using namespace cv::ximgproc;

int main()
{

	cv::Mat image;
	image = cv::imread("../test/smila.jpg");
	//image = cv::imread("../test/cat.bmp", 0);

	cv::Mat blur;
	int r = 8;
	double eps = 0.4 * 0.4;
	size_t s = 1;

	cv::Mat dst;
	guidedFilter(image, image, dst, r, 1000, -1);

	fgf::blur(image, blur, r, eps, s);

	// ----------------------


	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", image);
	cv::waitKey(0);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	cv::imshow("Display Image (ocv gf)", dst);
	cv::waitKey(0);

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
	cv::imshow("Display Image (own gf)", blur);
	cv::waitKey(0);

	return 0;
}
