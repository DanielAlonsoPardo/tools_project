#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <fstream>


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
  cerr << "usage: " << arg << " [-f <Image_path>] [-i <file with points to use>] [-o <file to write points used>] [-t <file with points to mark>]\n";
  cerr << "This program applies a perspective transform on the image provided using four points chosen by the user either manually or specifying a file through -i. The image is stretched such that the four points coincide with the edges of the original image. It outputs the name of the created file.\n"
       << "\t-f\tImage to transform.\n"
       << "\t-o\tFile to write used points to.\n"
       << "\t-i\tFile to read points from.\n"
       << "\t-t\tFile to read points from. These will be painted on the image while manually choosing points, has no other effect.\n";
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
  string read_file;
  string write_file;
  string helper_file;

  for (int i = 1; i < argc; i++)
    {
      if (!strcmp(argv[i], "-i"))
	read_file = argv[++i];
      else if (!strcmp(argv[i], "-o"))
	write_file = argv[++i];
      else if (!strcmp(argv[i], "-f"))
	filename = argv[++i];
      else if (!strcmp(argv[i], "-t"))
	helper_file = argv[++i];
      else
	{
	  print_errmsg(argv[0]);
	  return -1;
	}
    }

  if (filename.empty())
    cin >> filename;

  Mat image = imread(filename, 1);
  if ( !image.data )
    {
      cerr << "No image data: " << filename << endl;
      cerr << "Admitted formats reminder: *.bmp, *.dib, *.jpeg, *.jpg, *.jpe, *.jp2, *.png, *.pbm, *.pgm, *.ppm, *.sr, *.ras, *.tiff, *.tif \n";
      print_errmsg(argv[0]);
      return -1;
    }


  vector<Point2f> ref_points;
  if (read_file.empty())
    {//only if the points have to be chosen by hand
      /**                         **/
      /** Graphic point selection **/
      /**                         **/
      //Show image to the user

      //Image must be shown at x4 size
      const double ZOOM = 4;
      Mat point_selection_img;
      resize(image, point_selection_img, Size(), ZOOM, ZOOM, cv::INTER_NEAREST);
      namedWindow("Point selection");
      imshow("Point selection", point_selection_img);
      setMouseCallback("Point selection", callback, NULL);


      /**/
      /*Read points from -t file*/
      /**/
      if (!helper_file.empty())
	{
	  vector<Point2f> t_points;
	  ifstream t_file;
	  t_file.open(helper_file);
	  if (!t_file)
	    {
	      cerr << "File could not be opened: " << helper_file << endl;
	      print_errmsg(argv[0]);
	      return -1;
	    }

	  float x, y;
	  while (t_file >> x >> y)
	    t_points.push_back(Point2f(x, y) * ZOOM);

	  if (t_points.size() != 4)
	    {
	      cerr << "File must contain exactly 4 points with the following syntax: \n"
		   << "<x_pos> <y_pos>\\n\n";
	      print_errmsg(argv[0]);
	      return -1;
	    }
	  t_file.close();

	  /**/
	  /*Show the points*/
	  /**/
	  paint_tools::paint_points(point_selection_img, t_points, 1, paint_tools::red);
	  paint_tools::paint_points(point_selection_img, t_points, 6, paint_tools::red, 1);
	  imshow("Point selection", point_selection_img);
	}


      //Select 4 points, showing them
      /***********************************/
      /* Grab the four reference points */
      ///Mark each one in red
      int wK = -1;
      for (int i = 0; i < 4 && wK != 27; wK = waitKey(50))
	{
	  if (clicks.size() > 0)
	    {
	      //grab point
	      ref_points.push_back(Point2f(clicks.front())*(1/ZOOM));
	      clicks.pop();
	      //mark
	      circle(point_selection_img, ref_points[i]*ZOOM, 5, paint_tools::red, -1);
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
    }
  else
    /**/
    /* (-i): Read points from file instead of selecting by hand*/
    /**/
    {
      ifstream in_file;
      in_file.open(read_file);
      if (!in_file)
	{
	  cerr << "File could not be opened: " << read_file << endl;
	  print_errmsg(argv[0]);
	  return -1;
	}
      float x, y;
      while (in_file >> x >> y)
	ref_points.push_back(Point2f(x, y));

      if (ref_points.size() != 4)
	{
	  cerr << "File must contain exactly 4 points with the following syntax: \n"
	       << "<x_pos> <y_pos>\\n\n";
	  print_errmsg(argv[0]);
	  return -1;
	}
      in_file.close();

    }


  //Perform transformation
//Resulting image should be the same size as original image
//So the four target points will be the four edges.
  vector<Point2f> dst_points;
  dst_points.push_back(Point2f(0, 0));
  dst_points.push_back(Point2f(image.size().width, 0));
  dst_points.push_back(Point2f(image.size().width, image.size().height));
  dst_points.push_back(Point2f(0, image.size().height));

  Mat trans = getPerspectiveTransform(ref_points, dst_points);

  warpPerspective(image, image, trans, image.size());

  if (!write_file.empty())
    {
      ofstream out_file;
      out_file.open(write_file);
      if (!out_file)
	{
	  cerr << "Problem writing to file: " << write_file << endl
	       << "Points to be written: \n"
	       << ref_points << endl;
	  print_errmsg(argv[0]);
	}
      for (int i = 0; i < dst_points.size(); i++)
	out_file << ref_points[i].x << " " << ref_points[i].y << endl;
      out_file.close();
    }



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
