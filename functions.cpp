#include "functions.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
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
void equalization(cv::Mat image, std::vector<int> & normalizado, std::string newImage){
   for(int i=0; i<image.rows; i++){
      uchar *ptr=image.ptr<uchar>(i);
      for(int j=0; j<image.cols; j++){
         ptr[j]=normalizado[ptr[j]];
      }
   }
   cv::imwrite(newImage, image);
}
void equalizationMask(cv::Mat image, cv::Mat mask, std::vector<int> & normalizado, std::string newImage){
   for(int i=0; i<image.rows; i++){
      uchar *ptr=image.ptr<uchar>(i);
      uchar *ptr2=mask.ptr<uchar>(i);
      for(int j=0; j<image.cols; j++){
         if(ptr2[j]>0){
            ptr[j]=normalizado[ptr[j]];
         }
      }
   }
   cv::imwrite(newImage, image);
}
void equalizationImage(cv::Mat & image, std::string newImage){
   std::vector<int> histogram(256, 0);
   std::vector<int> cumulative(256, 0);
   std::vector<int> normalizado(256, 0);

   getHistogram(image, histogram);
   getCumulativeHistogram(histogram, cumulative);
   normalize(cumulative, normalizado);
   equalization(image, normalizado, newImage);
}
void equalizationImageWithMask(cv::Mat & image, cv::Mat & mask, std::string newImage){
   std::vector<int> histogram(256, 0);
   std::vector<int> cumulative(256, 0);
   std::vector<int> normalizado(256, 0);

   getHistogram(image, histogram);
   getCumulativeHistogram(histogram, cumulative);
   normalize(cumulative, normalizado);
   equalizationMask(image, mask, normalizado, newImage);
}
/*
void equalizationImageSlides(cv::Mat & image, std::string newImage, int r){
   cv::Mat image2(image.rows, image.cols, image.type());
   std::vector<int> histogram;
   std::vector<int> cumulative;
   std::vector<int> normalizado;
   r=2*r + 1;

   for(int i=0; i<image2.rows; i++){
      uchar *ptr=image.ptr<uchar>(i);
      uchar *ptr2=image2.ptr<uchar>(i);
      for(int j=0; j<image2.cols; j++){
         histogram.resize(256, 0);
         cumulative.resize(256, 0);
         normalizado.resize(256, 0);

         getHistogramSlides(image, histogram, i, j, r);
         getCumulativeHistogram(histogram, cumulative);
         normalize(cumulative, normalizado);
         ptr2[j]=normalizado[ptr[j]];
         // std::cout << ptr[j] << '\n';
      }
   }
   cv::imwrite(newImage, image2);
}
*/
/*
void getHistogramSlides(cv::Mat image, std::vector<int> & histogram, int i, int j, int r){
   uchar *puntero=image.ptr<uchar>(i);
   histogram[puntero[j]]++;

   int i_arriba=i-1;
   int j_izquierda=j-1;
   int j_derecha=j+1;
   int i_abajo=i+1;
   int contador1=0;
   int contador2=0;
   int contador3=0;
   int contador4=0;
   uchar *ptr1=image.ptr<uchar>(j);
   uchar *ptr2=image.ptr<uchar>(i_arriba);

   while(contador1 < r){
      ptr2=image.ptr<uchar>(i_arriba);
      contador2=0;
      j_izquierda=j-1;
      while(contador2 < r){
         histogram[ptr2[j_izquierda]]++;
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;
   }
      while(contador3 < r){
         histogram[ptr2[j_derecha]]++;
         j_derecha++;
         contador3++;
      histogram[ptr1[i_arriba]]++;
      i_arriba--;
      contador1++;
   }


   while(contador4 < r){
      ptr2=image.ptr<uchar>(i_abajo);
      contador2=0;
      j_izquierda=j-1;
      while(contador2 < r){
         histogram[ptr2[j_izquierda]]++;
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;
      while(contador3 < r){
         histogram[ptr2[j_derecha]]++;
         j_derecha++;
         contador3++;
      }
      histogram[ptr1[i_abajo]]++;
      i_abajo++;
      contador4++;
   }
}*/
void intento1(cv::Mat & image, std::string newImage, int r){
   cv::Mat image2=image;
   std::vector<int> histogram;
   std::vector<int> cumulative;
   std::vector<int> normalizado;
   histogram.resize(256, 0);
   cumulative.resize(256, 0);
   normalizado.resize(256, 0);
   r=2*r + 1;

   getHistogram(image, histogram);
   getCumulativeHistogram(histogram, cumulative);
   normalize(cumulative, normalizado);
   for(int i=r; i<image2.rows - r; i++){
      uchar *ptr=image.ptr<uchar>(i);
      uchar *ptr2=image2.ptr<uchar>(i);
      for(int j=r; j<image2.cols - r; j++){
         ptr2[j]=normalizado[ptr[j]];
      }
   }
   cv::imwrite(newImage, image2);
}
void RGB(cv::Mat image, std::string newImage){
   cv::Mat HSV[3];
   std::vector<int> histogram(256, 0);
   std::vector<int> cumulative(256, 0);
   std::vector<int> normalizado(256, 0);

   cv::cvtColor(image, image, cv::COLOR_RGB2HSV);
   split(image, HSV);

   getHistogram(HSV[2], histogram);
   getCumulativeHistogram(histogram, cumulative);
   normalize(cumulative, normalizado);

   for(int i=0; i<HSV[2].rows; i++){
      uchar *ptr=HSV[2].ptr<uchar>(i);
      for(int j=0; j<HSV[2].cols; j++){
         ptr[j]=normalizado[ptr[j]];
      }
   }

   cv::Mat newHSV;
   cv::merge(HSV, 3, newHSV);
   cv::cvtColor(newHSV, newHSV, cv::COLOR_HSV2RGB);
   cv::imwrite(newImage, newHSV);
}
