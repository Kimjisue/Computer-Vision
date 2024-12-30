#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

//Moon   
Mat moon, moon_image_ri, moon_image_ri_blur, unsharp, moon_sharping, moon_filtered; 
moon = imread("Moon.png",0);
//원본영상 
//moon = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/moon.png",0);
moon_image_ri = moon(Rect(moon.cols / 2, 0, moon.cols / 2, moon.rows));

/*
1. 원본영상 블러처리
2. 원본 - 블러링 영상 => unsharp
3. 원본 + unsharp 
*/
GaussianBlur(moon_image_ri, moon_image_ri_blur, Size(3, 3), 0, 0, BORDER_DEFAULT); 
unsharp = moon_image_ri - moon_image_ri_blur;
moon_sharping = moon_image_ri + unsharp;
moon_filtered = moon.clone();
moon_sharping.copyTo(moon_filtered(Rect(moon.cols/2,0,moon.cols/2,moon.rows)));

imshow("moon", moon); 
imshow("moon_filtered", moon_filtered);


//saltnpepper
Mat saltnpepper_image = imread("saltnpepper.png", 0);
//Mat saltnpepper_image = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/saltnpepper.png", 0);
imshow("saltnpepper", saltnpepper_image);
//Mat saltnpepper_image_ri = saltnpepper_image(Rect(saltnpepper_image.cols / 2, 0, saltnpepper_image.cols / 2, saltnpepper_image.rows));
Mat saltnpepper_image_le = saltnpepper_image(Rect(0, 0, saltnpepper_image.cols / 2, saltnpepper_image.rows));
Mat saltnpepper_filtered = saltnpepper_image.clone();

Mat mf;
// Blurs an image using the median filter
// image: src, mf1: dst, 3: aperture size(must be odd and greater than 1) 
medianBlur(saltnpepper_image_le, mf, 9);
//imshow("MedianFiltered1", mf);

mf.copyTo(saltnpepper_filtered(Rect(0, 0, mf.cols, mf.rows)));
imshow("Saltnpepper_filtered", saltnpepper_filtered);
waitKey(0);

return 0;

}
