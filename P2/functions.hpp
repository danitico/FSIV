#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <opencv2/core/core.hpp>
void obtenerSubImagen(cv::Mat & image, cv::Mat & subimage, int i, int j, int r);
cv::Mat createBoxFilter(int r);
cv::Mat createGaussianFilter(int r);
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & filtered, bool circular=false);
void enhance(cv::Mat & in, cv::Mat & filtered, cv::Mat & enhanced, int g);
void RGB(cv::Mat const & in, cv::Mat & out, int r, int g, int f);
#endif
