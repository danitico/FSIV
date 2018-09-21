#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <vector>
#include <opencv2/core/core.hpp> //core routines
void getHistogram(cv::Mat &image, std::vector<int> & histogram);
void getCumulativeHistogram(std::vector<int> & original, std::vector<int> & cumulative);
void normalize(std::vector<int> cumulative, std::vector<int> & normalizado);
void equalization(cv::Mat image, std::vector<int> & normalizado, std::string & newImage);
void equalizationImage(cv::Mat & image, std::string & newImage);
#endif
