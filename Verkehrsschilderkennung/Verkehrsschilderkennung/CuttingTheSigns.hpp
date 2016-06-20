#include <list>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Triangle.hpp"
#include "Circle.hpp"
using namespace cv;

class CuttingTheSigns {
public:
	std::list<Mat> cutTheSigns(Mat, std::list<Triangle>);
	std::list<Mat> cutTheCircles(Mat, std::list<Circle>);
	std::list<Mat> cutTheTriangles(Mat, std::list<Triangle>);

};