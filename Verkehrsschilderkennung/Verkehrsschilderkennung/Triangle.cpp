#include "Triangle.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Point2i Triangle::getTopPoint() { return this->top; }
Point2i Triangle::getBotLeftPoint() { return this->botLeft; }
Point2i Triangle::getBotRightPoint() { return this->botRight; }
Point2f Triangle::getCenter() { return this->center; }
int Triangle::getRadius(){ return this->radius; }

Triangle::Triangle(Point2i _top, Point2i _botLeft, Point2i _botRight) {
	this->top = _top;
	this->botLeft = _botLeft;
	this->botRight = _botRight;

	///calculate center
	double dBlBr = sqrt((botLeft.x - botRight.x) * (botLeft.x - botRight.x) + (botLeft.y - botRight.y) * (botLeft.y - botRight.y));
	double dTBr = sqrt((top.x - botRight.x) * (top.x - botRight.x) + (top.y - botRight.y) * (top.y - botRight.y));
	double dTBl = sqrt((botLeft.x - top.x) * (botLeft.x - top.x) + (botLeft.y - top.y) * (botLeft.y - top.y));
	this->radius = max({ dBlBr , dTBr , dTBl });;
	this->center = Point2f(botLeft.x - dBlBr / 2, botLeft.y + dBlBr / 2);



}