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
static void on_trackbar(int threshold_value, void* ptr);
static void on_trackbar_1(int brightness, void* ptr);
static void on_trackbar_2(int contrast, void* ptr);
static void on_trackbar_3(int saturation, void* ptr);
static void on_trackbar_4(int hue, void* ptr);
static void on_trackbar_5(int gain, void* ptr);
int main (int argc, char * const argv[]){
   bool cameraInput;
   int sizeSE_value, threshold_value, brightness, contrast, saturation, hue, gain;
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

   TCLAP::ValueArg<bool> cameraFlag("c", "camera", "Activate the camera", false, false, "bool");
   cmd.add(cameraFlag);

   // Parse input arguments
   cmd.parse(argc, argv);

   filein = filename.getValue().c_str();
   fileout = outname.getValue().c_str();
   threshold_value = thres.getValue();
   sizeSE_value = sizeSE.getValue();
   cameraInput = cameraFlag.getValue();

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
   output.open(fileout, CV_FOURCC('M','P','E','G'), fps, inFrame.size(), 0);
   if(!output.isOpened()){
      cerr << "Error: the ouput stream is not opened.\n";
   }

   int frameNumber=0;
   int key = 0;
   bool wasOk2=true;

   cv::namedWindow("Input");
   cv::namedWindow("Output with Color");
   createTrackbar("Threshold Value", "Output with Color", &threshold_value, 255, on_trackbar, (void*)&outFrame);

   if(cameraInput){
      brightness = saturation = gain = 64;
      contrast = 32;
      hue = 180;
      createTrackbar("Brightness", "Input", &brightness, 128, on_trackbar_1, (void*)&input);
      on_trackbar_1(brightness, (void*)&input);
      createTrackbar("Contrast", "Input", &contrast, 64, on_trackbar_2, (void*)&input);
      on_trackbar_2(contrast, (void*)&input);
      createTrackbar("Saturation", "Input", &saturation, 128, on_trackbar_3, (void*)&input);
      on_trackbar_3(saturation, (void*)&input);
      createTrackbar("Hue", "Input", &hue, 360, on_trackbar_4, (void*)&input);
      on_trackbar_4(hue, (void*)&input);
      createTrackbar("Gain", "Input", &gain, 128, on_trackbar_5, (void*)&input);
      on_trackbar_5(gain, (void*)&input);
   }

   Mat opening, closing, siguiente, gray, structureElement, mask;
   if(sizeSE_value!=0){
      structureElement=getStructuringElement(MORPH_RECT, Size(2*sizeSE_value+1, 2*sizeSE_value+1));
   }

   while(wasOk && key!=27){
      frameNumber++;

      cv::imshow ("Input", inFrame);

      if(wasOk2){
         if(!cameraInput){
            absdiff(inFrame1, inFrame, outFrame);
         }
         else{
            wasOk=input.read(siguiente);
            absdiff(siguiente, inFrame, outFrame);
         }
         cvtColor(outFrame, gray, COLOR_RGB2GRAY);

         mask=gray.clone();
         on_trackbar(threshold_value, (void*)&mask);

         if(sizeSE_value!=0){
            morphologyEx(mask.clone(), opening, MORPH_OPEN, structureElement);
            morphologyEx(mask.clone(), closing, MORPH_CLOSE, structureElement);
            mask = opening + closing;
         }

         cv::imshow ("MASK", mask);

         cv::Mat objects;
         inFrame.copyTo(objects, mask);

         if(cv::waitKey(20)==' '){
            cv::imwrite("out_" + to_string(frameNumber) + ".png", objects);
         }
         cv::imshow("Output with Color", objects);

         output.write(mask);
      }

      wasOk=input.read(inFrame);
      if(!cameraInput){
         wasOk2=input1.read(inFrame1);
      }
      key = cv::waitKey(20);
   }
   return 0;
}
static void on_trackbar(int threshold_value, void* ptr){
   Mat *mask = (Mat*)ptr;
   threshold(mask->clone(), *mask, threshold_value, 255, THRESH_BINARY);
}
static void on_trackbar_1(int brightness, void* ptr){
   double value = (double)brightness;
   VideoCapture *input = (VideoCapture*)ptr;
   input->set(CAP_PROP_BRIGHTNESS, value/128.0);
}
static void on_trackbar_2(int contrast, void* ptr){
   double value = (double)contrast;
   VideoCapture *input = (VideoCapture*)ptr;
   input->set(CAP_PROP_CONTRAST, value/64.0);
}
static void on_trackbar_3(int saturation, void* ptr){
   double value = (double)saturation;
   VideoCapture *input = (VideoCapture*)ptr;
   input->set(CAP_PROP_SATURATION, value/128.0);
}
static void on_trackbar_4(int hue, void* ptr){
   double value = (double)hue;
   VideoCapture *input = (VideoCapture*)ptr;
   input->set(CAP_PROP_HUE, value/360.0);
}
static void on_trackbar_5(int gain, void* ptr){
   double value = (double)gain;
   VideoCapture *input = (VideoCapture*)ptr;
   input->set(CAP_PROP_GAIN, value/128.0);
}
