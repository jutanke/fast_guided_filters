#include <iostream>
#include <opencv2/opencv.hpp>
#include "../src/guidedFilter.hpp"

int main()
{
	/*
	cv::Mat t[] = {
		cv::Mat::ones(3, 2, CV_32F),
		cv::Mat::ones(3, 2, CV_32F),
		cv::Mat::ones(3, 2, CV_32F)
	};
	cv::Mat o;
	//cv::Mat o = cv::Mat::ones(3, 2, 3);
	cv::merge(t, 3, o);
	std::cout << "o:" << o.channels() << std::endl;

	cv::Mat mult = cv::Mat::ones(3,2, CV_32F);
	mult = mult * 14;
	std::cout << mult << std::endl;

	//o * mult;
	o = fgf::mul(o, mult);

	cv::Mat out[3];
	cv::split(o, out);
	for (size_t i = 0; i < 3; ++i) {
		cv::Mat c = out[i];
		std::cout << "---" << std::endl;
		std::cout << c << std::endl;
	}

	exit(0);*/

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
