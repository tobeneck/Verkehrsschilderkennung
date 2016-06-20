#include <list>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Triangle.hpp"
#include "Circle.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
using namespace cv;
using namespace ml;

#ifndef NET_H
#define NET_H




class ClassificationNetwork {
private:
	Mat getInputDataFromSignsVector(std::list<Mat> roadSigns);

	Mat getOutputDataFromSignsVector(std::list<Mat> roadSigns);
public:
	std::list<int> classificate(std::list<Mat>);
	ClassificationNetwork();
	void train(std::list<Mat> trainingData);

	//Ptr<ANN_MLP> ANN_MLP::create

};
#endif