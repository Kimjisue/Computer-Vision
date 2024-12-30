#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat image;
    image = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/lena.png",IMREAD_COLOR);
    if(image.empty())
    {
        cout << "Could not open of find the image" << endl;
        return -1;
    }

    namedWindow("lena", WINDOW_AUTOSIZE);
    imshow("lena",image);

    waitKey(0);

}