#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
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

/*

  This program performs a gaussian difference operation on an image with the given radii for each gaussian blur. It allows for different ways of dealing with the resulting negative values.

  The image is converted to grayscale, then to 16bit signed integer (up from 8bit unsigned), both gaussians calculated and subtracted,
 the range of the result adjusted to be between 0 and 255 if so specified, and then converted back and written.

 */

int main(int argc, char** argv )
{
  if ( argc < 4 || argc > 5 )
    {
      printf("usage: %s <Image_Path> (first gauss radius) (second gauss radius) [optional option]\n", argv[0]);
      printf("This function performs a difference of gaussians operation on the provided image (after grayscale) and saves the inverted result as a png. It outputs the name of the created file.\n");
      cout << "Options:\n"
	   << "\t-l\t\"Lossless\": Does not truncate negative values to 0. This sets the colors range to [0-255].\n"
	   << "\t-r\t\"Range\": This sets the colors range to [0-255].\n"
	   << "\t-a\t\"Absolute\": Gaussian difference calculated as absolute difference.\n"
	   << "\t-ar\t\"Absolute+Range\" A combination of the previous two.\n";
      return -1; 
   }

  Mat image = imread( argv[1], 1 );
  if ( !image.data )
    {
      printf("No image data \n");
      printf("Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n");
      return -1;
    }

  //  cout << type2str(image.type()) << endl;

  enum Option {LOSSLESS, RANGE_ADJUSTED, ABSOLUTE, ABSOLUTE_RANGE_ADJUSTED, NO_OPTION = -1};

  Option option = NO_OPTION;

  if (argc == 5)
    if (!strcmp(argv[4], "-l"))
      option = LOSSLESS;
    else if (!strcmp(argv[4], "-r"))
      option = RANGE_ADJUSTED;
    else if (!strcmp(argv[4], "-a"))
      option = ABSOLUTE;
    else if (!strcmp(argv[4], "-ar"))
      option = ABSOLUTE_RANGE_ADJUSTED;
    else
      cout << "Ignoring unrecognized option: \"" << argv[4] << "\"\n";

  //only odd radius is allowed
  int r1 = atoi(argv[2]);
  if (r1%2 == 0) r1++;
  int r2 = atoi(argv[3]);
  if (r2%2 == 0) r2++;


  //Convert to grayscale
  image.convertTo( image, CV_16UC3 );
  cvtColor( image, image, CV_BGR2GRAY );
  image.convertTo( image, CV_16SC1 );



  //Gauss blurs
  Mat g1, g2;
  GaussianBlur(image, g1, Size(r1,r1), 0);
  GaussianBlur(image, g2, Size(r2,r2), 0);

  //Gauss difference
  Mat gauss_diff;
  if (option == ABSOLUTE || option == ABSOLUTE_RANGE_ADJUSTED)
    absdiff(g1, g2, gauss_diff);
  else
    gauss_diff = g1 - g2;

  if (option == LOSSLESS ||
      option == ABSOLUTE_RANGE_ADJUSTED ||
      option == RANGE_ADJUSTED)
    {
      double min, max;
      minMaxLoc(gauss_diff, &min, &max);
      if (option == LOSSLESS)
	{
	  gauss_diff -= min;
	  max -= min;
	}
      gauss_diff *= (255.0 / max);
    }

  gauss_diff.convertTo(gauss_diff, CV_8UC1);
  //bitwise_not(gauss_diff, gauss_diff);

  std::string writename = argv[1];
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  string opts = "";
  if (argc == 5)
    opts = argv[4];
  extension << "_" << r1 << "-" << r2 << "gd" << ((option == NO_OPTION) ? "" : opts) <<".png";
  writename += extension.str();
  imwrite(writename, gauss_diff);
  printf("%s\n", writename.c_str());



  return 0;
}
