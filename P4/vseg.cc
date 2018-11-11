#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include <cmath>
#include <tclap/CmdLine.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
using namespace std;
using namespace cv;
static void on_trackbar(int, void*);
int main (int argc, char * const argv[]){
  /* Default values */
   bool cameraInput=false;
   bool useWhitePatchCorrecction=false;
   bool useChromaticCooridnates=false;
   int sizeSE_value, threshold_value;
   const char * filein = 0;
   const char * fileout = 0;
   char opt;

   int optind = 1;

   TCLAP::CmdLine cmd("Video segmentation", ' ', "0.1");

   TCLAP::ValueArg<std::string> filename("v", "videoname", "Video path", true, "video.avi", "string");
   cmd.add(filename);
   TCLAP::ValueArg<std::string> outname("o", "outfilename", "Output video path", true, "out.avi", "string");
   cmd.add(outname);

   TCLAP::ValueArg<int> thres("t", "threshold", "Threshold value", false, 13, "int");
   cmd.add(thres);

   TCLAP::ValueArg<int> sizeSE("s", "size", "Size of the structure Element", false, 0, "int");
   cmd.add(sizeSE);

   // Parse input arguments
   cmd.parse(argc, argv);

   filein = filename.getValue().c_str();
   fileout = outname.getValue().c_str();
   threshold_value = thres.getValue();
   sizeSE_value = sizeSE.getValue();

   std::cout << "Input stream:" << filein << endl;
   std::cout << "Output:" << fileout << endl;

   VideoCapture input, input1;

   if(cameraInput){
      input.open(atoi(filein));
   }
   else{
      input.open(filein);
      input1.open(filein);
   }


   if(!input.isOpened()){
      cerr << "Error: the input stream is not opened.\n";
      abort();
   }

   Mat inFrame, inFrame1;
   bool wasOk = input.read(inFrame);
   input1.read(inFrame1);
   input1.read(inFrame1);
   if(!wasOk){
      cerr << "Error: could not read any image from stream.\n";
      abort();
   }

   double fps=25.0;
   if(!cameraInput){
      fps=input.get(CV_CAP_PROP_FPS);
   }

   Mat outFrame = Mat::zeros(inFrame.size(), CV_8UC1);

   VideoWriter output;
   // std::cout << inFrame.rows << " " << inFrame.cols << " " << inFrame.channels() << '\n';
   output.open(fileout, CV_FOURCC('H','2','6','4'), fps, inFrame.size(), 0);
   if(!output.isOpened()){
      cerr << "Error: the ouput stream is not opened.\n";
   }

   int frameNumber=0;
   int key = 0;
   bool wasOk2=true;

   cv::namedWindow("Output");
   cv::namedWindow("Output with Color");
   createTrackbar("Threshold Value", "Output", &threshold_value, 255, on_trackbar, (void*)&outFrame);

   Mat opening, closing, siguiente, gray, structureElement;
   if(sizeSE_value!=0){
      structureElement=getStructuringElement(MORPH_RECT, Size(sizeSE_value, sizeSE_value));
   }

   while(wasOk && key!=27){
      frameNumber++;

      cv::imshow ("Input", inFrame);

      if(wasOk2 && !cameraInput){
         absdiff(inFrame1, inFrame, outFrame);
         on_trackbar(threshold_value, (void*)&outFrame);

         if(sizeSE_value!=0){
            morphologyEx(outFrame.clone(), opening, MORPH_OPEN, structureElement);
            morphologyEx(outFrame.clone(), closing, MORPH_CLOSE, structureElement);
            outFrame=opening + closing;
         }

         if(cv::waitKey(20)==32){
            cv::imwrite("out_" + to_string(frameNumber) + ".png", outFrame);
         }

         cv::imshow("Output with Color", outFrame);
         cvtColor(outFrame, gray, COLOR_RGB2GRAY);
         threshold(gray.clone(), gray, 100, 255, THRESH_BINARY);
         output.write(gray);
         cv::imshow ("Output", gray);
      }
      // else if(wasOk2 && cameraInput){
      //    wasOk=input.read(siguiente);
      //
      //    absdiff(siguiente, inFrame, outFrame);
      //    threshold(outFrame, outFrame, threshold_value, 0, THRESH_TOZERO);
      //    opening=outFrame.clone(); closing=outFrame.clone();
      //
      //    erode(opening, opening, structureElement);
      //    dilate(opening, opening, structureElement);
      //    dilate(closing, closing, structureElement);
      //    erode(closing, closing, structureElement);
      //
      //    outFrame=opening + closing;
      //
      //    cvtColor(outFrame, gray, COLOR_RGB2GRAY);
      //    output.write(gray);
      //    cv::imshow ("Output", gray);
      // }

      wasOk=input.read(inFrame);
      if(!cameraInput){
         wasOk2=input1.read(inFrame1);
      }
      key = cv::waitKey(20);
   }
   return 0;
}
static void on_trackbar(int threshold_value, void* ptr){
   Mat *outFrame = (Mat*)ptr;
   threshold(outFrame->clone(), *outFrame, threshold_value, 0, THRESH_TOZERO);
}
