#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <tclap/CmdLine.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include "common_code.hpp"
#define IMG_WIDTH 300
int main(int argc, char **argv){
   TCLAP::CmdLine cmd("Train and test a BoVW model", ' ', "0.0");

	TCLAP::ValueArg<std::string> filename("", "image", "Image to categorize", true, "", "pathname");
	cmd.add(filename);

	TCLAP::ValueArg<std::string> classifierFile("", "classifier", "Path to the classifier", true, "", "pathname");
	cmd.add(classifierFile);

	TCLAP::ValueArg<std::string> dictionaryFile("", "dict", "Path to the dictionary", true, "", "pathname");
	cmd.add(dictionaryFile);

   TCLAP::ValueArg<std::string> config("", "config_file", "Path to the config file", true, "", "pathname");
	cmd.add(config);

   TCLAP::ValueArg<std::string> descriptor("", "descriptor", "Descriptor to use", false, "SIFT", "string");
   cmd.add(descriptor);

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

   cv::Ptr<cv::ml::KNearest> knnClassifier = cv::Algorithm::load<cv::ml::KNearest>(classifierFile.getValue());
   knnClassifier->setDefaultK(neighbours.getValue());

   cv::Mat image = imread(filename.getValue(), cv::IMREAD_GRAYSCALE);
   if(image.rows==0){
      std::cout << "La imagen no existe" << std::endl;
      exit(-1);
   }
   resize(image, image, cv::Size(IMG_WIDTH, round(IMG_WIDTH*image.rows / image.cols)));

   cv::Mat descriptorsMat;
   if(descriptor.getValue()=="SIFT"){
      descriptorsMat=extractSIFTDescriptors(image);
   }
   else if(descriptor.getValue()=="SURF"){
      descriptorsMat=extractSURFDescriptors(image);
   }
   else if(descriptor.getValue()=="DSIFT"){
      descriptorsMat=extractDenseSIFTDescriptors(image, siftScales);
   }
   else{
      std::cout << "Ese tipo de descriptor no está disponible" << std::endl;
      exit(-1);
   }

   cv::Mat bovw = compute_bovw(dict, keywords, descriptorsMat);

   cv::Ptr<cv::ml::StatModel> classifier;
   classifier = knnClassifier;

   cv::Mat predicted_labels;
   classifier->predict(bovw, predicted_labels);
   cv::namedWindow(categories[predicted_labels.at<float>(0,0)]);
   cv::imshow(categories[predicted_labels.at<float>(0,0)], image);
   cv::waitKey(0);
}
