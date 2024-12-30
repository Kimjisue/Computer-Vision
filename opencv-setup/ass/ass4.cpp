#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void dr_li(Mat canny, Mat& dr_line, double fi_degree, double se_degree, int check) {
	Mat result = dr_line;
	float rho, theta, a, b, x0, y0, xadd, yadd, rsum = 0, tsum = 0;
	Point p1, p2;
	vector<Vec2f> lines;

	HoughLines(canny, lines, 1, CV_PI / 180, 100, 0, 0, fi_degree * CV_PI / 180, se_degree * CV_PI / 180);
	if (check == 0) {
		xadd = 200;
		yadd = 400;
	}
	else {
		xadd = 600;
		yadd = 400;
	}
	for (int i = 0; i < lines.size(); i++) {
		rsum += lines[i][0];
		tsum += lines[i][1];
	}

	rho = rsum / lines.size();
	theta = tsum / lines.size();
	a = cos(theta);
	b = sin(theta);
	x0 = a * rho + xadd;
	y0 = b * rho + yadd;

	p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
	p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
	line(result, p1, p2, Scalar(0, 0, 255), 3, 8);

}
int main() {
	Mat frame, gray_scale_image, lt_canny, ri_canny;
	double fps, time_in_mesc;
	VideoCapture cap;
	Rect left_roi(200, 400, 400, 200), right_roi(600, 400, 400, 200);

	// check if file exists. if none program ends
    //if (cap.open("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/video.mp4") == 0){
	if (cap.open("road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}
	fps = cap.get(CAP_PROP_FPS);

	do {
		cap >> frame;
		time_in_mesc = cap.get(CAP_PROP_POS_MSEC);
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}
		cvtColor(frame, gray_scale_image, CV_BGR2GRAY);
		lt_canny = gray_scale_image(left_roi);
		ri_canny = gray_scale_image(right_roi);
		GaussianBlur(lt_canny, lt_canny, Size(15, 15), BORDER_DEFAULT);
		Canny(lt_canny, lt_canny, 10, 60, 3);
		GaussianBlur(ri_canny, ri_canny, Size(15, 15), BORDER_DEFAULT);
		Canny(ri_canny, ri_canny, 10, 60, 3);


		namedWindow("Left canny");
		moveWindow("Left canny", 200, 0);
		imshow("Left canny", lt_canny);

		namedWindow("Right canny");
		moveWindow("Right canny", 600, 0);
		imshow("Right canny", ri_canny);


		dr_li(lt_canny, frame, 30, 60, 0);
		dr_li(ri_canny, frame, 120, 150, 1);
		imshow("Frame", frame);
		waitKey(10);
	} while (time_in_mesc <= 20000.0);

	waitKey(0);

}