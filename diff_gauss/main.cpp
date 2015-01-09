#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program performs a gaussian difference operation on an image with the given radii for each gaussian blur. It allows for different ways of dealing with the resulting negative values.

  The image is converted to grayscale, then to 16bit signed integer (up from 8bit unsigned), both gaussians calculated and subtracted,
 the range of the result adjusted to be between 0 and 255 if so specified, and then converted back and written.

 */

void print_errmsg(const char* argv)
{
  cerr << "usage: " << argv << " <first gauss radius> <second gauss radius> [-l | -r | -a | ar] [-f <filename>]\n"
       << "This program performs a difference of gaussians operation on the provided image (after grayscale) and saves the result as a png. If the -f argument isn't provided, it takes a filename from standard input. It outputs the name of the created file.\n"
       << "Options:\n"
       << "\t-l\t\"Lossless\": Does not truncate negative values to 0. This stretches the colors range to [0-255].\n"
       << "\t-r\t\"Range\": This sets the colors range to [0-255].\n"
       << "\t-a\t\"Absolute\": Gaussian difference calculated as absolute difference.\n"
       << "\t-ar\t\"Absolute+Range\" A combination of the previous two.\n";
}

int main(int argc, char** argv)
{
  if ( argc < 3)
    {
      print_errmsg(argv[0]);
      return -1; 
   }

  enum Option {LOSSLESS, RANGE_ADJUSTED, ABSOLUTE, ABSOLUTE_RANGE_ADJUSTED, NO_OPTION = -1};

  string filename;
  string str_option = "";
  Option option = NO_OPTION;

  char* check = 0;

  int r1 = strtol(argv[1], &check, 10);
  if (check == argv[1])
    {
      print_errmsg(argv[0]);
      return -1;
    }

  int r2 = strtol(argv[2], &check, 10);
  if (check == argv[2])
    {
      print_errmsg(argv[0]);
      return -1;
    }

  //only odd radius is allowed
  if (r1%2 == 0) r1++;
  if (r2%2 == 0) r2++;


  for (int i = 3; i < argc; i++)
    {

      if (!strcmp(argv[i], "-l"))
	{option = LOSSLESS; str_option = "-l";}
      else if (!strcmp(argv[i], "-r"))
	{option = RANGE_ADJUSTED; str_option = "-r";}
      else if (!strcmp(argv[i], "-a"))
	{option = ABSOLUTE; str_option = "-a";}
      else if (!strcmp(argv[i], "-ar"))
	{option = ABSOLUTE_RANGE_ADJUSTED; str_option = "-ar";}
      else if (!strcmp(argv[i], "-f"))
	filename = argv[++i];
      else
	{
	  print_errmsg(argv[0]);
	  return -1;
	}
    }

  if (filename.empty())
    cin >> filename;

  Mat image = imread( filename, 1 );
  if ( !image.data )
    {
      cerr << "No image data \n";
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      return -1;
    }


  //Convert to grayscale
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

  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;

  extension << "_" << r1 << "-" << r2 << "gd" << str_option <<".png";
  writename += extension.str();
  imwrite(writename, gauss_diff);
  printf("%s\n", writename.c_str());

  return 0;
}
