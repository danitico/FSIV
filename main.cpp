#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <iostream>
#include <cstdlib>
using namespace std;
int main(int argc, char **argv){
   if(argc!=2){
      std::cout << "Error al llamar al programa" << '\n';
   }

   cv::Mat image=cv::imread(argv[1], IMREAD_GRAYSCALE);
   if(image.rows==0){
      std::cout << "Error reading image" << '\n';
   }

   // Do equalization
}
