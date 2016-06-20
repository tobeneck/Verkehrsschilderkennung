#include "ClassificationNetwork.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace ml;



Ptr<ml::ANN_MLP> nn;
cv::FileStorage fs;
String filename = "../Data/Trainingsdaten/Rund/mlp.xml";
String mlpname = "mlp";

Mat getInputDataFromImages(std::list<Mat> roadSigns) {
	cv::Mat roadSignsImageData;

	for (Mat m : roadSigns) {
		try {
			Mat image = Mat(m.rows, m.cols * 3, CV_32FC1);

			for (int i = 0; i < m.cols * 3; i = i + 3) {
				for (int j = 0; j < m.rows; j++) {
					image.at<float>(j,i) = m.at<uchar>(j, i);
					image.at<float>(j, i + 1) = m.at<uchar>(j, i + 1);
					image.at<float>(j, i + 2) = m.at<uchar>(j, i + 2);
				}
			}
			roadSignsImageData.push_back(image.reshape(0, 1));
		}
		catch (Exception e) {
			system("cls");
		}
	}

	roadSignsImageData.convertTo(roadSignsImageData, CV_32FC1);
	return roadSignsImageData;
}

std::list<int> ClassificationNetwork::classificate(std::list<Mat> input)
{
	std::list<int> results;
	Mat in = getInputDataFromImages(input);
	for (int i = 0; i < in.rows; i++) {
		cv::Mat out;
  		nn->predict(in.row(i), out);

		for (int y = 0; y< out.cols; y++) {
			if (out.at<float>(0,y) > 1.0f) {
				results.push_back(y);
			}
			std::cout << out.row(0).col(y) << ",";
		}

		std::cout << std::endl;
	}

	return results;
}

ClassificationNetwork::ClassificationNetwork()
{
	///*nn = cv::ml::ANN_MLP::create();*/
	//fs = cv::FileStorage(filename, FileStorage::READ);//FileStorage::READ +
	////fs.getFirstTopLevelNode();
	//nn->read(fs.getFirstTopLevelNode());
	//nn->loadFromString<ANN_MLP>(filename);
	////nn->load<ANN_MLP>(filename);//,mlp
	//waitKey(0);
}

void ClassificationNetwork::train(std::list<Mat> trainingData) {
	nn = cv::ml::ANN_MLP::create();
	nn->setTrainMethod(cv::ml::ANN_MLP::BACKPROP);
	nn->setBackpropMomentumScale(0.1);
	nn->setBackpropWeightScale(0.1);
	nn->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, (int)50, 0.0001));//100000, 1e-6

	Mat inputTrainingData = getInputDataFromImages(trainingData);

	///erste 7 bilder sind ausgang 1, zweite 7 bilder sind ausgang 2, dritte 7 bilder sind ausgang 3, ausgang 4 ist keine richtige eingabe
	cv::Mat outputTraining(8, 4, CV_32FC1);
	//outputTraining.at<float>(0, 0) = 1; outputTraining.at<float>(0, 1) = 0;  outputTraining.at<float>(0, 2) = 0;  outputTraining.at<float>(0, 3) = 0;
	//outputTraining.at<float>(1, 0) = 0; outputTraining.at<float>(1, 1) = 1;  outputTraining.at<float>(1, 2) = 0;  outputTraining.at<float>(1, 3) = 0;
	//outputTraining.at<float>(2, 0) = 0; outputTraining.at<float>(2, 1) = 0;  outputTraining.at<float>(2, 2) = 1;  outputTraining.at<float>(2, 3) = 0;
	//outputTraining.at<float>(3, 0) = 0; outputTraining.at<float>(3, 1) = 0;  outputTraining.at<float>(3, 2) = 0;  outputTraining.at<float>(3, 3) = 1;
	outputTraining.at<float>(0, 0) = 1; outputTraining.at<float>(0, 1) = 0;  outputTraining.at<float>(0, 2) = 0;  outputTraining.at<float>(0, 3) = 0;
	outputTraining.at<float>(1, 0) = 1; outputTraining.at<float>(1, 1) = 0;  outputTraining.at<float>(1, 2) = 0;  outputTraining.at<float>(1, 3) = 0;
	outputTraining.at<float>(2, 0) = 0; outputTraining.at<float>(2, 1) = 1;  outputTraining.at<float>(2, 2) = 0;  outputTraining.at<float>(2, 3) = 0;
	outputTraining.at<float>(3, 0) = 0; outputTraining.at<float>(3, 1) = 1;  outputTraining.at<float>(3, 2) = 0;  outputTraining.at<float>(3, 3) = 0;
	outputTraining.at<float>(4, 0) = 0; outputTraining.at<float>(4, 1) = 0;  outputTraining.at<float>(4, 2) = 1;  outputTraining.at<float>(4, 3) = 0;
	outputTraining.at<float>(5, 0) = 0; outputTraining.at<float>(5, 1) = 0;  outputTraining.at<float>(5, 2) = 1;  outputTraining.at<float>(5, 3) = 0;
	outputTraining.at<float>(6, 0) = 0; outputTraining.at<float>(6, 1) = 0;  outputTraining.at<float>(6, 2) = 0;  outputTraining.at<float>(6, 3) = 1;
	outputTraining.at<float>(7, 0) = 0; outputTraining.at<float>(7, 1) = 0;  outputTraining.at<float>(7, 2) = 0;  outputTraining.at<float>(7, 3) = 1;
	//outputTraining.at<float>(8, 0) = 0; outputTraining.at<float>(8, 1) = 0;  outputTraining.at<float>(8, 2) = 0;  outputTraining.at<float>(8, 3) = 1;
	//outputTraining.at<float>(9, 0) = 0; outputTraining.at<float>(9, 1) = 1;  outputTraining.at<float>(9, 2) = 0;  outputTraining.at<float>(9, 3) = 0;
	//outputTraining.at<float>(10, 0) = 0; outputTraining.at<float>(10, 1) = 0;  outputTraining.at<float>(10, 2) = 1;  outputTraining.at<float>(10, 3) = 0;
	//outputTraining.at<float>(11, 0) = 0; outputTraining.at<float>(11, 1) = 0;  outputTraining.at<float>(11, 2) = 0;  outputTraining.at<float>(11, 3) = 0;
	//outputTraining.at<float>(12, 0) = 1; outputTraining.at<float>(12, 1) = 1;  outputTraining.at<float>(12, 2) = 0;  outputTraining.at<float>(12, 3) = 0;
	//outputTraining.at<float>(13, 0) = 0; outputTraining.at<float>(13, 1) = 0;  outputTraining.at<float>(13, 2) = 1;  outputTraining.at<float>(13, 3) = 0;
	//outputTraining.at<float>(14, 0) = 0; outputTraining.at<float>(14, 1) = 0;  outputTraining.at<float>(14, 2) = 0;  outputTraining.at<float>(14, 3) = 0;
	//outputTraining.at<float>(15, 0) = 1; outputTraining.at<float>(15, 1) = 0;  outputTraining.at<float>(15, 2) = 1;  outputTraining.at<float>(15, 3) = 0;
	//outputTraining.at<float>(16, 0) = 0; outputTraining.at<float>(16, 1) = 1;  outputTraining.at<float>(16, 2) = 0;  outputTraining.at<float>(16, 3) = 0;
	//outputTraining.at<float>(17, 0) = 0; outputTraining.at<float>(17, 1) = 0;  outputTraining.at<float>(17, 2) = 0;  outputTraining.at<float>(17, 3) = 0;
	//outputTraining.at<float>(18, 0) = 1; outputTraining.at<float>(18, 1) = 0;  outputTraining.at<float>(18, 2) = 1;  outputTraining.at<float>(18, 3) = 0;
	//outputTraining.at<float>(19, 0) = 0; outputTraining.at<float>(19, 1) = 1;  outputTraining.at<float>(19, 2) = 0;  outputTraining.at<float>(19, 3) = 0;
	//outputTraining.at<float>(20, 0) = 0; outputTraining.at<float>(20, 1) = 0;  outputTraining.at<float>(20, 2) = 0;  outputTraining.at<float>(20, 3) = 0;
	//outputTraining.at<float>(21, 0) = 1; outputTraining.at<float>(21, 1) = 0;  outputTraining.at<float>(21, 2) = 0;  outputTraining.at<float>(21, 3) = 1;
	//outputTraining.at<float>(22, 0) = 0; outputTraining.at<float>(22, 1) = 0;  outputTraining.at<float>(22, 2) = 0;  outputTraining.at<float>(22, 3) = 1;
	//outputTraining.at<float>(23, 0) = 0; outputTraining.at<float>(23, 1) = 0;  outputTraining.at<float>(23, 2) = 0;  outputTraining.at<float>(23, 3) = 1;
	//outputTraining.at<float>(24, 0) = 0; outputTraining.at<float>(24, 1) = 0;  outputTraining.at<float>(24, 2) = 0;  outputTraining.at<float>(24, 3) = 1;

	cv::Mat layers = cv::Mat(3, 1, CV_32SC1);
	layers.row(0) = cv::Scalar(inputTrainingData.cols);
	layers.row(1) = cv::Scalar(45);
	layers.row(1) = cv::Scalar(30);
	layers.row(2) = cv::Scalar(outputTraining.cols);
	nn->setLayerSizes(layers);
	nn->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
 	nn->train(inputTrainingData, cv::ml::SampleTypes::ROW_SAMPLE, outputTraining);

	fs = cv::FileStorage(filename, cv::FileStorage::WRITE);
	nn->write(fs);
	nn->save(filename);

	/*
	cv::Mat out;
	nn->predict(inputTrainingData.row(0), out);

	for (int y = 0; y< out.cols; y++) {
		std::cout << out.row(0).col(y) << ",";
	}

	std::cout << std::endl;
	*/
}

