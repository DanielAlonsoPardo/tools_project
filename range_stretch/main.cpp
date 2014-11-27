#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program stretches the color range of an image to go from 0 to 255, up from whatever it was prior.

  The image is converted to grayscale in the process.

 */
void print_errmsg(const char* argv)
{
      cerr << "usage: " << argv << " [-f <Image_path>]\n";
      cerr << "This program stretches the color range of an image to go from 0 to 255 (after grayscale) and saves the result as a png. It outputs the name of the created file. If the -f argument isn't provided, it takes a filename from standard input.\n";
}

int main(int argc, char** argv )
{
  if (argc > 3)
    {
      print_errmsg(argv[0]);
      return -1; 
    }
  string filename;
  if (argc == 3)
    if (!strcmp(argv[1], "-f"))
      filename = argv[2];
    else
      {
	print_errmsg(argv[0]);
	return -1;
      }
  else if (argc == 1)
    cin >> filename;
  else
      {
	print_errmsg(argv[0]);
	return -1;
      }

  Mat image = imread(filename, 1);
  if ( !image.data )
    {
      cerr << "No image data \n";
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      return -1;
    }

  //Convert to grayscale
  cvtColor(image, image, CV_BGR2GRAY);

  //stretch
  double min, max;
  minMaxLoc(image, &min, &max);
  image -= min;
  max -= min;
  image *= (255.0 / max);

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_" << "r" << ".png";
  writename += extension.str();
  imwrite(writename, g1);
  cout << writename << endl;

  return 0;
}
