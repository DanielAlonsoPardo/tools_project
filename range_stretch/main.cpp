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

int main(int argc, char** argv )
{
  if ( argc < 2 || argc > 4 || argc == 3 )
    {
      cerr << "usage: " << argv[0] << " (gauss radius) [-f <Image_path>]\n";
      cerr << "This program stretches the color range of an image to go from 0 to 255 (after grayscale) and saves the result as a png. It outputs the name of the created file. If the -f argument isn't provided, it takes a filename from standard input.\n";
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

  int r1 = atoi(argv[1]);

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