#include "functions.hpp"
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
   for(int i=0; i<normalizado.size(); i++){
      normalizado[i]=(cumulative[i]-cumulative[0])*(255/(cumulative[255]-cumulative[0]));
   }
}
