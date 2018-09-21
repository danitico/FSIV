#include <opencv2/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include "functions.hpp"
int main(int argc, char **argv){
   // if(argc!=2){
   //    std::cout << "Error al llamar al programa" << '\n';
   // }

   cv::Mat image=cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
   if(image.rows==0){
      std::cout << "Error reading image" << '\n';
   }

   // cv::Mat mask=cv::imread(argv[3], cv::IMREAD_GRAYSCALE);
   // if(mask.rows==0){
   //    std::cout << "Error reading image" << '\n';
   // }

//---------------------------------------------------------------
   // equalizationImage(image, argv[2]);

//---------------------------------------------------------------
   // equalizationImageWithMask(image, mask, argv[2]);
//---------------------------------------------------------------
   equalizationImageSlides(image, argv[2], 25);

   cv::Mat after=cv::imread(argv[2]);
   cv::namedWindow(argv[2]);
   cv::imshow(argv[2], after);


   // cv::namedWindow(argv[1]);
   // cv::imshow(argv[1], image);
   // cv::namedWindow("opencv");
   // cv::imshow("opencv", b);

   // cv::waitKey(0);
}
