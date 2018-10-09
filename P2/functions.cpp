#include "functions.hpp"
#include <cmath>
#include <iostream>
#include <cstdlib>
void obtenerSubImagen(cv::Mat & image, cv::Mat & subimage, int i, int j, int r){
   int i_arriba=i;//debo de tener en cuenta los pixeles de la fila i
   int j_izquierda=j-1;
   int j_derecha=j+1;
   int i_abajo=i+1;
   int i_elemento_cero=i-r;
   int j_elemento_cero=j-r;
   int contador1=0;
   int contador2=0;
   int contador3=0;
   int contador4=0;

   while(i_arriba >= 0 && contador1 < r){
      contador2=0;
      j_izquierda=j-1;
      while(j_izquierda >= 0 && contador2 < r){
         subimage.at<float>(i_arriba - i_elemento_cero, j_izquierda - j_elemento_cero)=image.at<float>(i_arriba, j_izquierda);
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;

      while(j_derecha < image.cols - 1 && contador3 < r){
         subimage.at<float>(i_arriba - i_elemento_cero, j_derecha - j_elemento_cero)=image.at<float>(i_arriba, j_derecha);
         j_derecha++;
         contador3++;
      }
      subimage.at<float>(i_arriba - i_elemento_cero, j - j_elemento_cero)=image.at<float>(i_arriba, j);
      i_arriba--;
      contador1++;
   }

   while(i_abajo < image.rows - 1 && contador4 < r){
      contador2=0;
      j_izquierda=j-1;
      while(j_izquierda >= 0 && contador2 < r){
         subimage.at<float>(i_abajo - i_elemento_cero, j_izquierda - j_elemento_cero)=image.at<float>(i_abajo, j_izquierda);
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;

      while(j_derecha < image.cols - 1 && contador3 < r){
         subimage.at<float>(i_abajo - i_elemento_cero, j_derecha - j_elemento_cero)=image.at<float>(i_abajo, j_derecha);
         j_derecha++;
         contador3++;
      }
      subimage.at<float>(i_abajo - i_elemento_cero, j - j_elemento_cero)=image.at<float>(i_abajo, j);
      i_abajo++;
      contador4++;
   }
}
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
   cv::Mat a(2*r+1, 2*r+1, CV_32FC1);
   for(int i=r; i<filtered.rows-r; i++){
      std::cout << "hola" << '\n';
      float *ptr=filtered.ptr<float>(i);
      for(int j=r; j<filtered.cols-r; j++){
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
   cv::imwrite("prueba2.png", a);
}
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & out, int g, bool circular){
   cv::Mat filtered(in.rows, in.cols, CV_32FC1);

   applyFilter(in, filtered, filter);

   for(int i=0; i<out.rows; i++){
      float *ptr=out.ptr<float>(i);
      float *ptr1=in.ptr<float>(i);
      float *ptr2=filter.ptr<float>(i);
      for(int j=0; j<out.cols; j++){
         ptr[j]=(g+1)*ptr1[j] + ptr2[j]*g;
      }
   }
}
