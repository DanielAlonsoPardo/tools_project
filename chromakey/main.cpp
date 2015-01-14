#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

#include <OpenCV_tools.hpp>

using namespace cv;
using namespace std;

/*

  This program creates a mask that covers all hues between the given values.
  It also performs some optional tasks:
  -Invert the mask
  -Perform gauss operation beforehand
  -Return the original image cropped using the created mask
  -Perform threshold on channels other than just hue

 */

void print_errmsg(const char* argv)
{
  cerr << "Usage: " << argv << " <lower hue bound> <upper hue bound> [OPTIONS]\n"
       << "This program creates a mask that covers all hues between the given values. If the -f argument isn't provided, it takes a filename from standard input. The result is saved as a png and the filename is printed to standard output.\n"
       << "The name of the output file, if not specified, is equals to the original name minus the file extension (which is changed to .png) plus a trailingunderscore, the options used and \"ChrK\"\n"
       << "Options:\n"
       << "\t-f <file>\tSpecify file to use\n"
       << "\t-g <radius>\tPerform a gaussian blur with the given radius before calculating the mask\n"
       << "\t-i\tInvert the mask\n"
       << "\t-c\tSave the original image cropped with the generated mask, instead of the mask itself.\n"
       << "\t-s\tSpecify saturation lower bound\n"
       << "\t-v\tSpecify value lower bound\n";

}

int main(int argc, char** argv )
{
  string filename;
  int lowerbound, upperbound;//hue bounds
  int s_lowerbound, v_lowerbound;//saturation and value bounds
  int gauss_radius;
  //Option flags
  bool F = false, G = false, I = false, C = false, S = false, V = false;

  if (argc < 3)
    {
      print_errmsg(argv[0]);
      return -1;
    }

  ////////////////////////////////////////////////////////////
  //options
  /////////
  //
  //don't let getopt post error messages
  opterr = 0;
  //start from the third argument (first two must be the hue bounds)
  optind = 3;
  ////////////////////////////////////////////////////////////
  {//Parse options
    char opt;
    char* check;
    while ((opt = getopt(argc, argv, "f:g:ics:v:")) != -1)
      switch (opt)
	{
	case 'f':
	  F = true;
	  filename = optarg;
	  break;
	case 'g':
	  G = true;
	  gauss_radius = strtol(optarg, &check, 10);
	  if (check == optarg)
	    {
	      cerr << "Option -g requires a numerical argument.\n\n";
	      print_errmsg(argv[0]);
	      return -1;
	    }
	  if (gauss_radius%2 == 0) gauss_radius++;
	  break;
	case 'i':
	  I = true;
	  break;
	case 'c':
	  C = true;
	  break;
	case 's':
	  S = true;
	  s_lowerbound = strtol(optarg, &check, 10);
	  if (check == optarg)
	    {
	      cerr << "Option -s requires a numerical argument.\n\n";
	      print_errmsg(argv[0]);
	      return -1;
	    }
	  break;
	case 'v':
	  V = true;
	  v_lowerbound = strtol(optarg, &check, 10);
	  if (check == optarg)
	    {
	      cerr << "Option -v requires a numerical argument.\n\n";
	      print_errmsg(argv[0]);
	      return -1;
	    }
	  break;
	case '?':
	  if (optopt == 'f' || optopt == 'g' || optopt == 's' || optopt == 'v')
	    {
	      cerr << "Option " << (char)optopt << " requires an argument.\n\n";
	      print_errmsg(argv[0]);
	      return -1;
	    }
	  else if (isprint (optopt))
	    {
	      cerr << "Ignoring unknown option " << (char)optopt << ".\n";
	    }
	  //	  else
	    //	    cerr << "Ignoring an unknown option.\n";
	  break;
	default:
	  abort ();
	}
    }
  // end of options
  /////////////////////////////////////////////////
  {//
    char *check, *check2;
    lowerbound = strtol(argv[1], &check, 10);
    upperbound = strtol(argv[2], &check2, 10);
    if (check == argv[1] || check2 == argv[2])
      {
	cerr << "Hue bounds must be numerical.\n\n";
	print_errmsg(argv[0]);
	return -1;
      }
  }
  //end of arguments parsing
  /////////////////////////////////////////////////


  Mat image = imread(filename), copy, mask, smask, vmask;
  OpenCV_tools::image_check(image, filename);

  //G, I, C

  if (G)
    GaussianBlur(image, copy, Size(gauss_radius, gauss_radius), 0);
  else
    copy = image.clone();

  //Extract the Hue channel and get the mask
  cvtColor(copy, copy, CV_BGR2HSV);
  vector<Mat> channels;
  split(copy, channels);
  ///upper bound is allowed to be lower than lower bound, its the same but inverted
  if (upperbound < lowerbound)
    {
      int xchng = upperbound;
      upperbound = lowerbound;
      lowerbound = xchng;
      I = !I;
    }

  inRange(channels[0], lowerbound, upperbound, mask);

  if (I)
    bitwise_not(mask, mask);

  if (S)
    {
      inRange(channels[1], s_lowerbound, 255, smask);
      bitwise_and(mask, smask, mask);
    }

  if (V)
    {
      inRange(channels[2], v_lowerbound, 255, vmask);
      bitwise_and(mask, vmask, mask);
    }

  Mat cropped;
  if (C)
    image.copyTo(cropped, mask);

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_";
  if (G)
    extension << gauss_radius << "g-";
  if (I)
    extension << "i-";
  if (C)
    extension << "c-";
  if (S)
    extension << "s" << s_lowerbound << "-";
  if (V)
    extension << "v" << v_lowerbound << "-";
  extension << lowerbound << "-" << upperbound << "ChrK.png";
  writename += extension.str();
  imwrite(writename, C ? cropped : mask);
  cout << writename << endl;

  return 0;
}
