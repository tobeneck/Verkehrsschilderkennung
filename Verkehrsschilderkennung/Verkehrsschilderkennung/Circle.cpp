#include "Circle.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Point2i Circle::getTopLeftPoint() { return this->topLeft; }
Point2i Circle::getTopRightPoint() { return this->topRight; }
Point2i Circle::getBotLeftPoint() { return this->botLeft; }
Point2i Circle::getBotRightPoint() { return this->botRight; }
Point2f Circle::getCenter() { return this->center; }
int Circle::getRadius() { return this->radius; }

Circle::Circle(Point2i _topLeft, Point2i _topRight, Point2i _botLeft, Point2i _botRight) {
	this->topLeft = _topLeft;
	this->topRight = _topRight;
	this->botLeft = _botLeft;
	this->botRight = _botRight;

	///calculate center
	double dTlBr = sqrt((topLeft.x - botRight.x) * (topLeft.x - botRight.x) + (topLeft.y - botRight.y) * (topLeft.y - botRight.y));
	this->radius = dTlBr / 2;
	this->center = Point2f(topLeft.x - radius, topLeft.y - radius);
}