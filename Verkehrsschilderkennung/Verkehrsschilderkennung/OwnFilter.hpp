#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
using namespace cv;

class OwnFilter {
public:
	Mat ownRedWhiteBlackThreshold(Mat);
	Mat ownRedThreshold(Mat);

};