#include <opencv2/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <exception>
#include "functions.hpp"
const cv::String keys =
   "{help h usage ? |      | print this message                }"
   "{hsv            |      | It does the HSV stuff             }"
   "{radius r       |0     | Apply a radius to the image       }"
   "{bipartition b  |      | biequalization of the image       }"
   "{@image1        |<none>| The input image                   }"
   "{@image2        |<none>| The output image                  }"
   "{@mask          |      | Mask applied to the equalization  }"
   ;
int main(int argc, char* const* argv){
   int retCode=EXIT_SUCCESS;

   try{
      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Image equalization");
      if(parser.has("help")){
         parser.printMessage();
         return 0;
      }
      int r=parser.get<int>("r");
      bool bipartition=parser.has("b");
      bool hsv=parser.has("hsv");
      std::string image1=parser.get<std::string>("@image1");
      std::string image2=parser.get<std::string>("@image2");
      std::string mask=parser.get<std::string>("@mask");

      if(!parser.check()){
         parser.printErrors();
         return 0;
      }

      cv::Mat picture1=cv::imread(image1, cv::IMREAD_GRAYSCALE);
      if(picture1.rows==0){
         std::cout << "Error reading image 1" << '\n';
         return 0;
      }

      if(r==0 && bipartition==false && mask=="" && hsv==false){
         std::cout << "meow" << '\n';
         equalizationImage(picture1, image2);
      }
      else{
         if(r==0 && bipartition==false && mask!="" && hsv==false){
            cv::Mat picture2=cv::imread(mask, cv::IMREAD_GRAYSCALE);
            if(picture2.rows==0){
               std::cout << "Error reading image 2" << '\n';
               return 0;
            }
            std::cout << "meow1" << '\n';
            equalizationImageWithMask(picture1, picture2, image2);
         }
         else{
            if(r>0 && bipartition==false && mask=="" && hsv==false){
               std::cout << "meow2" << '\n';
               equalizationImageSlides(picture1, image2, r);
            }
            else{
               if(r>0 && bipartition==false && mask!="" && hsv==false){
                  cv::Mat picture2=cv::imread(mask, cv::IMREAD_GRAYSCALE);
                  if(picture2.rows==0){
                     std::cout << "Error reading image 3" << '\n';
                     return 0;
                  }
                  std::cout << "meow3" << '\n';
                  equalizationImageSlidesAndMask(picture1, picture2, image2, r);
               }
               else{
                  if(r==0 && bipartition==false && mask=="" && hsv==true){
                     cv::Mat picture3=cv::imread(image1);
                     if(picture3.rows==0){
                        std::cout << "Error reading image 1" << '\n';
                        return 0;
                     }
                     RGB(picture3, image2);
                  }
                  else{
                     if(r==0 && bipartition==true && mask=="" && hsv==false){
                        biequalizationImage(picture1, image2);
                     }
                     else{
                        if(r==0 && bipartition==true && mask!="" && hsv==false){
                           cv::Mat picture2=cv::imread(mask, cv::IMREAD_GRAYSCALE);
                           if(picture2.rows==0){
                              std::cout << "Error reading image 3" << '\n';
                              return 0;
                           }
                           biequalizationImagewithMask(picture1, picture2, image2);
                        }
                        else{
                           if(r>0 && bipartition==true && mask=="" && hsv==false){
                              biequalizationImageSlides(picture1, r, image2);
                           }
                           else{
                              std::cout << "Opciones incompatibles" << '\n';
                              parser.printMessage();
                              return 0;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }
   catch(std::exception& e){
      std::cerr << "Exception: " << e.what() << std::endl;
      retCode=EXIT_FAILURE;
   }
//---------------------------------------------------------------
   // biequalizationImage(image, "hola.png");
//---------------------------------------------------------------
   return retCode;
}
