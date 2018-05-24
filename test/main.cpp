#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <fstream>

#include <paint_tools.hpp>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;
using namespace paint_tools;

/*

  This program is a test.

 */

queue<Point2f> clicks;
int clicks_counter = 0;

void callback(int event, int x, int y, int flags, void* userdata)
{
  if (event == EVENT_LBUTTONDOWN && clicks_counter < 6)
    {
      clicks.push(Point2f(x,y));
      clicks_counter++;
    }
  return;
}


int main(int argc, char** argv )
{
  if ( argc != 2 )
    {
      cerr << "Please provide an image.\n";
      return -1; 
   }

  string filename;
  filename = argv[1];

  Mat image = imread(filename, 1);
  if ( !image.data )
    {
      cerr << "No image data \n";
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      return -1;
    }

  namedWindow("Image");
  imshow("Image", image);

  setMouseCallback("Image", callback, NULL);
  vector<Point2f> ref_points;
  int wK = -1;
  for (int i = 0; i < 4 && wK != 27; wK = waitKey(50))
    {
      if (clicks.size() > 0)
  	{
  	  //grab point
  	  ref_points.push_back(Point2f(clicks.front())*0.5);
  	  clicks.pop();
  	  //mark
  	  circle(image, ref_points[i], 5, red, -1);
  	  //show
  	  imshow("Image", image);
  	  i++;
  	}
    }
  if (wK == 27)
    exit(0);
  paint_polygon(image, ref_points, red);

  imshow("Image", image);
  cout << ref_points << endl;

  waitKey(0);

  return 0;
}
