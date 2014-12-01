#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program performs a gaussian blur on an image with the given radius.

  The image is converted to grayscale.

 */

int main(int argc, char** argv )
{
  if ( argc < 2 || argc > 4 || argc == 3 )
    {
      cerr << "usage: " << argv[0] << " (gauss radius) [-f <Image_path>]\n";
      cerr << "This function performs a gaussian blur operation on the provided image (after grayscale) and saves the result as a png. It outputs the name of the created file. If the -f argument isn't provided, it takes a filename from standard input.\n";
      return -1; 
   }

  char* check = 0;
  int r1 = strtol(argv[1], &check, 10);
  if (check == argv[1])
    {
      print_errmsg(argv[0]);
      return -1;
    }


  string filename;
  if (argc == 4)
    if (!strcmp(argv[2], "-f"))
      filename = argv[3];
    else
      {
	cerr << "No filename provided/n";
	return -1;
      }
  else
    cin >> filename;

  Mat image = imread(filename, 1);
  if ( !image.data )
    {
      cerr << "No image data \n";
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      return -1;
    }

  //Convert to grayscale
  cvtColor( image, image, CV_BGR2GRAY );
  if (r1%2 == 0) r1++;

  //Gauss blurs
  Mat g1;
  GaussianBlur(image, g1, Size(r1,r1), 0);

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_" << r1 << "g" << ".png";
  writename += extension.str();
  imwrite(writename, g1);
  cout << writename << endl;

  return 0;
}
