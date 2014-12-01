#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program performs a threshold operation on an image with the given options.

  The image is converted to grayscale.

 */

void print_errmsg(const char* argv) {
      cerr << "usage: " << argv << " (threshold value) [-t <threshold type>] [-f <Image_path>]\n";
      cerr << "This function performs a threshold operation on the provided image (after grayscale) and saves the inverted result as a png. It outputs the name of the created file. If the -f argument isn't provided, it takes a filename from standard input.\n";
      cerr << "Valid threshold types are:\n"
	   << "THRESH_BINARY\n"
	   << "THRESH_BINARY_INV\n"
	   << "THRESH_TRUNC\n"
	   << "THRESH_TOZERO\n"
	   << "THRESH_TOZERO_INV\n";
};

int main(int argc, char** argv )
{
  if (argc < 2)
    {
      print_errmsg(argv[0]);
      return -1; 
    }

  string filename;
  int thresh_type = THRESH_BINARY;
  int fill_value = 255;
  double thresh;

  bool filename_chosen = false;
  bool err = false;

  char* check = 0;
  thresh = strtol(argv[1], &check, 10);
  if (check == argv[1])
    {
      print_errmsg(argv[0]);
      return -1;
    }


  string th_t = "b";
  for (int i = 2; i < argc; i += 2)
    if (!strcmp(argv[i], "-f"))
      {
	filename = argv[i + 1];
	filename_chosen = true;
      }
    else if (!strcmp(argv[i], "-c"))
      fill_value = stoi(argv[i+1]);
    else if (!strcmp(argv[i], "-t"))
      {
        if (!strcmp(argv[i+1], "THRESH_BINARY")) {
	  thresh_type = THRESH_BINARY; th_t = "b";}
        else if (!strcmp(argv[i+1], "THRESH_BINARY_INV")) {
	  thresh_type = THRESH_BINARY_INV; th_t = "bi";}
        else if (!strcmp(argv[i+1], "THRESH_TRUNC")) {
	  thresh_type = THRESH_TRUNC; th_t = "t";}
        else if (!strcmp(argv[i+1], "THRESH_TOZERO")) {
	  thresh_type = THRESH_TOZERO; th_t = "z";}
        else if (!strcmp(argv[i+1], "THRESH_TOZERO_INV")) {
	  thresh_type = THRESH_TOZERO_INV; th_t = "zi";}
	else
	  err = true;
      }
    else
      err = true;
  

  if (err)
    {
      cerr << "Invalid parameters.";
      print_errmsg(argv[0]);
      return -1;
    }

  if (!filename_chosen)
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

  //Threshold
  threshold(image, image, thresh, fill_value, thresh_type);

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_" << thresh << "th-" << th_t << ".png";
  writename += extension.str();
  imwrite(writename, image);
  cout << writename << endl;

  return 0;
}
