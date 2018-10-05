#include "functions.hpp"
#include <cmath>
#include <iostream>
cv::Mat createBoxFilter(int r){
   cv::Mat filtro(2*r+1, 2*r+1, CV_32FC1);

   for(int i=0; i<filtro.rows; i++){
      uchar *ptr=filtro.ptr<uchar>(i);
      for(int j=0; j<filtro.cols; j++){
         ptr[j]=1/pow(2*r+1, 2);
      }
   }
   return filtro;
}
void applyFilter(cv::Mat & in, cv::Mat & filtered, cv::Mat & filter){
   int r=(filter.rows - 1)/2;
   for(int i=r; i<filtered.rows - r; i++){
      uchar *ptr=filtered.ptr<uchar>(i);
      std::cout << "hola" << '\n';
      for(int j=r; j<filtered.cols - r; j++){
         cv::Mat a=in(cv::Rect(0, 0, filter.rows, filter.cols));

         // for(int k=0; k<filter.rows; k++){
         //
         //    uchar *ptr1=a.ptr<uchar>(k);
         //    uchar *ptr2=filter.ptr<uchar>(k);
         //
         //    for(int k1=0; k1<filter.cols; k1++){
         //       ptr[j]+=(ptr1[k1]*ptr2[k1]);
         //       std::cout << "hola1" << '\n';
         //    }
         // }
      }
   }
   std::cout << "hola2" << '\n';
}
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & out, int g, bool circular){
   cv::Mat filtered(in.rows, in.cols, CV_32FC1);

   applyFilter(in, filtered, filter);


   for(int i=0; i<out.rows; i++){
      uchar *ptr=out.ptr<uchar>(i);
      uchar *ptr1=in.ptr<uchar>(i);
      uchar *ptr2=filter.ptr<uchar>(i);
      for(int j=0; j<out.cols; j++){
         ptr[j]=(g+1)*ptr1[j] + ptr2[j]*g;
      }
   }
}
