#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <vector>
#include <opencv2/core/core.hpp> //core routines
void getHistogram(cv::Mat &image);
void getCumulativeHistogram(std::vector<int> & original, std::vector<int> & cumulative);
void normalize(std::vector<int> & normalizado)
void equalization();
#endif
