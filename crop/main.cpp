#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <fstream>

#include <paint_tools.hpp>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace xfeatures2d;
using namespace std;
using namespace paint_tools;

/*

  This program is a test.

 */

queue<Point2f> clicks;
int clicks_counter = 0;

void image_check(Mat img) {
}

int main(int argc, char** argv )
{
  const int MIN_ARGS = 4 + 1;
  if ( argc < 1 + MIN_ARGS )
    {
      cerr << "Use: " << argv[0] << " <corner 1 x> <corner 1 y> <corner 2 x> <corner 2 y> <image> [more images ...].\n";
      return -1; 
    }

  ////////////////////////////////////////////////////////////////

  Rect roi(strtol(argv[1], NULL, 10), strtol(argv[2], NULL, 10), strtol(argv[3], NULL, 10), strtol(argv[4], NULL, 10));



  std::ostringstream extension;
  //operation tag
  extension << "_" << "crop" << ".png";
  for (int i = MIN_ARGS; i < argc; i++)
    {
      Mat image = imread(argv[i], 1);
      image_check(image);

      if (!image.data)
	{
	  cerr << "Error when reading file: " << argv[i] << endl;
	  continue;
	}


      Mat cropped = image(roi);


      //Save file to disk
      std::string writename = argv[i];
      writename.erase(writename.find_last_of('.'));
      writename += extension.str();
      imwrite(writename, cropped);
      cout << writename << endl;
    }


  ////////////////////////////////////////////////////////////////

  return 0;
}
