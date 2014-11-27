#pragma once

#include <opencv2/opencv.hpp>

namespace paint_tools
{
  using namespace std;
  using namespace cv;

  const Scalar red(0, 0, 255);
  const Scalar cyan(255, 255, 0);
  const Scalar light_green(100, 200, 100);
  const Scalar white(255, 255, 255);
  const Scalar black(0, 0, 0);

  template <class T>
  void paint_polygon(Mat& img, vector<T> points, const Scalar& color, int thickness = 1, int lineType = 8, int shift = 0)
  {
    if (points.size() > 1)
      {
	for (int i = 1; i < points.size(); i++)
	  line(img, points[i-1], points[i], color, thickness, lineType, shift);
	line(img, points[points.size()-1], points[0], color, thickness, lineType, shift);
      }
  }


  template <class T>
  void paint_points(Mat& img, vector<T> points, int radius, const Scalar& color, int thickness=-1, int lineType=8, int shift=0)
  {
    for (int i = 0; i < points.size(); i++)
      circle(img, points[i], radius, color, thickness, lineType, shift);
  }

}
