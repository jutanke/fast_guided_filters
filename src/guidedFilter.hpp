#ifndef GFG_H
#define GFG_H
#include <iostream>
#include <opencv2/opencv.hpp>

namespace fgf {

  /**  @brief transfers structure of one image onto the other

    @param I guidance image
    @param p filtering input image
    @param q filter output image
    @param r local window radius
    @param eps regularization parameter
    @param s subsampling ratio 
  */
  void inline structureTransference(
    const cv::Mat &I,
    const cv::Mat &p,
    cv::Mat &q,
    const size_t r,
    const double eps,
    const size_t s)
  {

    cv::Mat I_sub, p_sub;
    size_t r_sub = r / s;
    cv::Size r_p_size(r_sub, r_sub);

    cv::Mat mean_I, mean_p;

    cv::resize(I, I_sub, cv::Size(I.cols/s, I.rows/s), 0, 0, cv::INTER_NEAREST);
    cv::blur(I_sub, mean_I, r_p_size);

    if (&I == &p) {
      p_sub = I_sub;
      mean_p = mean_I;
    } else {
      cv::resize(p, p_sub, cv::Size(p.cols/s, p.rows/s), 0, 0, cv::INTER_NEAREST);
      cv::blur(p_sub, mean_p, r_p_size);
    }

    cv::Mat mean_Ip, cov_Ip;
    cv::blur(I_sub.mul(p_sub), mean_Ip, r_p_size);
    cov_Ip = mean_Ip - mean_I.mul(mean_p); // cov of (I, p) in each local patch

    cv::Mat mean_II, var_I;
    cv::blur(I_sub.mul(I_sub), mean_II, r_p_size);
    var_I = mean_II - mean_I.mul(mean_I);

    cv::Mat a, b;
    a = cov_Ip / (var_I + eps);
    b = mean_p - a.mul(mean_I);

    cv::Mat mean_a, mean_b;
    cv::blur(a, a, r_p_size);
    cv::blur(b, b, r_p_size);

    cv::resize(a, a, I.size());
    cv::resize(b, b, p.size());

    q = a.mul(I) + b;
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
