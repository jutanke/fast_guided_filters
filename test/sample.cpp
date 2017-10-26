#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <ctime>
#include "../src/guidedFilter.hpp"

using namespace cv::ximgproc;

int main()
{

	cv::Mat image;
	//image = cv::imread("../test/smila.jpg");
	image = cv::imread("../test/cat.bmp", 0);

	cv::Mat blur;
	int r = 8;
	double eps = 0.4 * 0.4;
	size_t s = 1;

	cv::Mat dst;

	clock_t begin = clock();
	guidedFilter(image, image, dst, r, 1000, -1);
	clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "ocv:" << elapsed_secs << std::endl;

	begin = clock();
	fgf::blur(image, blur, r, eps, s);
	end = clock();
  elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "own:" << elapsed_secs << std::endl;

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

	/*dst.convertTo(dst, CV_32F);
	blur.convertTo(blur, CV_32F);

	cv::Mat diff = dst - blur;
	double min, max;
	cv::minMaxLoc(diff, &min, &max);
	std::cout << "min:" << min << ", max:" << max << std::endl;*/

	return 0;
}
