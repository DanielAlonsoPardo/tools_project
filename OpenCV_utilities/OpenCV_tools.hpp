#pragma once

namespace OpenCV_tools
{
#include <stdlib.h>
#include <opencv2/opencv.hpp>
  using namespace std;
  using namespace cv;
  string type2str(int type);
  void image_check(Mat img);
  void image_check(Mat img, string filename);
}
