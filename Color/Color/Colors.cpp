#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
VideoCapture cap(0);
Mat img;
vector<vector<int>> newPoints;
vector<vector<int>> myColors{ {129,60,205,174,150,255},//  �������
							  {39,132,65,92,252,255} }; // �������
vector<Scalar> myColorValues{ {255,0,255},
								{0,255,0} };
Point getContours(Mat image) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		string objectType;
		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;
		}
	}
	return myPoint;
}
vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		Point myPoint = getContours(mask);
		if (myPoint.x != 0) {
			newPoints.push_back({ myPoint.x,myPoint.y,i });
		}
	}
	return newPoints;
}
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}
}
void main() {
	while (true) {
		cap.read(img);
		newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		imshow("Image", img);
		waitKey(1);
	}
}