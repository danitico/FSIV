#include <opencv2/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <exception>
const cv::String keys =
   "{help h usage ? |      | print this message                }"
   "{radius r       |1     | It controls the size of the filter}"
   "{gain g         |1     | biequalization of the image       }"
   "{filter f       |0     | It selects the type of filter     }"
   "{circular c     |      | The border pixels will be procesed using the opposite pixels}"
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
   }
   catch(std::exception& e){
      std::cerr << "Exception: " << e.what() << std::endl;
      retCode=EXIT_FAILURE;
   }
   return retCode;
}
