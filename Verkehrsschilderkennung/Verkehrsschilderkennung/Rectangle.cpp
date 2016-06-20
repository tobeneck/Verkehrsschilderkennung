#include "Rectangle.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Point2i Rectangle::getTopLeftPoint() { return this->topLeft; }
Point2i Rectangle::getTopRightPoint() { return this->topRight; }
Point2i Rectangle::getBotLeftPoint() { return this->botLeft; }
Point2i Rectangle::getBotRightPoint() { return this->botRight; }
Point2f Rectangle::getCenter() { return this->center; }

Rectangle::Rectangle(Point2i _topLeft, Point2i _topRight, Point2i _botLeft, Point2i _botRight) {
	this->topLeft = _topLeft;
	this->topRight = _topRight;
	this->botLeft = _botLeft;
	this->botRight = _botRight;

	///calculate center
	double dTlTr = sqrt((topLeft.x - topRight.x) * (topLeft.x - topRight.x) + (topLeft.y - topRight.y) * (topLeft.y - topRight.y));
	this->center = Point2f(topLeft.x - dTlTr / 2, topLeft.y - dTlTr / 2);
}