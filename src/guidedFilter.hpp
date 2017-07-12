#ifndef GFG_H
#define GFG_H
#include <iostream>
#include <opencv2/opencv.hpp>

namespace fgf {

  /** @brief fast guided filter that blurs the image but preserves edges

  @param in Source image
  @param out Output image
  @param s subsampling ratio

   */
  void inline blur(
    const cv::Mat &in,
    cv::Mat &out,
    const size_t s,
    const double eps,
    const size_t scale)
  {

    out = in;
  }


  /**  @brief transfers structure of one image onto the other

  */
  void inline structureTransferene(
    const cv::Mat &I,
    const cv::Mat &p,
    const cv::Mat &q,
    const size_t s,
    const double eps,
    const size_t scale)
  {

  }

}

#endif
