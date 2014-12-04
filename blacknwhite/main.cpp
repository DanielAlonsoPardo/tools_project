#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program converts an image to grayscale.

 */


void print_errmsg(char* argv)
{
  cerr << "usage: " << argv << " [-f <Image_path>]\n";
  cerr << "This function converts an image to grayscale and saves the result as a png. It outputs the name of the created file. If the -f argument isn't provided, it takes a filename from standard input.\n";
}

int main(int argc, char** argv )
{
  string filename;
  if (argc == 3)
    if (!strcmp(argv[2], "-f"))
      filename = argv[3];
    else
      {
	cerr << "No filename provided/n";
	print_errmsg(argv[0]);
	return -1;
      }
  else
    cin >> filename;

  Mat image = imread(filename, 1);
  if ( !image.data )
    {
      cerr << "No image data \n";
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      print_errmsg(argv[0]);
      return -1;
    }

  //Convert to grayscale
  cvtColor( image, image, CV_BGR2GRAY );

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_" << "bw" << ".png";
  writename += extension.str();
  imwrite(writename, g1);
  cout << writename << endl;

  return 0;
}
