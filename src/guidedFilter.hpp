#ifndef GUIDED_FILTER_H
#define GUIDED_FILTER_H
#include <iostream>
#include <opencv2/opencv.hpp>

namespace fgf {

  /** @brief unnormalized boxfilter

    @param imSrc input image
    @param r size of extension into all directions
   */
  cv::Mat inline boxfilter(const cv::Mat &imSrc, size_t r) {
    r = r * 2 + 1;
    cv::Mat imDst;
  	cv::boxFilter(imSrc, imDst, -1, cv::Size(r,r), cv::Point(-1, -1), false, cv::BORDER_CONSTANT);
    return imDst;
  }

  /**  @brief transfers structure of one image onto the other

    @param I guidance image
    @param p filtering input image
    @param q filter output image
    @param r local window radius
    @param eps regularization parameter
    @param s subsampling ratio
  */
  void inline structureTransference(
    const cv::Mat &I_,
    const cv::Mat &p_,
    cv::Mat &q,
    const size_t r,
    const double eps,
    const size_t s)
  {
    cv::Mat I, p;
    I_.convertTo(I, CV_32F);
    I = I /255.0;

    cv::Mat I_sub, p_sub;
    size_t r_sub = r / s;


    cv::resize(I, I_sub, cv::Size(I.cols/s, I.rows/s), 0, 0, cv::INTER_NEAREST);
    cv::Mat mean_I, mean_p;
    mean_I = boxfilter(I_sub, r_sub);

    cv::Mat N(I_sub.rows, I_sub.cols, CV_32F, cv::Scalar(1));
    N = boxfilter(N, r_sub);

    mean_I = mean_I / N;

    if (&I_ == &p_) {
      // in case of just smoothing we do not need to run the
      //   - resize
      //   - boxplot
      //   - div / N
      // twice on the same data
      p = I;
      p_sub = I_sub;
      mean_p = mean_I;
    } else {
      // actual structural transfer
      p_.convertTo(p, CV_32F);
      p = p / 255.0;
      cv::resize(p, p_sub, cv::Size(p.cols/s, p.rows/s), 0, 0, cv::INTER_NEAREST);
      mean_p = boxfilter(p_sub, r_sub);
      mean_p = mean_p / N;
    }


    cv::Mat mean_Ip = boxfilter(I_sub.mul(p_sub), r_sub);
    mean_Ip = mean_Ip / N;

    cv::Mat cov_Ip = mean_Ip - mean_I.mul(mean_p); // cov of (I, p) in each local patch

    cv::Mat mean_II = boxfilter(I_sub.mul(I_sub), r_sub) / N;
    cv::Mat var_I = mean_II - mean_I.mul(mean_I);

    cv::Mat a, b;
    a = cov_Ip / (var_I + eps);
    b = mean_p - a.mul(mean_I);

    a = boxfilter(a, r_sub) / N;
    b = boxfilter(b, r_sub) / N;

    cv::resize(a, a, I.size(), cv::INTER_LINEAR);
    cv::resize(b, b, p.size()), cv::INTER_LINEAR;

    q = a.mul(I) + b;
    q *= 255;
    q.convertTo(q, CV_8U);
  }


  /** @brief fast guided filter that blurs the image but preserves edges

  @param in Source image
  @param out Output image
  @param s subsampling ratio

   */
  void inline blur(
    const cv::Mat &in,
    cv::Mat &out,
    const size_t r,
    const double eps,
    const size_t s)
  {
    structureTransference(in, in, out, r, eps, s);
  }

}

#endif
