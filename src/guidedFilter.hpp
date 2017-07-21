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

  /** @brief applies the unnormalized boxfilter onto all channels of the image

    @param I input image
    @param r size of extension into all directions
    @param B boxfiltered blue channel
    @param G boxfiltered green channel
    @param R boxfiltered red channel
   */
  void inline boxfilterColor(
    const cv::Mat &I,
    size_t r,
    cv::Mat &B, cv::Mat &G, cv::Mat &R)
  {
    cv::Mat bgr[3];
    cv::split(I, bgr);
    B = boxfilter(bgr[0], r);
    G = boxfilter(bgr[1], r);
    R = boxfilter(bgr[2], r);
  }

  /** @brief multiplies each channel of the colorImage with the
             perChannel matrix

    @param colorImage given colorImage (3 channels)
    @param perChannel channel that has to be multiplied on all channels (1 channel)
  */
  cv::Mat inline mul(const cv::Mat &colorImage, const cv::Mat &perChannel) {
    assert(colorImage.channels() == 3);
    assert(perChannel.channels() == 1);
    cv::Mat result;
    cv::Mat bgr[3];
    cv::split(colorImage, bgr);
    bgr[0] = bgr[0].mul(perChannel);
    bgr[1] = bgr[1].mul(perChannel);
    bgr[2] = bgr[2].mul(perChannel);
    cv::merge(bgr, 3, result);
    return result;
  }

  // see documentation above
  void inline structureTransferenceGray(
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


  // see documentation above
  void inline structureTransferenceColor(
    const cv::Mat &I_,
    const cv::Mat &p_,
    cv::Mat &q,
    const size_t r,
    const double eps,
    const size_t s)
  {
    cv::Mat I, p;
    I_.convertTo(I, CV_32FC3);
    I = I / 255.0;

    p_.convertTo(p, CV_32F);
    p = p / 255.0;

    cv::Mat I_sub, p_sub;
    size_t r_sub = r / s;
    cv::resize(I, I_sub, cv::Size(I.cols/s, I.rows/s), 0, 0, cv::INTER_NEAREST);
    cv::resize(p, p_sub, cv::Size(I.cols/s, I.rows/s), 0, 0, cv::INTER_NEAREST);

    cv::Mat N(I_sub.rows, I_sub.cols, CV_32F, cv::Scalar(1));
    N = boxfilter(N, r_sub);

    cv::Mat mean_p = boxfilter(p_sub, r_sub) / N;

    cv::Mat mean_I_r, mean_I_g, mean_I_b;

    boxfilterColor(I_sub, r_sub, mean_I_b, mean_I_g, mean_I_r);

    mean_I_r = mean_I_r / N;
    mean_I_g = mean_I_g / N;
    mean_I_b = mean_I_b / N;

    cv::Mat mean_Ip_r, mean_Ip_g, mean_Ip_b;
    boxfilterColor(mul(I_sub, p_sub), r_sub, mean_Ip_b, mean_Ip_g, mean_Ip_r);
    mean_Ip_r = mean_Ip_r / N;
    mean_Ip_g = mean_Ip_g / N;
    mean_Ip_b = mean_Ip_b / N;

    // covariance of (I, p) in each local patch
    cv::Mat cov_Ip_r = mean_Ip_r - mean_I_r.mul(mean_p);
    cv::Mat cov_Ip_g = mean_Ip_g - mean_I_g.mul(mean_p);
    cv::Mat cov_Ip_b = mean_Ip_b - mean_I_b.mul(mean_p);

    // covariance of I in each local patch: the matrix Sigma i Eqn (14).
    // Note the variance in each local patch is a 3x3 symmetric matrix:
    //          rr, rg, rb
    //  Sigma = rg, gg, gb
    //          rb, gb, bb
    cv::Mat bgr[3];
    cv::split(I_sub, bgr);
    cv::Mat var_I_rr = boxfilter(bgr[0].mul(bgr[0]), r_sub) / N - mean_I_r.mul(mean_I_r);
    cv::Mat var_I_rg = boxfilter(bgr[0].mul(bgr[1]), r_sub) / N - mean_I_r.mul(mean_I_g);
    cv::Mat var_I_rb = boxfilter(bgr[0].mul(bgr[2]), r_sub) / N - mean_I_r.mul(mean_I_b);
    cv::Mat var_I_gg = boxfilter(bgr[1].mul(bgr[1]), r_sub) / N - mean_I_g.mul(mean_I_g);
    cv::Mat var_I_gb = boxfilter(bgr[1].mul(bgr[2]), r_sub) / N - mean_I_g.mul(mean_I_b);
    cv::Mat var_I_bb = boxfilter(bgr[2].mul(bgr[2]), r_sub) / N - mean_I_b.mul(mean_I_b);

    // --------------------
    size_t wid = p_sub.cols;
    size_t hei = p_sub.rows;

    cv::Mat t[] = {
      cv::Mat::ones(hei, wid, CV_32F),
      cv::Mat::ones(hei, wid, CV_32F),
      cv::Mat::ones(hei, wid, CV_32F)
    };
  	cv::Mat a;
  	cv::merge(t, 3, a);

    for (size_t y = 0; y < hei; ++y) {
      for (size_t x = 0; x < wid; ++x) {

        cv::Mat Sigma(3, 3, CV_32F);

        //          rr, rg, rb
        //  Sigma = rg, gg, gb
        //          rb, gb, bb
        Sigma.at<float>(0, 0) = var_I_rr.at<float>(y, x);
        Sigma.at<float>(1, 0) = var_I_rg.at<float>(y, x);
        Sigma.at<float>(2, 0) = var_I_rb.at<float>(y, x);
        Sigma.at<float>(0, 1) = var_I_rg.at<float>(y, x);
        Sigma.at<float>(1, 1) = var_I_gg.at<float>(y, x);
        Sigma.at<float>(2, 1) = var_I_gb.at<float>(y, x);
        Sigma.at<float>(0, 2) = var_I_rb.at<float>(y, x);
        Sigma.at<float>(1, 2) = var_I_gb.at<float>(y, x);
        Sigma.at<float>(2, 2) = var_I_bb.at<float>(y, x);

        cv::Vec3f cov_Ip;
        cov_Ip[0] = cov_Ip_b.at<float>(y,x);
        cov_Ip[1] = cov_Ip_g.at<float>(y,x);
        cov_Ip[2] = cov_Ip_r.at<float>(y,x);

        cv::Mat trans = (Sigma + eps * cv::Mat::eye(3, 3, CV_32F)).inv();
        cv::Mat res = trans * cv::Mat(cov_Ip);
        a.at<cv::Vec3f>(y, x) = cv::Vec3f(res);
      }
    }

    cv::Mat a_[3];
    cv::split(a, a_);

    cv::Mat b = mean_p -
      a_[0].mul(mean_I_b) -
      a_[1].mul(mean_I_g) -
      a_[2].mul(mean_I_r);

      /*
    cv::Mat mean_a_b = boxfilter(a_[0], r_sub) / N;
    cv::Mat mean_a_g = boxfilter(a_[1], r_sub) / N;
    cv::Mat mean_a_r = boxfilter(a_[2], r_sub) / N;
    */
    cv::Mat mean_a;
    cv::Mat mean_a_[] = {
      boxfilter(a_[0], r_sub) / N,
      boxfilter(a_[1], r_sub) / N,
      boxfilter(a_[2], r_sub) / N
    };
  	cv::merge(mean_a_, 3, mean_a);

    cv::Mat mean_b = boxfilter(b, r_sub) / N;

    cv::resize(mean_a, mean_a, I.size(), cv::INTER_LINEAR);
    cv::resize(mean_b, mean_b, p.size(), cv::INTER_LINEAR);

    cv::Mat a_mean_lg[3];
    cv::split(mean_a, a_mean_lg);

    cv::Mat I__[3];
    cv::split(I, I__);

    cv::Mat q_[] = {
      a_mean_lg[0].mul(I__[0]) + mean_b,
      a_mean_lg[1].mul(I__[1]) + mean_b,
      a_mean_lg[2].mul(I__[2]) + mean_b
    };

    cv::merge(q_, 3, q);

    //cv::Mat a = cv::Mat::zeros(hei, wid, 3);

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
      assert(I_.channels() == p_.channels());
      assert(I_.channels() == 1 || I_.channels() == 3);
      if (I_.channels() == 1) {
        structureTransferenceGray(I_, p_, q, r, eps, s);
      } else {
        cv::Mat gray;
        cv::cvtColor(I_, gray, cv::COLOR_BGR2GRAY);
        structureTransferenceColor(I_, gray, q, r, eps, s);
      }
  }

  /** @brief fast guided filter that blurs the image but preserves edges

  @param in Source image
  @param out Output image
  @param r local window radius
  @param eps regularization parameter
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
