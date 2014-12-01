#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include <paint_tools.hpp>

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

/*

  This program performs transforms part of an image, chosen by the user, into another one, using perspective transform.

  The user selects four points. These four points are then set to be at the edges of the image with perspective transform.

 */

void print_errmsg(char* arg)
{
  cerr << "usage: " << arg << " [-f <Image_path>]\n";
  cerr << "This program applies a perspective transform on the image provided using four points chosen by the user. The image is stretched such that the four points coincide with the edges of the original image. It outputs the name of the created file. If the -f argument isn't specified, it takes a filename from standard input.\n";
}

//globals???????
queue<Point2f> clicks;
int clicks_counter = 0;

void callback(int event, int x, int y, int flags, void* userdata)
{
  if (event == EVENT_LBUTTONDOWN && clicks_counter < 6)
    {
      clicks.push(Point2f(x,y));
      clicks_counter++;
    }
  return;
}


int main(int argc, char** argv )
{

  string filename;
  if (argc > 1)
    if (!strcmp(argv[1], "-f"))
      if (argc > 2)
	filename = argv[2];
      else
	{
	  cerr << "No filename introduced\n";
	  print_errmsg(argv[0]);
	  return -1;
	}
    else
      {
	cerr << "Unrecognized arguments\n";
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
      return -1;
    }


  /**                 **/
  /** Point selection **/
  /**                 **/
  //Show image to the user
  Mat point_selection_img = image.clone();
  namedWindow("Point selection");
  imshow("Point selection", point_selection_img);
  setMouseCallback("Point selection", callback, NULL);

  //Select 4 points, showing them
  /***********************************/
  /* Grab the four reference points */
  ///Mark each one in red
  vector<Point2f> ref_points;
  int wK = -1;
  for (int i = 0; i < 4 && wK != 27; wK = waitKey(50))
    {
      if (clicks.size() > 0)
	{
	  //grab point
	  ref_points.push_back(Point2f(clicks.front()));
	  clicks.pop();
	  //mark
	  circle(point_selection_img, ref_points[i], 5, paint_tools::red, -1);
	  //show
	  imshow("Point selection", point_selection_img);
	  i++;
	}
    }
  if (wK == 27)
    {
      cerr << "Aborting...\n";
      return -1;
    }
  waitKey(300);
  destroyAllWindows();


  //Perform transformationx
//Resulting image should be the same size as original image
//So the four target points will be the four edges.
  vector<Point2f> dst_points;
  dst_points.push_back(Point2f(0, 0));
  dst_points.push_back(Point2f(0, image.size().width));
  dst_points.push_back(Point2f(image.size().height, image.size().width));
  dst_points.push_back(Point2f(image.size().height, 0));

  Mat trans = getPerspectiveTransform(ref_points, dst_points);

  warpPerspective(image, image, trans, image.size());

  //Change filename extension
  std::string writename = filename;
  writename.erase(writename.find_last_of('.'));
  std::ostringstream extension;
  //add operation tag
  extension << "_de-persp" << ".png";
  writename += extension.str();
  imwrite(writename, image);
  cout << writename << endl;

  return 0;
}
