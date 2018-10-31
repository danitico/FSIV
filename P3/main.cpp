#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <exception>
#include <string>
#include "functions.hpp"
const cv::String keys =
   "{help h usage ? |      | print this message                }"
   "{@rows          |<none>| Rows of the chessboard            }"
   "{@cols          |<none>| Columns of the chessboard         }"
   "{@size          |<none>| Size                              }"
   "{@file          |<none>| yaml file                         }"
   "{@video         |<none>| The input video                   }"
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
      int rows=parser.get<int>("@rows");
      int cols=parser.get<int>("@cols");
      int size=parser.get<int>("@size");
      std::string file=parser.get<std::string>("@file");
      std::string input=parser.get<std::string>("@video");

      if(!parser.check()){
         parser.printErrors();
         parser.printMessage();
         return 0;
      }
      cv::Mat frame;
      cv::VideoCapture video(input);

      while(video.read(frame)){
         cv::Mat corners;
         cv::Size xy(rows,cols);
         cv::Mat frameButInGrey;
         cv::cvtColor(frame, frameButInGrey, cv::COLOR_BGR2GRAY);

         bool patternfound=cv::findChessboardCorners(frame,xy,corners);
         cv::cornerSubPix(frameButInGrey,corners,cv::Size(5,5), cv::Size(-1,-1), cv::TermCriteria());
         cv::Mat rvec,tvec;
         drawChessboardCorners(frame, xy, cv::Mat(corners), patternfound);
         // cv::solvePnP(,,l.cameraMatrix1,l.distortionCoefficient1,rvec,tvec);
         cv::imshow("Live", frame);
         if(cv::waitKey(5)>=0){
            break;
         }
      }
   }
   catch(std::exception& e){
      std::cerr << "Exception: " << e.what() << std::endl;
      retCode=EXIT_FAILURE;
   }
   return retCode;
}
