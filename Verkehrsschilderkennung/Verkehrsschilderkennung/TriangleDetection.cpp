                                                                           #include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "TriangleDetection.hpp"
#include <list>

using namespace cv;

std::list<Triangle> TriangleDetection::detectTriangle(Mat image, bool showFilter, bool showFoundTriangles) {


		Mat botLeftI, botRightI, topI;
		
		filter2D(image, botLeftI, ddepth, botLeftTriangle, anchor, delta, BORDER_DEFAULT);
		filter2D(image, botRightI, ddepth, botRightTriangle, anchor, delta, BORDER_DEFAULT);
		filter2D(image, topI, ddepth, topTriangle, anchor, delta, BORDER_DEFAULT);
		
		if (showFilter) {
			imshow("leftI", botLeftI);
			imshow("rightI", botRightI);
			imshow("topI", topI);
			waitKey(0);
			destroyWindow("leftI");
			destroyWindow("rightI");
			destroyWindow("topI");
		}

		return possiblePoints(topI, botLeftI, botRightI, showFoundTriangles);
	}


	std::list<Triangle> TriangleDetection::possiblePoints(Mat topPoints, Mat leftPoints, Mat rightPoints, bool showFoundTriangles) {
		std::list<Triangle> points = std::list<Triangle>();
		///optimierung: Bildpunkte Schwarzen die nichtmehr angeschaut werden sollen
		int redThreshhold = 0;

		int alpha = 2; ///möglicher offset nach links/rechts
		int beta = 2; ///möglicher offset nach oben/unten
		int generalDistanceX = 35;///generelle distanz zwischen den zu findenden Punkten
		int generalDistanceY = 25;

		Mat test = Mat::zeros(topPoints.rows, topPoints.cols, 0); ///test Matrix zum visualisieren der zwischenergebnisse
		Mat test2 = Mat::zeros(topPoints.rows, topPoints.cols, 0);

		int x = topPoints.cols * 3;/// = i
		int y = topPoints.rows;/// = j

		bool blackRight = true,
			blackLeft = true;

		for (int i = alpha + generalDistanceX / 2; i < x - (alpha * 3 + generalDistanceX + 2); i = i + 3) {
			for (int j = beta; j < y - (beta + generalDistanceY); j++) {

				float blue = topPoints.at<uchar>(j, i);
				float green = topPoints.at<uchar>(j, i + 1);
				float red = topPoints.at<uchar>(j, i + 2);

				if (red > redThreshhold) {
					Point2i topPoint = Point2i(j, i);
					///a und b iterieren über alpha
					for (int a = -alpha; a < (alpha * 3); a = a + 3) {
						for (int b = -beta; b < beta; b++) {

							if (i + a < 0)
								a = 0;//falsch?
							if (j + b < 0)
								b = 0;
							if ((i + a + generalDistanceX < x) && (j + b + generalDistanceY < y)) {

								///finden des unteren linken Punktes
								blue = leftPoints.at<uchar>(j + generalDistanceY + b, i - generalDistanceX / 2 + a);
								green = leftPoints.at<uchar>(j + generalDistanceY + b, i - generalDistanceX / 2 + 1 + a);
								red = leftPoints.at<uchar>(j + generalDistanceY + b, i - generalDistanceX / 2 + 2 + a);
								if (red > redThreshhold) {
									blackLeft = false;
									Point2i leftPoint = Point2i(j + generalDistanceY + b, i - generalDistanceX / 2 + a);

									///finden des unteren rechten Punktes
									blue = rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX / 2 + a);
									green = rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX / 2 + 1 + a);
									red = rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX / 2 + 2 + a);
									if (red > redThreshhold) {
										blackRight = false;
										Point2i rightPoint = Point2i(j + generalDistanceY + b, i + generalDistanceX / 2 + a);

											///schauen ob sich oberer rechter punkt verbindet
											//if(topRightPoint.x > topLeftPoint.x)
											//	waitKey(0);
											if (abs(leftPoint.x - rightPoint.x) < alpha * 2 /*&& abs( topRightPoint.y - topLeftPoint.y) < beta * 2*/) {
												Triangle r = Triangle(topPoint, leftPoint, rightPoint);
												//redunktion von 5200 aud 230
												bool toClose = false;
												for each(Triangle t in points) {
													if (sqrt((t.getCenter().x - r.getCenter().x) * (t.getCenter().x - r.getCenter().x) +
														(t.getCenter().y - r.getCenter().y) * (t.getCenter().y - r.getCenter().y)) < generalDistanceY)
														toClose = true;
												}
												if (!toClose) {
													//reduktion von 5200 auf 170 bei 5
													//reduktion von 5200 auf 67 bei 10
													//reduktion von 5200 auf 41 bei 30
													//int radiusOffset = 10;
													////noch nicht korrekt, man geht nicht von 0,0 aus
													//for (int q = 0; q < (r.getRadius() + radiusOffset * 2) * 3; q++) {
													//	for (int w = 0; w < r.getRadius() + radiusOffset * 2; w++) {
													//		int radiusOffset = 5;
													//		topPoints.at<uchar>(j + w, i + q) = 0;
													//	}
													//}
													//beide reduktionsmethoden: 28
													test.at<uchar>(topPoint.x, topPoint.y / 3) = 255;
													test.at<uchar>(leftPoint.x, leftPoint.y / 3) = 255;
													test.at<uchar>(rightPoint.x, rightPoint.y / 3) = 255;

													points.push_back(r);
												}
											}
										}
									}
								}
							///punkte schwarzen die sowieso keine folgepunkte haben
							if (blackLeft) {
								leftPoints.at<uchar>(j + generalDistanceY + b, i + a) = 0;
								leftPoints.at<uchar>(j + generalDistanceY + b, i + 1 + a) = 0;
								leftPoints.at<uchar>(j + generalDistanceY + b, i + 2 + a) = 0;
							}
							if (blackRight) {
								rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX + a) = 0;
								rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX + 1 + a) = 0;
								rightPoints.at<uchar>(j + generalDistanceY + b, i + generalDistanceX + 2 + a) = 0;
							}
						}
					}
				}
			}
		}
		if (showFoundTriangles) {
			imshow("gefundenne eckpunkte", test);
			waitKey(0);
			destroyWindow("gefundenne eckpunkte");
		}

		return points;
	}