#include <opencv2/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include "functions.hpp"
using namespace std;
int main(int argc, char **argv){
   // if(argc!=2){
   //    std::cout << "Error al llamar al programa" << '\n';
   // }

   cv::Mat image=cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
   if(image.rows==0){
      std::cout << "Error reading image" << '\n';
   }

   std::vector<int> histogram(256, 0);
   std::vector<int> cumulative(256, 0);
   getHistogram(image, histogram);

   for(int i=0; i<histogram.size(); i++){
      std::cout << "v[" << i << "]" << histogram[i] << '\n';
   }
   // Do equalization

   getCumulativeHistogram(histogram, cumulative);
   for(int i=0; i<cumulative.size(); i++){
      std::cout << "c[" << i << "]" << cumulative[i] << '\n';
   }

   std::vector<int> normalizado(256, 0);
   normalize(cumulative, normalizado);

   for(int i=0; i<normalizado.size(); i++){
      std::cout << "c[" << i << "]" << normalizado[i] << '\n';
   }
}
