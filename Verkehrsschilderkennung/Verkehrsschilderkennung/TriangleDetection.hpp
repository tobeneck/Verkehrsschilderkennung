#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Triangle.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <list>
using namespace cv;

#ifndef TRIANGLEDETECTION_H
#define TRIANGLEDETECTION_H
class TriangleDetection {
protected:
	Mat botLeftTriangle = (Mat_<int>(9, 9)
		<< -6, -11, -11, -6, 0, -6, 7, 7, 4,
		-11, -18, -18, -11, 0, -11, 13, 13, 8,
		-11, -18, -18, -11, 0, 10, 17, 17, 10,
		-6, -11, -11, -6, 0, 12, 19, 19, 12,
		0, 0, 0, 0, 0, 12, 20, 20, 12,
		-6, -11, -11, -6, 0, -6, -11, -11, -6,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-6, -11, -11, -6, 0, -6, -11, -11, -6);

	Mat botRightTriangle = (Mat_<int>(9, 9)
		<< 4, 7, 7, -6, 0, -6, -11, -11, -6,
		8, 13, 13, -11, 0, -11, -18, -18, -11,
		10, 17, 17, 10, 0, -11, -18, -18, -11,
		12, 19, 19, 12, 0, -6, -11, -11, -6,
		12, 20, 20, 12, 0, 0, 0, 0, 0,
		-6, -11, -11, -6, 0, -6, -11, -11, -6,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-6, -11, -11, -6, 0, -6, -11, -11, -6);

	Mat topTriangle = (Mat_<int>(9, 9)
		<< -6, -11, -11, -6, 0, -6, -11, -11, -6,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-11, -18, -18, -11, 0, -11, -18, -18, -11,
		-6, -11, -11, -6, 0, -6, -11, -11, -6,
		-6, -11, -11, 12, 12, 12, -11, -11, -6,
		-6, -11, -11, 19, 20, 19, -11, -11, -6,
		-11, -18, 17, 19, 20, 19, 17, -18, -11,
		-11, -18, 10, 12, 12, 12, 10, -18, -11,
		0, 12, 20, 20, 12, 20, 20, 12, 0);

	Point anchor = Point(-1, -1);
	int ddepth = -1;
	double delta = 0;

	std::list<Triangle> possiblePoints(Mat, Mat, Mat, bool);

public :
	std::list<Triangle> detectTriangle(Mat, bool, bool);
};


#endif