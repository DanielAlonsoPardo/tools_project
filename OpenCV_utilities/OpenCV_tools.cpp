#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "OpenCV_tools.hpp"

namespace OpenCV_tools
{
  using namespace std;
  string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
  }

  void image_check(Mat img) {
    if (!img.data)
      {
	cerr << "No image data \n"
	     << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
	exit(-1);
      }
  }

  void image_check(Mat img, string filename) {
    if (!img.data)
      {
	cerr << "Failed to load " << filename << ", no image data \n"
	     << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
	exit(-1);
      }
  }
}
