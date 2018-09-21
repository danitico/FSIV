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

   std::string newImage;
   std::cout << "Type the name of the equalized image: ";
   std::cin >> newImage;

   cv::Mat b;
   cv::Mat negro;
   equalizationImage(image, newImage);
   cv::equalizeHist(image, b);


   cv::Mat after=cv::imread(newImage);

   cv::namedWindow(argv[1]);
   cv::imshow(argv[1], image);
   cv::namedWindow(newImage);
   cv::imshow(newImage, after);
   cv::namedWindow("opencv");
   cv::imshow("opencv", b);

   cv::waitKey(0);

}
