#include "OwnFilter.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Mat OwnFilter::ownRedWhiteBlackThreshold(Mat image) {
	Mat threshholdImage = image;

	float a = 0.5;
	float b = 2.1;
	float c = -0.9;
	float d = 0.8;

	float blackDist = 15;
	float blackMin = 0;
	float blackMax = 50;
	float whiteMin = 245;
	float whiteMax = 255;

	///own threshholding
	int x = image.cols;
	int y = image.rows;
	for (int i = 0; i < x * 3; i = i + 3) {
		for (int j = 0; j < y; j++) {
			Scalar intensity = image.at<uchar>(j, i);
			float b = image.at<uchar>(j, i);
			float g = image.at<uchar>(j, i + 1);
			float r = image.at<uchar>(j, i + 2);
			///r != 0!
			if (r == 0)
				r = 1;
			if ((a <= log(r / g) && log(r / g) <= b //red threshhold
				&& c <= log(b / g) && log(b / g) <= d)
			||((r >= blackMin && g >= blackMin && b >= blackMin
				&& r <= blackMax && g <= blackMax && b <= blackMax)//black threshhold
			&&(abs(r - g) <= blackDist && abs(r - b) <= blackDist && abs(g - b) <= blackDist) )//another black
				){
				threshholdImage.at<uchar>(j, i+2) = r;
				threshholdImage.at<uchar>(j, i + 1) = g;
				threshholdImage.at<uchar>(j, i + 0) = b;
			}
			else {
				threshholdImage.at<uchar>(j, i + 2) = 255;
				threshholdImage.at<uchar>(j, i + 1) = 255;
				threshholdImage.at<uchar>(j, i + 0) = 255;
			}


		}
	}

	return image;
}

Mat OwnFilter::ownRedThreshold(Mat image) {
	Mat threshholdImage = image;

	float a = 0.5;
	float b = 2.1;
	float c = -0.9;
	float d = 0.8;

	///own threshholding
	int x = image.cols;
	int y = image.rows;
	for (int i = 0; i < x * 3; i = i + 3) {
		for (int j = 0; j < y; j++) {
			Scalar intensity = image.at<uchar>(j, i);
			float b = image.at<uchar>(j, i);
			float g = image.at<uchar>(j, i + 1);
			float r = image.at<uchar>(j, i + 2);
			///r != 0!
			if (r == 0)
				r = 1;
			if (a <= log(r / g) && log(r / g) <= b
				&& c <= log(b / g) && log(b / g) <= d) {

				//threshholdImage.at<uchar>(j, i+2) = r;
				//threshholdImage.at<uchar>(j, i + 1) = g;
				//threshholdImage.at<uchar>(j, i + 0) = b;
				threshholdImage.at<uchar>(j, i + 2) = 255;
				threshholdImage.at<uchar>(j, i + 1) = 255;
				threshholdImage.at<uchar>(j, i + 0) = 255;
			}
			else {
				threshholdImage.at<uchar>(j, i + 2) = 0;
				threshholdImage.at<uchar>(j, i + 1) = 0;
				threshholdImage.at<uchar>(j, i + 0) = 0;
			}


		}
	}

	return image;
}