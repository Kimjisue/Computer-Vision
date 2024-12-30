#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("lena.png", 0); 
    //Mat image = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/lena.png", 0); 
    Mat negative_img = image.clone(); 
    Mat result = negative_img.clone(); 
   
    for (int j = 0; j < image.rows; j++){
        for (int i = 0; i < image.cols; i++){
            uchar value = image.at<uchar>(j, i); // 픽셀값 계산
			if (value < 127)
			{
				negative_img.at<uchar>(j, i) = 255 - value; // negative transformation
			}
			else
			{
				negative_img.at<uchar>(j, i) = pow((float)(value / 255.0), 10) * 255; // gamma transformation (gamma = 10)
			}
        }
    }
    for(int i=0; i<negative_img.rows; i++) {
        for(int j=0; j<negative_img.cols; j++) {
            result.at<uchar>(i, negative_img.rows - 1 - j) = negative_img.at<uchar>(j, i);
        }
    }
    imshow("gray_image", image);
    imshow("result", result);

    waitKey(0);
    return 0; 
}