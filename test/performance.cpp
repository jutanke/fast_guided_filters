#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <ctime>
#include <numeric>
#include "../src/guidedFilter.hpp"

using namespace cv::ximgproc;

int main()
{

	cv::Mat image;
	image = cv::imread("../test/smila.jpg");
	//image = cv::imread("../test/cat.bmp", 0);

	size_t nbr_tests = 100;

	cv::Mat blur;
	size_t r = 4;
	double eps = 0.2 * 0.2;
	size_t s = 1;

	std::vector<double> elapsed_secs;

	for (size_t i = 0; i < nbr_tests; ++i) {
		clock_t begin = clock();
		fgf::blur(image, blur, r, eps, s);
		clock_t end = clock();
		double elapsed_sec = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
		elapsed_secs.push_back(elapsed_sec);
	}

	double fgf_avg_elapsed_sec = std::accumulate(
		elapsed_secs.begin(), elapsed_secs.end(), 0.0
	) / elapsed_secs.size();

	std::cout << "fgf: elapsed:" << fgf_avg_elapsed_sec << std::endl;

	// ------------------------------------
	// OpenCV implementation
	// ------------------------------------

	std::vector<double> elapsed_secs_ocv;
	cv::Mat dst;
	for (size_t i = 0; i < nbr_tests; ++i) {
		clock_t begin = clock();
	  guidedFilter(image, image, dst, r, eps);
		clock_t end = clock();
		double elapsed_sec = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
		elapsed_secs_ocv.push_back(elapsed_sec);
	}

	double fgf_avg_elapsed_sec_ocv = std::accumulate(
		elapsed_secs_ocv.begin(), elapsed_secs_ocv.end(), 0.0
	) / elapsed_secs_ocv.size();

	std::cout << "ocv: elapsed:" << fgf_avg_elapsed_sec_ocv << std::endl;

	cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
  cv::imshow("Display Image", dst);
	cv::waitKey(0);

	return 0;
}
