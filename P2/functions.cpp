#include "functions.hpp"
#include <cmath>
#include <iostream>
#include <cstdlib>
void normalizarImagen(cv::Mat & filtered){
   float menor=filtered.at<float>(0,0), mayor=filtered.at<float>(0,0);
   for(int i=0; i<filtered.rows; i++){
      float *ptr=filtered.ptr<float>(i);
      for(int j=0; j<filtered.cols; j++){
         if(ptr[j]<menor){
            menor=ptr[j];
         }
         else if(ptr[j]>mayor){
            mayor=ptr[j];
         }
      }
   }

   float a=0.0;
   for(int i=0; i<filtered.rows; i++){
      float *ptr=filtered.ptr<float>(i);
      for(int j=0; j<filtered.cols; j++){
         a=(float)(ptr[j]-menor)/(float)(mayor-menor);
         ptr[j]=(255*a);
      }
   }
   // std::cout << "mayor: " << mayor << '\n';
   // std::cout << "menor: " << menor << '\n';
}
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
         std::cout << i_arriba - i_elemento_cero << " " << j_izquierda - j_elemento_cero << " valor: " << subimage.at<float>(i_arriba - i_elemento_cero, j_izquierda - j_elemento_cero) << '\n';
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;

      while(j_derecha < image.cols - 1 && contador3 < r){
         subimage.at<float>(i_arriba - i_elemento_cero, j_derecha - j_elemento_cero)=image.at<float>(i_arriba, j_derecha);
         std::cout << i_arriba - i_elemento_cero << " " << j_derecha - j_elemento_cero << " valor: " << subimage.at<float>(i_arriba - i_elemento_cero, j_derecha - j_elemento_cero) << '\n';
         j_derecha++;
         contador3++;
      }
      subimage.at<float>(i_arriba - i_elemento_cero, j - j_elemento_cero)=image.at<float>(i_arriba, j);
      std::cout << i_arriba - i_elemento_cero << " " << j - j_elemento_cero << " valor: " << subimage.at<float>(i_arriba - i_elemento_cero, j - j_elemento_cero) << '\n';
      i_arriba--;
      contador1++;
   }

   while(i_abajo < image.rows - 1 && contador4 < r){
      contador2=0;
      j_izquierda=j-1;
      while(j_izquierda >= 0 && contador2 < r){
         subimage.at<float>(i_abajo - i_elemento_cero, j_izquierda - j_elemento_cero)=image.at<float>(i_abajo, j_izquierda);
         std::cout << i_abajo - i_elemento_cero << " " << j_izquierda - j_elemento_cero << " valor: " << subimage.at<float>(i_abajo - i_elemento_cero, j_izquierda - j_elemento_cero) << '\n';
         j_izquierda--;
         contador2++;
      }
      contador3=0;
      j_derecha=j+1;

      while(j_derecha < image.cols - 1 && contador3 < r){
         subimage.at<float>(i_abajo - i_elemento_cero, j_derecha - j_elemento_cero)=image.at<float>(i_abajo, j_derecha);
         std::cout << i_abajo - i_elemento_cero << " " << j_derecha - j_elemento_cero << " valor: " << subimage.at<float>(i_abajo - i_elemento_cero, j_derecha - j_elemento_cero) << '\n';
         j_derecha++;
         contador3++;
      }
      subimage.at<float>(i_abajo - i_elemento_cero, j - j_elemento_cero)=image.at<float>(i_abajo, j);
      std::cout << i_abajo - i_elemento_cero << " " << j - j_elemento_cero << " valor: " << subimage.at<float>(i_abajo - i_elemento_cero, j - j_elemento_cero) << '\n';
      i_abajo++;
      contador4++;
   }
}
cv::Mat createBoxFilter(int r){
   cv::Mat filtro(2*r+1, 2*r+1, CV_32FC1);

   for(int i=0; i<filtro.rows; i++){
      float *ptr=filtro.ptr<float>(i);
      for(int j=0; j<filtro.cols; j++){
         ptr[j]=1/pow(filtro.rows, 2);
      }
   }
   return filtro;
}
void applyFilter(cv::Mat & in, cv::Mat & filtered, cv::Mat & filter){
   int r=(filter.rows - 1)/2;
   for(int i=r; i<filtered.rows-r; i++){
      float *ptr=filtered.ptr<float>(i);
      for(int j=r; j<filtered.cols-r; j++){
         cv::Mat a(2*r+1, 2*r+1, CV_32FC1);
         obtenerSubImagen(in, a, i, j, r);
         std::cout << "++++++++++" << '\n';
         for(int k=0; k<filter.rows; k++){
            float *ptr1=a.ptr<float>(k);
            // float *ptr2=filter.ptr<float>(k);
            for(int k1=0; k1<filter.cols; k1++){
               std::cout << k << " " << k1 << " valor: " << ptr1[k1] << '\n';
               // ptr[j]+=(ptr1[k1]*ptr2[k1]);
            }
         }
         std::cout << "----------" << '\n';
         // std::cout << i << " " << j << "valor: " << ptr[j] << '\n';
      }
   }
   // normalizarImagen(filtered);
   // normalize(filtered, filtered, 0, 255, cv::NORM_MINMAX);
   // cv::Mat prueba(6, 8, CV_32FC1);
   // for(int i=0; i<6; i++){
   //    float *ptr=prueba.ptr<float>(i);
   //    for(int j=0; j<8; j++){
   //       if(j>3){
   //          ptr[j]=255;
   //       }
   //       else{
   //          ptr[j]=0;
   //       }
   //    }
   // }
   cv::imwrite("prueba2.png", filtered);
}
void convolve(cv::Mat & in, cv::Mat & filter, cv::Mat & out, int g, bool circular){
   cv::Mat filtered=in.clone();

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
