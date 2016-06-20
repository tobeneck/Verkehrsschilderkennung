#include "SFML\Graphics.hpp"
#include "Button.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "TriangleDetection.hpp"
#include "RectangleDetection.hpp"
#include "OwnFilter.hpp"
#include "CuttingTheSigns.hpp"
#include "Circle.hpp"
#include "CircleDetection.hpp"
#include "Rectangle.hpp"
#include "ClassificationNetwork.hpp"
#include "Constands.hpp"
#include <list>
#include <iostream>

using namespace cv;
using namespace std;

/// Declare variables
Mat src, src2;
Mat dst;

Constands constands;

CircleDetection circleDetection = CircleDetection();
TriangleDetection triangleDetection = TriangleDetection();

ClassificationNetwork circleClass = ClassificationNetwork();
ClassificationNetwork triClass = ClassificationNetwork();

OwnFilter of = OwnFilter();

CuttingTheSigns cuttTheSigns = CuttingTheSigns();


Button switchLeft;
Button switchRight;
Button start;
Button triangular;
Button triangular_threshhold;
Button triangular_Filter;
Button triangular_foundTriangles;
Button triangular_cut;
Button triangular_classificate;
Button circular;
Button circular_threshhold;
Button circular_Filter;
Button circular_foundTriangles;
Button circular_cut;
Button circular_classificate;

sf::Texture tex;
sf::Sprite sprite;

void detectCircle();
void detectTriangle();
void draw(sf::RenderWindow &window);
int testImageCount = 0;

sf::Clock timer;

int main(int argc, char** argv)
{
	/// Load an image
	src = cv::imread(std::string("../Data/Test Images/test0"));
	src2 = cv::imread(std::string("../Data/Test Images/test0"));

	constands = Constands();

	///load the classificationSigns
	std::list<Mat> trainingRund;
	std::list<Mat> trainingTri;
	for (int i = 1; i < 9; i++) {
		Mat imageToLoadCirc;
		String filename = "../Data/Trainingsdaten/Rund/";
		stringstream ss;
		ss << i;
		filename = filename + ss.str();
		filename = filename + ".jpg";
		imageToLoadCirc = imread(filename);
		trainingRund.push_back(imageToLoadCirc);

		Mat imageToLoadTri;
		filename = "../Data/Trainingsdaten/Dreieck/";
		filename = filename + ss.str();
		filename = filename + ".jpg";
		imageToLoadTri = imread(filename);//, IMREAD_GRAYSCALE
		trainingTri.push_back(imageToLoadTri);
	}

	///initialize and train the Neuronal Nets
	std::cout << "Training the Nets...";
	circleClass.train(trainingRund);
	triClass.train(trainingTri);

	system("cls");

	/// Create a OpenCV window
	//imshow("Original Picture", src2);

	dst = of.ownRedThreshold(src);//of.ownRedThreshold(src);

	bool isKlicked = false;

	switchLeft = Button(std::string("<- left"), sf::Vector2f(230, 530));
	switchRight = Button(std::string("right ->"), sf::Vector2f(470, 530));
	start = Button(std::string("start"), sf::Vector2f(350, 530));
	triangular = Button(std::string("Triangular"), sf::Vector2f(20, 20));
	triangular_threshhold = Button(std::string("Threshhold"), sf::Vector2f(20, 90));
	triangular_Filter = Button(std::string("Filter"), sf::Vector2f(20, 160));
	triangular_foundTriangles = Button(std::string("Found"), sf::Vector2f(20, 230));
	triangular_cut = Button(std::string("Cut"), sf::Vector2f(20, 300));
	triangular_classificate = Button(std::string("Classification"), sf::Vector2f(20, 370));
	circular = Button(std::string("Circular"), sf::Vector2f(140, 20));
	circular_threshhold = Button(std::string("Threshhold"), sf::Vector2f(140, 90));
	circular_Filter = Button(std::string("Filter"), sf::Vector2f(140, 160));
	circular_foundTriangles = Button(std::string("Found"), sf::Vector2f(140, 230));
	circular_cut = Button(std::string("Cut"), sf::Vector2f(140, 300));
	circular_classificate = Button(std::string("Classification"), sf::Vector2f(140, 370));

	//bild von 
	//x 260 - 780
	//y 20 - 420

	tex.loadFromFile(std::string("../Data/Test Images/test0"));
	sprite.setTexture(tex);

	if (sprite.getTextureRect().width* sprite.getScale().x > 512)
		sprite.setScale(520.0 / sprite.getTextureRect().width, 520.0 / sprite.getTextureRect().width );
	if (sprite.getTextureRect().height * sprite.getScale().y > 400) 
		sprite.setScale(400.0 / sprite.getTextureRect().height, 400.0 / sprite.getTextureRect().height);
	sprite.setPosition(520.0 - (sprite.getTextureRect().width * sprite.getScale().x / 2.0), 220.0 - sprite.getTextureRect().height * sprite.getScale().y / 2.0);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Controll Window");

	while (window.isOpen())
	{
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			switch (Event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}
		///update code here
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isKlicked) {
			isKlicked = true;
			switchLeft.checkClick(sf::Mouse::getPosition(window));
			switchRight.checkClick(sf::Mouse::getPosition(window));;
			start.checkClick(sf::Mouse::getPosition(window));
			triangular.checkClick(sf::Mouse::getPosition(window));
			triangular_threshhold.checkClick(sf::Mouse::getPosition(window));
			triangular_Filter.checkClick(sf::Mouse::getPosition(window));
			triangular_foundTriangles.checkClick(sf::Mouse::getPosition(window));
			triangular_cut.checkClick(sf::Mouse::getPosition(window));
			triangular_classificate.checkClick(sf::Mouse::getPosition(window));
			circular.checkClick(sf::Mouse::getPosition(window));
			circular_threshhold.checkClick(sf::Mouse::getPosition(window));
			circular_Filter.checkClick(sf::Mouse::getPosition(window));
			circular_foundTriangles.checkClick(sf::Mouse::getPosition(window));
			circular_cut.checkClick(sf::Mouse::getPosition(window));
			circular_classificate.checkClick(sf::Mouse::getPosition(window));
		}
		if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			isKlicked = false;

		constands.circularB = circular.getVar();

		constands.circularB = (circular.getVar());
		constands.circular_threshholdB = (circular_threshhold.getVar());
		constands.circular_FilterB = (circular_Filter.getVar());
		constands.circular_foundCirclesB = (circular_foundTriangles.getVar());
		constands.circular_cutB = (circular_cut.getVar());
		constands.circular_classificateB = (circular_classificate.getVar());

		constands.triangularB = (triangular.getVar());
		constands.triangular_threshholdB = (triangular_threshhold.getVar());
		constands.triangular_FilterB = (triangular_Filter.getVar());
		constands.triangular_foundTrianglesB = (triangular_foundTriangles.getVar());
		constands.triangular_cutB = (triangular_cut.getVar());
		constands.triangular_classificateB = (triangular_classificate.getVar());

		if (start.getVar()) {
			draw(window);
			timer.restart();
			detectCircle();
			detectTriangle();
			String time = "\n Computation Time (in ms): ";
			stringstream ss;
			ss << timer.getElapsedTime().asMilliseconds();
			time = time + ss.str();
			std::cout << time;
			start.setState(false);
		}

		if (switchLeft.getVar())
			testImageCount++;
		if (switchRight.getVar())
			testImageCount--;
		int maxTestImages = 7;
		if (testImageCount > maxTestImages)
			testImageCount = 0;
		if (testImageCount < 0)
			testImageCount = maxTestImages;

		if (switchLeft.getVar() || switchRight.getVar()) {
			draw(window);
			String filename = "../Data/Test Images/test";
			stringstream ss;
			ss << testImageCount;
			filename = filename + ss.str();
			filename = filename + ".jpg";

			src = cv::imread(filename);
			src2 = cv::imread(filename);
			dst = of.ownRedThreshold(src);

			sprite.setScale(1, 1);
			tex.~Texture();
			tex.loadFromFile(filename);
			sprite.setTextureRect(sf::IntRect(0,0,tex.getSize().x, tex.getSize().y));
			sprite.setTexture(tex);
			if (sprite.getTextureRect().width* sprite.getScale().x > 512)
				sprite.setScale(520.0 / sprite.getTextureRect().width, 520.0 / sprite.getTextureRect().width);
			if (sprite.getTextureRect().height * sprite.getScale().y > 400)
				sprite.setScale(400.0 / sprite.getTextureRect().height, 400.0 / sprite.getTextureRect().height);
			sprite.setPosition(520.0 - (sprite.getTextureRect().width * sprite.getScale().x / 2.0), 220.0 - sprite.getTextureRect().height * sprite.getScale().y / 2.0);

			switchLeft.setState(false);
			switchRight.setState(false);
		}

		///draw code here
		draw(window);
	}

	return 0;
}

void draw(sf::RenderWindow &window) {
	window.clear(sf::Color(50, 50, 50));


	window.draw(sprite);

	switchLeft.draw(window);
	switchRight.draw(window);
	start.draw(window);
	triangular.draw(window);
	triangular_threshhold.draw(window);
	triangular_Filter.draw(window);
	triangular_foundTriangles.draw(window);
	triangular_cut.draw(window);
	triangular_classificate.draw(window);
	circular.draw(window);
	circular_threshhold.draw(window);
	circular_Filter.draw(window);
	circular_foundTriangles.draw(window);
	circular_cut.draw(window);
	circular_classificate.draw(window);

	window.display();
}

void detectCircle() {
	if (constands.circularB) {
		if (constands.circular_threshholdB) {
			imshow("Treshhold", dst);
			waitKey(0);
			destroyWindow("Treshhold");
		}
		std::list<Circle> circles = circleDetection.detectCircle(dst, constands.circular_FilterB, constands.circular_foundCirclesB);

		std::list<Mat> potentialCircleSigns = cuttTheSigns.cutTheCircles(src2, circles);
		if (constands.circular_cutB) {
			for each(Mat m in potentialCircleSigns) {
				imshow("Cut Sign Image", m);
				waitKey(0);
				destroyWindow("Cut Sign Image");
			}
		}
		if (constands.circular_classificateB) {
			std::list<int> resultsCircle = circleClass.classificate(potentialCircleSigns);

			bool zero = false,
				one = false,
				two = false,
				three = false;

			for each(int i in resultsCircle) {
				switch (i) {
				case 0:
					zero = true;
					break;
				case 1:
					one = true;
					break;
				case 2:
					two = true;
					break;
				default:
					three = true;
				}
			}
			if (zero) {
				std::cout << "No overtaking!";
			}
			if (one) {
				std::cout << "Speedlimit 60!";
			}
			if (two) {
				std::cout << "No turning left!";
			}
			if (!zero && !one && !two) {
				std::cout << "No Signs Detected!" << endl;
			}
		}
	}
}

void detectTriangle() {
	if (constands.triangularB) {
		if (constands.triangular_threshholdB) {
			imshow("Trheshhold", dst);
			waitKey(0);
			destroyWindow("Treshhold");
		}
		std::list<Triangle> triangles = triangleDetection.detectTriangle(dst, constands.triangular_FilterB, constands.triangular_foundTrianglesB);

		std::list<Mat> potentialTriangleSigns = cuttTheSigns.cutTheTriangles(src2, triangles);
		if (constands.triangular_cutB) {
			for each(Mat m in potentialTriangleSigns) {
				imshow("Cut Sign Image", m);
				waitKey(0);
				destroyWindow("Cut Sign Image");
			}
		}
		if (constands.triangular_classificateB) {
			std::list<int> resultsTri = triClass.classificate(potentialTriangleSigns);

			bool zero = false,
				one = false,
				two = false,
				three = false;

			for each(int i in resultsTri) {
				switch (i) {
				case 0:
					zero = true;
					break;
				case 1:
					one = true;
					break;
				case 2:
					two = true;
					break;
				default:
					three = true;
				}
			}

			if (zero) {
				std::cout << "Double Courve!";
			}
			if (one) {
				std::cout << "Animals crossing the Road!";
			}
			if (two) {
				std::cout << "Children crossing the Road!";
			}
			if (!zero && !one && !two) {
				std::cout << "No Signs Detected!";
			}
		}
	}
}
