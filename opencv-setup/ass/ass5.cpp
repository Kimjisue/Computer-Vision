#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    //Develop a program which performs various kinds of thresholding
    Mat finger_print,adaptive_1,adaptive;
    // finger_print = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/6_week_dataset/finger_print.png",0);
    // adaptive_1 = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/6_week_dataset/adaptive_1.jpg",0);
    // adaptive = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/6_week_dataset/adaptive.png",0);
    finger_print = imread("finger_print.png",0);
    adaptive_1 = imread("adaptive_1.jpg",0);
    adaptive = imread("adaptive.png",0);

    //For ‘finger_print.png’, set finger print region to 0 and background region to 255
    threshold(finger_print, finger_print, 0, 255, THRESH_BINARY | THRESH_OTSU);
    //For ‘adaptive_1.jpg’, and ‘adaptive.png’, set character region to 0 and background region to 255
    adaptiveThreshold(adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15);
    adaptiveThreshold(adaptive, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 15);


    imshow("finger_print", finger_print);
    imshow("adaptive_1", adaptive_1 );
    imshow("adaptive",adaptive);
    waitKey(0);

}
