#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <vector>
#include <opencv2/core/core.hpp> //core routines
void getHistogram(cv::Mat &image, std::vector<int> & histogram);
void getHistogramSlides(cv::Mat image, std::vector<int> & histogram, int i, int j, int r);
void getCumulativeHistogram(std::vector<int> & original, std::vector<int> & cumulative);
void normalize(std::vector<int> cumulative, std::vector<int> & normalizado, int nuevo_minimo=0, int nuevo_maximo=255);
void equalization(cv::Mat image, std::vector<int> & normalizado, std::string newImage);
void equalizationMask(cv::Mat image, cv::Mat mask, std::vector<int> & normalizado, std::string newImage);
void equalizationImage(cv::Mat & image, std::string newImage);
void equalizationImageWithMask(cv::Mat & image, cv::Mat & mask, std::string newImage);
void equalizationImageSlides(cv::Mat & image, std::string newImage, int r);
void equalizationImageSlidesAndMask(cv::Mat & image, cv::Mat & mask, std::string newImage, int r);
void RGB(cv::Mat image, std::string newImage);
void biequalizationImage(cv::Mat image, std::string newImage);
int calcularMediana(std::vector<int> a);
#endif
