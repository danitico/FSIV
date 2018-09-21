#include "functions.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>
void getHistogram(cv::Mat &image, std::vector<int> & histogram){
   for(int i=0; i<image.rows; i++){
      uchar *ptr=image.ptr<uchar>(i);
      for(int j=0; j<image.cols; j++){
         histogram[ptr[j]]++;
      }
   }
}
void getCumulativeHistogram(std::vector<int> & original, std::vector<int> & cumulative){
   for(int i=0; i<original.size(); i++){
      if(i==0){
         cumulative[0]=original[0];
      }
      else{
         cumulative[i]=original[i]+cumulative[i-1];
      }
   }
}
void normalize(std::vector<int> cumulative, std::vector<int> & normalizado){
      float a=0.0;
      for(int i=0; i<normalizado.size(); i++){
         a=(float)(cumulative[i]-cumulative[0])/(float)(cumulative[255]-cumulative[0]);
         normalizado[i]=255*a;
      }
}
void equalization(cv::Mat image, std::vector<int> & normalizado, std::string & newImage){
   for(int i=0; i<image.rows; i++){
      uchar *ptr=image.ptr<uchar>(i);
      for(int j=0; j<image.cols; j++){
         ptr[j]=normalizado[ptr[j]];
      }
   }
   cv::imwrite(newImage, image);
}
void equalizationImage(cv::Mat & image, std::string & newImage){
   std::vector<int> histogram(256, 0);
   std::vector<int> cumulative(256, 0);
   std::vector<int> normalizado(256, 0);

   getHistogram(image, histogram);
   getCumulativeHistogram(histogram, cumulative);
   normalize(cumulative, normalizado);
   equalization(image, normalizado, newImage);
}
