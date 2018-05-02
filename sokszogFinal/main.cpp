#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

const int w = 500;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

int findMinX(const vector<vector<Point>>& contours) {
	int min = 9999;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if (contours[i][j].x < min) {
				min = contours[i][j].x;
			}
		}
	}
	return min;
}
int findMinY(const vector<vector<Point>>& contours) {
	int min = 9999;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if (contours[i][j].y < min) {
				min = contours[i][j].x;
			}
		}
	}
	return min;
}
int findMaxX(const vector<vector<Point>>& contours) {
	int max = -9999;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if (contours[i][j].x > max) {
				max = contours[i][j].x;
			}
		}
	}
	return max;
}
int findMaxY(const vector<vector<Point>>& contours) {
	int max = -9999;
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if (contours[i][j].y > max) {
				max = contours[i][j].x;
			}
		}
	}
	return max;
}

int angleOf(const vector<Point>& line0, const vector<Point>& line1) {
	for (int i = 0; i < line0.size(); i++) {
		cout << line0[i] << " : ";
	}
	cout << "\n\n\n";
	Point vektor0(line0[1].x-line0[0].x,line0[1].y- line0[0].y);
	Point vektor1(line1[1].x-line1[0].x,line1[1].y- line1[0].y);

	cout << vektor0 << "\n" << vektor1;

	float dot = vektor0.x*vektor1.x + vektor0.y*vektor1.y;
	float vekt0_len = sqrtf(vektor0.x*vektor0.x+vektor0.y*vektor0.y);
	float vekt1_len = sqrtf(vektor1.x*vektor1.x + vektor1.y*vektor1.y);

	float cos_alfa = dot / (vekt0_len*vekt1_len);
	float deg = acosf(cos_alfa) * 180 / 3.1415;
	return (int)deg;
}


int main(int argc, char** argv)
{
	Mat img = Mat::zeros(w, w, CV_8UC1);
	Mat rsc = imread("3szog.jpg", IMREAD_GRAYSCALE);
	Canny(rsc, img, 100, 200, 3);

	namedWindow("image");
	imshow("image", img);


	vector<vector<Point> > contours0;
	findContours(img, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	for (size_t k = 0; k < contours0.size(); k++) {
		contours.push_back(contours0[k]);
		approxPolyDP(Mat(contours0[k]), contours[k], 12, true);
	}


	namedWindow("contours");
	Mat cnt_img = Mat::zeros(w, w, CV_8UC3);
	drawContours(cnt_img, contours, -1, Scalar(128, 255, 255), 3, LINE_AA, hierarchy, 1);

	string s = std::to_string(contours.size());
	int labelX = (findMinX(contours) + findMaxX(contours)) / 2 - 5;
	int labelY = (findMinY(contours) + findMaxY(contours)) / 2 + 3;
	putText(cnt_img, s, Point(labelX, labelY),FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255, 255, 255), 1.0);

	string s2 = std::to_string(angleOf(contours[0], contours[1]));

	putText(cnt_img, s2, Point(contours[0][1].x, contours[0][1].y+15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255, 255, 255), 1.0);

	
	cout << contours.size() << "\n";

	imshow("contours", cnt_img);
	waitKey();

	return 0;
}