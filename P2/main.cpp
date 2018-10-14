#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <exception>
#include <string>
#include "functions.hpp"
const cv::String keys =
   "{help h usage ? |      | print this message                }"
   "{radius r       |1     | It controls the size of the filter}"
   "{gain g         |1     | biequalization of the image       }"
   "{filter f       |0     | It selects the type of filter     }"
   "{circular c     |      | The border pixels will be procesed using the opposite pixels}"
   "{hsv            |      | hsv stuff                         }"
   "{interactive i  |      | interactive mode                  }"
   "{@image1        |<none>| The input image                   }"
   "{@image2        |<none>| The output image                  }"
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
      int g=parser.get<int>("g");
      int f=parser.get<int>("f");
      bool hsv=parser.has("hsv");
      bool i=parser.has("i");
      std::string image1=parser.get<std::string>("@image1");
      std::string image2=parser.get<std::string>("@image2");

      if(!parser.check()){
         parser.printErrors();
         parser.printMessage();
         return 0;
      }
      if(i){
         int prueba=0;
         cv::namedWindow("Prueba"); // Create Window
         cv::createTrackbar("Barritas", "Prueba", &prueba, 10, on_trackbar);
         on_trackbar(prueba, 0);
         cv::waitKey(0);
      }
      else if(!hsv){
         cv::Mat picture1=cv::imread(image1, CV_LOAD_IMAGE_GRAYSCALE);
         if(picture1.rows==0){
            std::cout << "Error reading image 1" << '\n';
            return 0;
         }
         picture1.convertTo(picture1, CV_32FC1);
         cv::Mat filtered(picture1.rows, picture1.cols, CV_32FC1);
         cv::Mat enhanced(picture1.rows, picture1.cols, CV_32FC1);
         cv::Mat filtro;

         if(f==0){
            filtro=createBoxFilter(r);
         }
         else{
            std::cout << "Hay que implementarlo ;)" << '\n';
         }

         convolve(picture1, filtro, filtered);
         enhance(picture1, filtered, enhanced, g);
         cv::imwrite(image2, enhanced);
      }
      else{
         cv::Mat picture2=cv::imread(image1, CV_LOAD_IMAGE_ANYCOLOR);
         cv::Mat out;
         RGB(picture2, out, r, g, f);
         cv::imwrite(image2, out);
      }
   }
   catch(std::exception& e){
      std::cerr << "Exception: " << e.what() << std::endl;
      retCode=EXIT_FAILURE;
   }
   return retCode;
}
