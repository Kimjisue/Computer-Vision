#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

Mat combineImages(Mat firstImage, Mat secondImage) {
    if (firstImage.empty() || secondImage.empty()) {
        cout << "image empty!" << endl;
        return Mat();
    }

    Ptr<ORB> orbF = ORB::create(1000);


    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    orbF->detectAndCompute(firstImage, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(secondImage, noArray(), keypoints2, descriptors2);

    BFMatcher matcher(NORM_HAMMING);
    vector<vector<DMatch>> matches;
    matcher.knnMatch(descriptors1, descriptors2, matches, 2);

    float nndr = 0.7f;
    vector<DMatch> goodMatches;
    for (auto& match : matches) {
        if (match[0].distance < nndr * match[1].distance) {
            goodMatches.push_back(match[0]);
        }
    }

    vector<Point2f> akeypoints1, akeypoints2;
    for (const auto& match : goodMatches) {
        akeypoints1.push_back(keypoints1[match.queryIdx].pt);
        akeypoints2.push_back(keypoints2[match.trainIdx].pt);
    }

    Mat homography = findHomography(akeypoints2, akeypoints1, RANSAC);

    Mat result;
    warpPerspective(secondImage, result, homography, Size(firstImage.cols * 1.4, firstImage.rows * 1.2));

    for (int y = 0; y < firstImage.rows; y++) {
        for (int x = 0; x < firstImage.cols; x++) {
            if (result.at<Vec3b>(y, x) == Vec3b(0, 0, 0)) {
                result.at<Vec3b>(y, x) = firstImage.at<Vec3b>(y, x);
            }
        }
    }

    return result;
}

int main() {
    Mat panorama;
    vector<string> imageFiles = {
        "/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/panorama_image/pano1.JPG",
        "/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/panorama_image/pano2.JPG",
        "/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/panorama_image/pano3.JPG",
        "/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/panorama_image/pano4.JPG"
    };
    // vector<string> imageFiles = {
    //     "/panorama_image/pano1.JPG",
    //     "/panorama_image/pano2.JPG",
    //     "/panorama_image/pano3.JPG",
    //     "/panorama_image/pano4.JPG"
    // };

    for (const auto& filePath : imageFiles) {
        Mat currentImage = imread(filePath);
        if (panorama.empty()) {
            panorama = currentImage;
        } else {
            panorama = combineImages(panorama, currentImage);
        }
    }

    resize(panorama, panorama, Size(640, 480));

    imshow("panorama_img", panorama);
    waitKey(0);

    return 0;
}
