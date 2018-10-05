#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <opencv2/core/core.hpp>
cv::Mat createBoxFilter(int r);
cv::Mat createGaussianFilter(int r);
void applyFilter(cv::Mat &in, cv::Mat & filtered, cv::Mat & filter);
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & out, int g, bool circular=false);
#endif
