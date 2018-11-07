/* 
   (c) Fundamentos de Sistemas Inteligenties en Vision
   University of Cordoba, Spain  
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
//#include <unistd.h>
#include <ctype.h>
#include <cmath>

#include <tclap/CmdLine.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

using namespace std;
using namespace cv;

/*
  Use CMake to compile
*/

int main (int argc, char * const argv[])
{
  /* Default values */
  bool cameraInput=false;
  bool useWhitePatchCorrecction=false;
  bool useChromaticCooridnates=false;
  int threshold;
  const char * filein = 0;
  const char * fileout = 0;
  char opt;

  int optind = 1; // TODO: mjmarin!!!

  TCLAP::CmdLine cmd("Video segmentation", ' ', "0.1");

  TCLAP::ValueArg<std::string> filename("v", "videoname", "Video path", true, "video.avi", "string");
  cmd.add(filename);
  TCLAP::ValueArg<std::string> outname("o", "outfilename", "Output video path", true, "out.avi", "string");
  cmd.add(outname);
  
  TCLAP::ValueArg<int> thres("t", "threshold", "Threshold value", false, 13, "int");
  cmd.add(thres);
  
  // Parse input arguments
  cmd.parse(argc, argv);

  filein = filename.getValue().c_str();
  fileout = outname.getValue().c_str();

  std::cout << "Input stream:" << filein << endl;
  std::cout << "Output:" << fileout << endl;

  VideoCapture input;
    
  if (cameraInput)
    input.open(atoi(filein));
  else  
    input.open(filein);
  
  
  if (!input.isOpened())
  {
    cerr << "Error: the input stream is not opened.\n";
    abort();
  }

  Mat inFrame;
  bool wasOk = input.read(inFrame);
  if (!wasOk)
  {
    cerr << "Error: could not read any image from stream.\n";
    abort();
  }
  
  double fps=25.0;
  if (!cameraInput)
    fps=input.get(CV_CAP_PROP_FPS);
  
  Mat outFrame = Mat::zeros(inFrame.size(), CV_8UC1);
  
  VideoWriter output;
  output.open(fileout, CV_FOURCC('M','J','P','G'), fps, inFrame.size(), 0);
  if (!output.isOpened())
  {
    cerr << "Error: the ouput stream is not opened.\n";
  }  

  int frameNumber=0;
  int key = 0;

  cv::namedWindow("Output");

  while(wasOk && key!=27)
  {
     frameNumber++;
        
     cv::imshow ("Input", inFrame);    
     
	 // Do your processing
	 // TODO

	 outFrame = inFrame; // CHANGE ME!

     cv::imshow ("Output", outFrame);
     
     wasOk=input.read(inFrame);
     key = cv::waitKey(20);
     
  }           
  return 0;
}
