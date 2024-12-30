#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
    Mat image;
    Mat image_Ycbcr;
    Mat Ycbcr_channels[3];
    Mat y(512, 512, CV_8UC1);

    Mat quantization_mat1 = (Mat_<double>(8, 8) <<
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
    )
    Mat quantization_mat2 = (Mat_<double>(8, 8) <<
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1
    );
    Mat quantization_mat3 = (Mat_<double>(8, 8) <<
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100
    );

    image = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/1_week_dataset/lena.png", IMREAD_COLOR);
    cvtColor(image, image_Ycbcr, COLOR_BGR2YCrCb);
    split(image_Ycbcr, Ycbcr_channels);

    for (int j = 0; j < 512; j++){
        for (int i = 0; i < 512; i++)
        {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j,i);
        }
    }
    y.convertTo(y, CV_32F);


    imshow("Ychannel", y);
    waitKey(0);
    return 0;
}