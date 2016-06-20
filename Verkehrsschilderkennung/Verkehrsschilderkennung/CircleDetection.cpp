#include "CircleDetection.hpp"
#include "Circle.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
using namespace cv;

std::list<Circle> CircleDetection::detectCircle(Mat image, bool showFilter, bool showFoundPoints) {
	Mat botLeftI, botRightI, topLeftI, topRightI;
	botLeftI = Mat::zeros(image.rows, image.cols, 0);
	botRightI = Mat::zeros(image.rows, image.cols, 0);
	topLeftI = Mat::zeros(image.rows, image.cols, 0);
	topRightI = Mat::zeros(image.rows, image.cols, 0);

	filter2D(image, botLeftI, ddepth, botLeft, anchor, delta, BORDER_DEFAULT);
	filter2D(image, botRightI, ddepth, botRight, anchor, delta, BORDER_DEFAULT);
	filter2D(image, topLeftI, ddepth, topLeft, anchor, delta, BORDER_DEFAULT);
	filter2D(image, topRightI, ddepth, topRight, anchor, delta, BORDER_DEFAULT);

	if (showFilter) {
		imshow("botLeftI", botLeftI);
		imshow("botRightI", botRightI);
		imshow("topLeftI", topLeftI);
		imshow("topRightI", topRightI);
		waitKey(0);
		destroyWindow("botLeftI");
		destroyWindow("botRightI");
		destroyWindow("topLeftI");
		destroyWindow("topRightI");
	}

	return possiblePoints(topLeftI, topRightI, botLeftI, botRightI, showFoundPoints);
}


std::list<Circle> CircleDetection::possiblePoints(Mat topLeftPoints, Mat topRightPoints, Mat botLeftPoints, Mat botRightPoints, bool showFoundPoints) {
	std::list<Circle> points = std::list<Circle>();
	///optimierung: Bildpunkte Schwarzen die nichtmehr angeschaut werden sollen
	int redThreshhold = 0;

	int alpha = 5; ///möglicher offset nach links/rechts
	int beta = 5; ///möglicher offset nach oben/unten
	int generalDistance = 25;///generelle distanz zwischen den zu findenden Punkten

	Mat test = Mat::zeros(topLeftPoints.rows, topLeftPoints.cols, 0); ///test Matrix zum visualisieren der zwischenergebnisse
	Mat test2 = Mat::zeros(topLeftPoints.rows, topLeftPoints.cols, 0);

	int x = topLeftPoints.cols * 3;/// = i
	int y = topLeftPoints.rows;/// = j

	bool blackBotLeft = true, 
		 blackBotRight = true, 
		 blackTopRight = true;

	for (int i = alpha; i < x - (alpha * 3 + generalDistance + 2); i = i + 3) {
		for (int j = beta; j < y - (beta + generalDistance); j++) {

			float blue = topLeftPoints.at<uchar>(j, i);
			float green = topLeftPoints.at<uchar>(j, i + 1);
			float red = topLeftPoints.at<uchar>(j, i + 2);

			if (red > redThreshhold) {
				Point2i topLeftPoint = Point2i(j, i);
				///a und b iterieren über alpha
				for (int a = -alpha; a < (alpha * 3); a = a + 3) {
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
								blackBotLeft = false;
								Point2i botLeftPoint = Point2i(j + generalDistance + b, i + a);

								///finden des unteren rechten Punktes
								blue = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + a);
								green = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 1 + a);
								red = botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 2 + a);
								if (red > redThreshhold) {
									blackBotRight = false;
									Point2i botRightPoint = Point2i(j + generalDistance + b, i + generalDistance + a);

									///finden des oberen rechten Punktes
									blue = topRightPoints.at<uchar>(j + b, i + generalDistance + a);
									green = topRightPoints.at<uchar>(j + b, i + generalDistance + 1 + a);
									red = topRightPoints.at<uchar>(j + b, i + generalDistance + 2 + a);
									if (red > redThreshhold) {
										blackTopRight = false;
										Point2i topRightPoint = Point2i(j + b, i + generalDistance + a);

										///schauen ob sich oberer rechter punkt verbindet
										//if(topRightPoint.x > topLeftPoint.x)
										//	waitKey(0);
										if (abs(topLeftPoint.x - topRightPoint.x) < alpha * 2 /*&& abs( topRightPoint.y - topLeftPoint.y) < beta * 2*/) {
											Circle r = Circle(topLeftPoint, topRightPoint, botLeftPoint, botRightPoint);

											//redunktion von 5200 aud 230
											bool toClose = false;
											for each(Circle c in points) {
												if (sqrt((c.getCenter().x - r.getCenter().x) * (c.getCenter().x - r.getCenter().x) +
													(c.getCenter().y - r.getCenter().y) * (c.getCenter().y - r.getCenter().y)) < generalDistance)
													toClose = true;
											}
											if (!toClose) {
												//reduktion von 5200 auf 170 bei 5
												//reduktion von 5200 auf 67 bei 10
												//reduktion von 5200 auf 41 bei 30
												int radiusOffset = 10;
												//noch nicht korrekt, man geht nicht von 0,0 aus
												for (int q = 0; q < (r.getRadius() + radiusOffset * 2) * 3 && q + x< x; q++) {
													for (int w = 0; w < r.getRadius() + radiusOffset * 2 && w + y < y; w++) {
														int radiusOffset = 5;
														topLeftPoints.at<uchar>(j + w, i + q) = 0;
													}
												}
												test.at<uchar>(topLeftPoint.x, topLeftPoint.y / 3) = 255;
												test.at<uchar>(botRightPoint.x, botRightPoint.y / 3) = 255;
												test.at<uchar>(botLeftPoint.x, botLeftPoint.y / 3) = 255;
												test.at<uchar>(topRightPoint.x, topRightPoint.y / 3) = 255;
												//beide reduktionsmethoden: 28
												points.push_back(r);
											}	
										}
									}
								}
							}
						}
						///punkte schwarzen die sowieso keine folgepunkte haben
						if (blackBotLeft) {
							botLeftPoints.at<uchar>(j + generalDistance + b, i + a) = 0;
							botLeftPoints.at<uchar>(j + generalDistance + b, i + 1 + a) = 0;
							botLeftPoints.at<uchar>(j + generalDistance + b, i + 2 + a) = 0;
						}
						if (blackBotRight) {
							botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + a) = 0;
							botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 1 + a) = 0;
							botRightPoints.at<uchar>(j + generalDistance + b, i + generalDistance + 2 + a) = 0;
						}
						if (blackTopRight) {
							topRightPoints.at<uchar>(j + b, i + generalDistance + a) = 0;
							topRightPoints.at<uchar>(j + b, i + generalDistance + 1 + a) = 0;
							topRightPoints.at<uchar>(j + b, i + generalDistance + 2 + a) = 0;
						}
					}
				}
			}
		}
	}
	if (showFoundPoints) {
		imshow("found points", test);
		waitKey(0);
		destroyWindow("found points");
	}

	return points;
}
