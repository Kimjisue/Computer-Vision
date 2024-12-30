#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{   
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
    Mat foregroundMask, foregroundImg, backgroundImg;
    VideoCapture cap;
    bool startMoving = false;
    Mat roiFrame, leftFrame, rightFrame ,centerFrame;
    float rho, theta, a, b, x0, y0, total_rho, total_theta;
    float rho1, theta1, a1, b1, x1, y1, total_rho1, total_theta1;
    float rho2, theta2, a2, b2, x2, y2, total_rho2, total_theta2;
    Point p1, p2;
    Point p3, p4;
    Point p5, p6;
    vector<Vec2f> lines1, lines2,lines3;

    Rect rect_left(210, 270, 150, 200); 
    Rect rect_right(370, 270, 150, 200);
    Rect rect_center(350,270,30,200);
    Rect rect(260, 250, 180, 150);

    //if (cap.open("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/Project2_video.mp4") == 0) {
    if(cap.open("Project2_video.mp4")==0){
        cout << "no such file!" << endl;
        return -1;
    }

    int fps = cap.get(CAP_PROP_FPS);
    int delay = 1000 / fps;

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }

        roiFrame = frame(rect);
        leftFrame = frame(rect_left);
        rightFrame = frame(rect_right);
        centerFrame = frame(rect_center);

        cvtColor(leftFrame, leftFrame, COLOR_BGR2GRAY);
        blur(leftFrame,leftFrame,Size(5,5));
        Canny(leftFrame, leftFrame, 50, 200, 3);

        cvtColor(rightFrame, rightFrame, COLOR_BGR2GRAY);
        blur(rightFrame,rightFrame,Size(5,5));
        Canny(rightFrame, rightFrame, 50, 200, 3);

        cvtColor(centerFrame, centerFrame, COLOR_BGR2GRAY);
        blur(centerFrame,centerFrame,Size(5,5));
        Canny(centerFrame, centerFrame, 50, 200, 3);

        HoughLines(leftFrame, lines1, 1, CV_PI / 180, 60, 0, 0, CV_PI / 6, CV_PI / 3);

        if (lines1.size()) {
            total_rho = 0;
            total_theta = 0;
            for (int i = 0; i < lines1.size(); i++) {
                rho = lines1[i][0];
                theta = lines1[i][1];

                total_rho += rho;
                total_theta += theta;
            }
            total_rho /= lines1.size();
            total_theta /= lines1.size();

            a = cos(total_theta);
            b = sin(total_theta);
            x0 = a * total_rho;
            y0 = b * total_rho;
            p1 = Point(cvRound(x0 + 1000 * (-b)) + rect_left.x, cvRound(y0 + 1000 * a) + rect_left.y);
            p2 = Point(cvRound(x0 - 1000 * (-b)) + rect_left.x, cvRound(y0 - 1000 * a) + rect_left.y);
            //line(frame, p1, p2, Scalar(0, 255, 0), 3, 8);
        }

        HoughLines(rightFrame, lines2, 1, CV_PI / 180, 70, 0, 0, CV_PI / 3 * 2, CV_PI / 6 * 5);

        if (lines2.size()) {
            total_rho1 = 0;
            total_theta1 = 0;

            for (int i = 0; i < lines2.size(); i++) {
                rho1 = lines2[i][0];
                theta1 = lines2[i][1];

                total_rho1 += rho1;
                total_theta1 += theta1;
            }

            total_rho1 /= lines2.size();
            total_theta1 /= lines2.size();

            a1 = cos(total_theta1);
            b1 = sin(total_theta1);
            x1 = a1 * total_rho1;
            y1 = b1 * total_rho1;
            p3 = Point(cvRound(x1 + 1000 * (-b1)) + rect_right.x, cvRound(y1 + 1000 * a1) + rect_right.y);
            p4 = Point(cvRound(x1 - 1000 * (-b1)) + rect_right.x, cvRound(y1 - 1000 * a1) + rect_right.y);
            //line(frame, p3, p4, Scalar(0, 0, 255), 3, 8);
        }

        HoughLines(centerFrame, lines3, 1, CV_PI / 180, 47, 0, 0);
        if (lines3.size()) {
            total_rho2 = 0;
            total_theta2 = 0;

            for (int i = 0; i < lines3.size(); i++) {
                rho2 = lines3[i][0];
                theta2 = lines3[i][1];
                
                total_rho2 += rho2;
                total_theta2 += theta2;
            }

                a2 = cos(theta2);
                b2 = sin(theta2);
                x2 = a2 * rho2;
                y2 = b2 * rho2;
                p5 = Point(cvRound(x2 + 1000 * (-b2)) + rect_center.x, cvRound(y2 + 1000 * a2) + rect_center.y);
                p6 = Point(cvRound(x2 - 1000 * (-b2)) + rect_center.x, cvRound(y2 - 1000 * a2) + rect_center.y);
                //line(frame, p5, p6, Scalar(255, 255, 0), 3, 8);
                putText(frame, "Lane departure!", Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }

        bg_model->apply(roiFrame, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), BORDER_DEFAULT);
        threshold(foregroundMask, foregroundMask, 100, 255, THRESH_BINARY);
        foregroundImg = Scalar::all(0);
        roiFrame.copyTo(foregroundImg, foregroundMask);

        //NonZeroCount 가 1 이상이면 다른 영상으로 간주
        int nNonZeroCount = countNonZero(foregroundMask);

        if (nNonZeroCount > 15000) {
            putText(frame, "Start Moving!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }

        bg_model->getBackgroundImage(backgroundImg);

        //rectangle(frame, rect, Scalar(127, 127, 127), 2);
        //rectangle(frame, rect_left, Scalar(255, 0, 0), 2);
        //rectangle(frame, rect_right, Scalar(0, 255, 0), 2);
        //rectangle(frame, rect_center, Scalar(0, 255, 255), 2);

        imshow("Project2", frame);

    }

    return 0;
}
