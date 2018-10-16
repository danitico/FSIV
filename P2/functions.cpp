#include "functions.hpp"
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <cstdlib>
void obtenerSubImagen(cv::Mat & image, cv::Mat & subimage, int i, int j, int r){
   int j_izquierda=j-1;
   int j_derecha=j+1;
   int i_elemento_cero=i-r;
   int j_elemento_cero=j-r;
   int contador1=-1;
   int contador2=0;
   int contador3=0;
   int contador4=0;

   for(int inicio=i; inicio>=0; inicio--){
      if(contador1 < r){
         float *ptr=subimage.ptr<float>(inicio - i_elemento_cero);
         float *ptr1=image.ptr<float>(inicio);
         ptr[j - j_elemento_cero]=ptr1[j];

         contador2=0;
         j_izquierda=j-1;
         while(contador2 < r){
            ptr[j_izquierda - j_elemento_cero]=ptr1[j_izquierda];
            contador2++;
         }
         contador3=0;
         j_izquierda--;
         j_derecha=j+1;

         while(contador3 < r){
            ptr[j_derecha - j_elemento_cero]=ptr1[j_derecha];
            j_derecha++;
            contador3++;
         }
         contador1++;
      }
      else{
         break;
      }
   }

   for(int inicio=i+1; inicio < image.rows; inicio++){
      if(contador4 < r){
         float *ptr=subimage.ptr<float>(inicio - i_elemento_cero);
         float *ptr1=image.ptr<float>(inicio);
         ptr[j - j_elemento_cero]=ptr1[j];

         contador2=0;
         j_izquierda=j-1;
         while(contador2 < r){
            ptr[j_izquierda - j_elemento_cero]=ptr1[j_izquierda];
            j_izquierda--;
            contador2++;
         }
         contador3=0;
         j_derecha=j+1;

         while(contador3 < r){
            ptr[j_derecha - j_elemento_cero]=ptr1[j_derecha];
            j_derecha++;
            contador3++;
         }
         contador4++;
      }
      else{
         break;
      }
   }
}
cv::Mat createBoxFilter(int r){
   cv::Mat filtro(2*r+1, 2*r+1, CV_32FC1, 0.0);

   for(int i=0; i<filtro.rows; i++){
      float *ptr=filtro.ptr<float>(i);
      for(int j=0; j<filtro.cols; j++){
         ptr[j]=1/pow(filtro.rows, 2);
      }
   }
   return filtro;
}
cv::Mat createGaussianFilter(int r){
   cv::Mat filtro(2*r+1, 2*r+1, CV_32FC1, 0.0);
   float sigma=1;
   const double pi = 3.1415926535897;

   for(int i=0; i<filtro.rows; i++){
      float *ptr=filtro.ptr<float>(i);
      for(int j=0; j<filtro.cols; j++){
         float a=((pow(i-r, 2)/sigma) + (pow(j-r, 2)/sigma))/2.0;
         a=-a;
         ptr[j]=(1.0/(2*pi*sigma))*exp(a);
      }
   }
   return filtro;
}
void applyFilter(cv::Mat & in, cv::Mat & filtered, cv::Mat & filter){
   int r=(filter.rows - 1)/2;
   for(int i=r; i<filtered.rows-r; i++){
      float *ptr=filtered.ptr<float>(i);
      for(int j=r; j<filtered.cols-r; j++){
         cv::Mat a(2*r+1, 2*r+1, CV_32FC1, 0.0);
         obtenerSubImagen(in, a, i, j, r);
         for(int k=0; k<filter.rows; k++){
            float *ptr1=a.ptr<float>(k);
            float *ptr2=filter.ptr<float>(k);
            for(int k1=0; k1<filter.cols; k1++){
               ptr[j]+=(ptr1[k1]*ptr2[k1]);
            }
         }
      }
   }
}
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & filtered, bool circular){
   applyFilter(in, filtered, filter);
}
void enhance(cv::Mat & in, cv::Mat & filtered, cv::Mat & enhanced, int g){
   for(int i=0; i<enhanced.rows; i++){
      float *ptr=enhanced.ptr<float>(i);
      float *ptr1=in.ptr<float>(i);
      float *ptr2=filtered.ptr<float>(i);
      for(int j=0; j<enhanced.cols; j++){
         ptr[j]=(g+1)*ptr1[j] - ptr2[j]*g;
      }
   }
}
void RGB(cv::Mat const & in, cv::Mat & out, int r, int g, int f){
   cv::Mat HSV[3];
   cv::Mat input=in.clone();
   cv::Mat filter;
   cv::Mat filtered(in.rows, in.cols, CV_32FC1, 0.0);
   cv::Mat enhanced(in.rows, in.cols, CV_32FC1, 0.0);

   cv::cvtColor(input, input, cv::COLOR_RGB2HSV);
   split(input, HSV);
   HSV[2].convertTo(HSV[2], CV_32FC1);

   if(f==0){
      filter=createBoxFilter(r);
   }
   else{
      filter=createGaussianFilter(r);
   }

   convolve(HSV[2], filter, filtered);
   enhance(HSV[2], filtered, enhanced, g);

   for(int i=0; i<HSV[2].rows; i++){
      float *ptr=HSV[2].ptr<float>(i);
      float *ptr1=enhanced.ptr<float>(i);
      for(int j=0; j<HSV[2].cols; j++){
         ptr[j]=ptr1[j];
      }
   }
   HSV[2].convertTo(HSV[2], CV_8UC1);

   cv::merge(HSV, 3, out);
   cv::cvtColor(out, out, cv::COLOR_HSV2RGB);
}
