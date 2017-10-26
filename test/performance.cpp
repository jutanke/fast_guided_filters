#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <ctime>
#include <numeric>
#include "../src/guidedFilter.hpp"

using namespace cv::ximgproc;

/**
 * Runs the experiment with the given parameters + clocks the
 * execution
 */
double clock_guidedFilterRuns(size_t r, double eps, size_t s,
	double resize_factor, size_t nbr_tests, std::string image_path) {

	std::vector<double> elapsed_secs;

	for (size_t i = 0; i < nbr_tests; ++i) {
		cv::Mat image, blur;
		image = cv::imread(image_path, 0);
		cv::resize(image, image, cv::Size(0, 0), resize_factor, resize_factor);
		clock_t begin = clock();
		fgf::blur(image, blur, r, eps, s);
		clock_t end = clock();
		double elapsed_sec = static_cast<double>(end - begin) / CLOCKS_PER_SEC;
		elapsed_secs.push_back(elapsed_sec);
	}


}

int main()
{

	cv::Mat image;
	//image = cv::imread("../test/cat.bmp", 0);

	size_t nbr_tests = 10;
	std::vector<double> resizes = {0.5, 1, 2, 4, 8};
	std::string image_path = "../test/smila.jpg";

	for (double resize : resizes) {
		// -----------------------------------------------
		// no-resize test
		// -----------------------------------------------
		size_t r = 4;
		double eps = 0.2 * 0.2;
		size_t s = 4;

		double elapsed = clock_guidedFilterRuns(r, eps, s,
			resize, nbr_tests, image_path);
		std::cout << "res=" << resize << ", s=" << r << ", elapsed=" << elapsed << std::endl;
	}

	/*
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
	*/

	return 0;
}
