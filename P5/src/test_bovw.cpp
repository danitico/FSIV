#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <tclap/CmdLine.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include "common_code.hpp"
#define IMG_WIDTH 300
int main(int argc, char **argv){
   TCLAP::CmdLine cmd("Train and test a BoVW model", ' ', "0.0");

	TCLAP::ValueArg<std::string> filename("", "image", "Image to categorize", false, "", "pathname");
	cmd.add(filename);

   TCLAP::ValueArg<std::string> video("", "video", "Video to categorize", false, "", "pathname");
   cmd.add(video);

   TCLAP::ValueArg<bool> webcam("", "webcam", "Video to categorize", false, "", "bool");
   cmd.add(webcam);

	TCLAP::ValueArg<std::string> classifierFile("", "classifier_name", "Path to the classifier", true, "", "pathname");
	cmd.add(classifierFile);

	TCLAP::ValueArg<std::string> dictionaryFile("", "dict_name", "Path to the dictionary", true, "", "pathname");
	cmd.add(dictionaryFile);

   TCLAP::ValueArg<std::string> config("", "config_file", "Path to the config file", true, "", "pathname");
	cmd.add(config);

   TCLAP::ValueArg<std::string> descriptor("", "descriptor", "Descriptor to use", false, "SIFT", "string");
   cmd.add(descriptor);

   TCLAP::ValueArg<std::string> classifierToUse("", "classifier", "Classifier to use", false, "KNN", "string");
   cmd.add(classifierToUse);

	TCLAP::ValueArg<int> neighbours("", "neighbours", "Number of neighbours for KNN. Default 1", false, 1, "int");
	cmd.add(neighbours);

	cmd.parse(argc, argv);

   std::vector<int> siftScales{ 9, 13 }; // 5 , 9
   std::vector<std::string> categories;
   std::vector<int> samples_per_cat;
   load_dataset_information(config.getValue(), categories, samples_per_cat);

   cv::FileStorage dictFile;
   dictFile.open(dictionaryFile.getValue(), cv::FileStorage::READ);
   int keywords;
   dictFile["keywords"]>>keywords;
   cv::Ptr<cv::ml::KNearest> dict = cv::Algorithm::read<cv::ml::KNearest>(dictFile.root());
   dictFile.release();

   cv::Ptr<cv::ml::StatModel> classifier;
   if(classifierToUse.getValue()=="KNN"){
      cv::Ptr<cv::ml::KNearest> knnClassifier = cv::Algorithm::load<cv::ml::KNearest>(classifierFile.getValue());
      knnClassifier->setDefaultK(neighbours.getValue());
      classifier = knnClassifier;
   }
   else{
      if(classifierToUse.getValue()=="SVM"){
         cv::Ptr<cv::ml::SVM> svm = cv::Algorithm::load<cv::ml::SVM>(classifierFile.getValue());
         classifier = svm;
      }
   }

   if(filename.getValue()!=""){
      cv::Mat image=readImage(filename.getValue(), descriptor.getValue()=="PHOW");
      resize(image, image, cv::Size(IMG_WIDTH, round(IMG_WIDTH*image.rows / image.cols)));

      cv::Mat descriptorsMat=getDescriptors(image, descriptor.getValue(), siftScales);
      cv::Mat bovw = compute_bovw(dict, keywords, descriptorsMat);

      cv::Mat predicted_labels;
      classifier->predict(bovw, predicted_labels);

      cv::namedWindow(categories[predicted_labels.at<float>(0,0)]);

      if(descriptor.getValue()=="PHOW"){
         cv::cvtColor(image, image, cv::COLOR_HSV2BGR);
      }

      cv::imshow(categories[predicted_labels.at<float>(0,0)], image);
      cv::waitKey(0);
   }
   else if(video.getValue()!=""){
      // cv::namedWindow("Input");
      cv::VideoCapture input;
      if(webcam.getValue()){
         input.open(stoi(video.getValue()));
      }
      else{
         input.open(video.getValue());
      }

      cv::Mat inFrame;
      bool wasOk = input.read(inFrame);
      int key = 0;

      std::string pruebaxd="";
      cv::Mat inFrame1;
      while(wasOk && key!=27){
         // cv::imshow ("Input", inFrame);
         inFrame1=inFrame.clone();
         resize(inFrame1, inFrame1, cv::Size(IMG_WIDTH, round(IMG_WIDTH*inFrame1.rows / inFrame1.cols)));

         if(descriptor.getValue()=="PHOW"){
            cv::cvtColor(inFrame1, inFrame1, cv::COLOR_BGR2HSV);
         }

         cv::Mat descriptorsMat=getDescriptors(inFrame1, descriptor.getValue(), siftScales);
         cv::Mat bovw = compute_bovw(dict, keywords, descriptorsMat);

         cv::Mat predicted_labels;
         classifier->predict(bovw, predicted_labels);

         if(pruebaxd!=categories[predicted_labels.at<float>(0,0)]){
            std::cout << "In the video there is/are " << categories[predicted_labels.at<float>(0,0)] << std::endl;
            pruebaxd=categories[predicted_labels.at<float>(0,0)];
         }

         wasOk=input.read(inFrame);
         key = cv::waitKey(20);
      }

      std::cout << std::endl;
   }
   else{
      std::cout << "ERROR" << std::endl;
      exit(-1);
   }
}
