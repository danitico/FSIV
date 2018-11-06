#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/core/types_c.h>
#include <cassert>
#include <iostream>
#include <exception>
#include <unistd.h>
#include <string>
#include "functions.hpp"
const cv::String keys =
   "{help h usage ? |      | print this message                        }"
   "{@rows          |<none>| Rows of the chessboard                    }"
   "{@cols          |<none>| Columns of the chessboard                 }"
   "{@size          |<none>| Size                                      }"
   "{@file          |<none>| yaml file                                 }"
   "{@video         |<none>| The input video                           }"
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

      cv::FileStorage descriptor(file, cv::FileStorage::READ);
      cv::Mat cameraMatrix, distortionCoefficients, image_points;
      descriptor["camera_matrix"] >> cameraMatrix;
      descriptor["distortion_coefficients"] >> distortionCoefficients;

      std::vector<cv::Point3f> objectPoints;
      for(float y=0; y<cols; y++){
         for(float x=0; x<rows; x++){
            objectPoints.push_back(cv::Point3f(x*size, y*size, 0));
         }
      }

      std::vector<cv::Point3f> puntos3Dlineas;
      puntos3Dlineas.push_back(cv::Point3f((rows/2)*size,(cols/2)*size,0));
      puntos3Dlineas.push_back(cv::Point3f(((rows/2)+1)*size,(cols/2)*size,0));
      puntos3Dlineas.push_back(cv::Point3f((rows/2)*size,((cols/2)+1)*size,0));
      puntos3Dlineas.push_back(cv::Point3f((rows/2)*size,(cols/2)*size,-size));

      cv::VideoCapture video;
      cv::Mat frame;

      if(input=="0"){
         video.open(0);
      }
      else{
         video.open(input);
      }

      while(video.read(frame)){
         std::vector<cv::Point2f> corners;
         std::vector<cv::Point2f> puntos(0);
         cv::Mat a, GreyFrame, rvec, tvec;
         cv::Size patternSize(rows,cols);

         bool patternfound=cv::findChessboardCorners(frame, patternSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH +
                                                                                  cv::CALIB_CB_NORMALIZE_IMAGE +
                                                                                  cv::CALIB_CB_FAST_CHECK);
         if(patternfound){
            cv::cvtColor(frame, GreyFrame, cv::COLOR_RGB2GRAY);
            cv::cornerSubPix(GreyFrame, corners, cv::Size(5, 5), cv::Size(-1,-1), cv::TermCriteria());
            cv::solvePnP(cv::Mat(objectPoints), cv::Mat(corners), cameraMatrix, distortionCoefficients, rvec, tvec);

            cv::projectPoints(puntos3Dlineas, rvec, tvec, cameraMatrix, distortionCoefficients, puntos);

            cv::line(frame, puntos[0], puntos[1], cv::Scalar(255, 0, 0), 3);
            cv::line(frame, puntos[0], puntos[2], cv::Scalar(0, 255, 0), 3);
            cv::line(frame, puntos[0], puntos[3], cv::Scalar(0, 0, 255), 3);
         }

         cv::imshow("Augmented Reality", frame);
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
