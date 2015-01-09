#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program calculates the average of several images with the same size.

 */

void print_errmsg(const char* argv)
{
  cerr << "Use: " << argv << "<image1> <image2> [more images ...]" endl
       << "This program takes in several images and outputs a single image that is the average of the inputs. Using images of different sizes causes undefined behaviour. It outputs the name of the created file, which is the name of the first file + \"_avg\"\n";
}

int main(int argc, char** argv )
{
  if ( argc < 3 )
    {
      print_errmsg(argv[0]);
      return -1; 
   }

  Mat acc, avg;
  for (int i = 0; i < argc - 1; i++)
    {
      Mat image = imread(argv[i], 1);
      if ( !image.data )
	{
	  cerr << "No image data \n";
	  cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
	  return -1;
	}
      if (acc.empty())
	acc = Mat(image.size(), CV_32UC3, Scalar(0, 0, 0));
      accumulate(image, acc);
    }

  acc.convertTo(avg, CV_32UC3, 1.0/(argc - 1));


  std::string writename = argv[1];
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;

  extension << "_avg.png";
  writename += extension.str();
  imwrite(writename, gauss_diff);
  printf("%s\n", writename.c_str());

  return 0;
}
