#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

#ifndef RECTANGLE_H
#define RECTANGLE_H
class Rectangle {

public:
	Point2i getTopLeftPoint();
	Point2i getTopRightPoint();
	Point2i getBotLeftPoint();
	Point2i getBotRightPoint();
	Point2f getCenter();

	Rectangle(Point2i, Point2i, Point2i, Point2i);
protected:
	Point2i topLeft;
	Point2i topRight;
	Point2i botLeft;
	Point2i botRight;
	Point2f center;

};
#endif