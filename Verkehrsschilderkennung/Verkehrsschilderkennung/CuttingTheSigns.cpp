#include <list>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Triangle.hpp"
#include "Circle.hpp"
#include "CuttingTheSigns.hpp"

using namespace cv;

std::list<Mat> CuttingTheSigns::cutTheSigns(Mat image, std::list<Triangle> Triangles) {
	std::list<Mat> r;
	return r;
}

std::list<Mat> CuttingTheSigns::cutTheCircles(Mat image, std::list<Circle> Circles) {
	std::list<Mat> r;
	int radiusOffset = 5;///pixels the radius is extendet
	for each (Circle c in Circles)
	{
		cv::Mat signImage;
		image.copyTo(signImage); 
		int x = c.getTopLeftPoint().x - radiusOffset;
		int y = c.getBotLeftPoint().y - radiusOffset;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		int width = c.getRadius() + radiusOffset * 2;
		int	height = c.getRadius() + radiusOffset * 2;

		if (y / 3 + width > image.cols)
			width = image.cols - y / 3 - 1;
		if (x  + height > image.rows)
			height = image.rows - x - 1;

		signImage = Mat(image, Rect(y/3, x, width, height));
		resize(signImage, signImage, Size(30, 30), 0, 0, INTER_CUBIC);
		r.push_back(signImage);
	}
	return r;
}
std::list<Mat> CuttingTheSigns::cutTheTriangles(Mat image, std::list<Triangle> Triangles) {
	std::list<Mat> r;
	int radiusOffset = 5;///pixels the radius is extendet
	for each (Triangle t in Triangles)
	{
		cv::Mat signImage;
		image.copyTo(signImage);
		int x = t.getTopPoint().x - t.getRadius()/2- radiusOffset;
		int y = t.getBotLeftPoint().y - radiusOffset;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		int width = t.getRadius() + radiusOffset * 2;
		int	height = t.getRadius() + radiusOffset * 2;

		if (y / 3 + width > image.cols)
			width = image.cols - y / 3 - 1;
		if (x + height > image.rows)
			height = image.rows - x - 1;

		signImage = Mat(image, Rect(y / 3, x, width, height));
		resize(signImage, signImage, Size(30, 30), 0, 0, INTER_CUBIC);
		r.push_back(signImage);
	}
	return r;
}