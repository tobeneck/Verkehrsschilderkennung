#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

#ifndef TRIANGLE_H
#define TRIANGLE_H
class Triangle {
public:
	Point2i getTopPoint();
	Point2i getBotLeftPoint();
	Point2i getBotRightPoint();
	Point2f getCenter();
	int getRadius();

	Triangle(Point2i, Point2i, Point2i);
protected:
	Point2i top;
	Point2i botLeft;
	Point2i botRight;
	Point2f center;
	int radius;
};
#endif