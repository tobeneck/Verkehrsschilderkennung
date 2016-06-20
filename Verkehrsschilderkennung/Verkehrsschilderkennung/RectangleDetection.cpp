#include "RectangleDetection.hpp"
#include "Rectangle.hpp"
#include "Triangle.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace cv;



std::list<Rectangle> RectangleDetection::detectRectangle(Mat image) {
		Mat botLeftI, botRightI, topLeftI, topRightI;
		botLeftI = Mat::zeros(image.rows, image.cols, 0);
		botRightI = Mat::zeros(image.rows, image.cols, 0);
		topLeftI = Mat::zeros(image.rows, image.cols, 0);
		topRightI = Mat::zeros(image.rows, image.cols, 0);

		filter2D(image, botLeftI, ddepth, botLeft, anchor, delta, BORDER_DEFAULT);
		filter2D(image, botRightI, ddepth, botRight, anchor, delta, BORDER_DEFAULT);
		filter2D(image, topLeftI, ddepth, topLeft, anchor, delta, BORDER_DEFAULT);
		filter2D(image, topRightI, ddepth, topRight, anchor, delta, BORDER_DEFAULT);

		//cv::Sobel(botLeftI, botLeftI, ddepth, 1, 1, 3, 1, 0, BORDER_DEFAULT);

		imshow("botLeftI", botLeftI);
		imshow("botRightI", botRightI);
		imshow("topLeftI", topLeftI);
		imshow("topRightI", topRightI);
		waitKey(0);
		//destroyWindow("botLeftI");
		//destroyWindow("botRightI");
		//destroyWindow("topLeftI");
		//destroyWindow("topRightI");

		return possiblePoints(topLeftI, topRightI, botLeftI, botRightI);
	}


std::list<Rectangle> RectangleDetection::possiblePoints(Mat topLeftPoints, Mat topRightPoints, Mat botLeftPoints, Mat botRightPoints) {
	std::list<Rectangle> points = std::list<Rectangle>();
	///optimierung: Bildpunkte Schwarzen die nichtmehr angeschaut werden sollen
	int redThreshhold = 0;

	int alpha = 5; ///möglicher offset nach links/rechts
	int beta = 5; ///möglicher offset nach oben/unten
	int generalDistance = 25;///generelle distanz zwischen den zu findenden Punkten

	Mat test = Mat::zeros(topLeftPoints.rows, topLeftPoints.cols, 0); ///test Matrix zum visualisieren der zwischenergebnisse
	Mat test2 = Mat::zeros(topLeftPoints.rows, topLeftPoints.cols, 0);

	int x = topLeftPoints.cols * 3;/// = i
	int y = topLeftPoints.rows;/// = j
	for (int i = 0; i < x; i = i + 3) {
		for (int j = 0; j < y; j++) {

			float blue = topLeftPoints.at<uchar>(j, i);
			float green = topLeftPoints.at<uchar>(j, i + 1);
			float red = topLeftPoints.at<uchar>(j, i + 2);

			if (red > redThreshhold) {
				Point2i topLeftPoint = Point2i(j , i);
				///a und b iterieren über alpha
				for (int a = -alpha; a < alpha * 3; a = a + 3) {
					for (int b = -beta; b < beta; b++) {

						if (i + a < 0)
							a = 0;//falsch?
						if (j + b < 0)
							b = 0;
						if ((i + a + generalDistance < x) && (j + b + generalDistance < y)) {

							///finden des unteren linken Punktes
							blue = botLeftPoints.at<uchar>(j + generalDistance + b, i + a);
							green = botLeftPoints.at<uchar>(j + generalDistance + b, i + 1 + a);
							red = botLeftPoints.at<uchar>(j + generalDistance + b, i + 2 + a);
							if (red > redThreshhold) {
								Point2i botLeftPoint = Point2i(j + generalDistance + b, i + a);

								///finden des unteren rechten Punktes
								blue = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + a);
								green = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 1 + a);
								red = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 2 + a);
								if (red > redThreshhold) {
									Point2i botRightPoint = Point2i(j + generalDistance + b, i + generalDistance + a);

									///finden des oberen rechten Punktes
									blue = topRightPoints.at<uchar>(j + b, i + generalDistance + a);
									green = topRightPoints.at<uchar>(j + b, i + generalDistance + 1 + a);
									red = topRightPoints.at<uchar>(j + b, i + generalDistance + 2 + a);
									if (red > redThreshhold) {
										Point2i topRightPoint = Point2i(j + b, i + generalDistance + a);

										///schauen ob sich oberer rechter punkt verbindet
										test.at<uchar>(topLeftPoint.x, topLeftPoint.y / 3) = 255;
										test.at<uchar>(botRightPoint.x, botRightPoint.y / 3) = 255;
										test.at<uchar>(botLeftPoint.x, botLeftPoint.y / 3) = 255;
										test.at<uchar>(topRightPoint.x, topRightPoint.y / 3) = 255;

										imshow("gefundenne eckpunkte", test);
										waitKey(0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	imshow("gefundenne eckpunkte", test);
	waitKey(0);
	return points;
}
