#ifndef GFG_H
#define GFG_H
#include <iostream>
#include <opencv2/opencv.hpp>

namespace fgf {

  /**  @brief transfers structure of one image onto the other

  */
  void inline structureTransference(
    const cv::Mat &I,
    const cv::Mat &p,
    cv::Mat &q,
    const size_t r,
    const double eps,
    const size_t s)
  {

    cv::Mat I_p, p_p;
    size_t r_p = r / s;
    cv::Size r_p_size(r_p, r_p);

    cv::Mat mean_I, mean_p;

    cv::resize(I, I_p, cv::Size(I.cols/s, I.rows/s), 0, 0, cv::INTER_NEAREST);
    cv::blur(I_p, mean_I, r_p_size);

    if (&I == &p) {
      p_p = I_p;
      mean_p = mean_I;
    } else {
      cv::resize(p, p_p, cv::Size(p.cols/s, p.rows/s), 0, 0, cv::INTER_NEAREST);
      cv::blur(p_p, mean_p, r_p_size);
    }

    cv::Mat corr_I;
    cv::blur(I, corr_I, )

    q = mean_I;
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
