// #include <opencv2/cv.h>
// #include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Open webcam
    VideoCapture cap(0);

    int successes = 0;
    int numBoards = 70; // Total number of boards to capture
    int numCornersHor = 7; // Number of horizontal corners
    int numCornersVer = 7; // Number of vertical corners
    int Rect_size = 20; // Length of one side of the rectangle
    int numSquares = (numCornersHor - 1) * (numCornersVer - 1);
    Size board_sz = Size(numCornersHor, numCornersVer);

    // Containers for storing points
    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;
    vector<Point2f> corners;
    vector<Point3f> obj;

    for (int i = 0; i < numCornersHor; i++) {
        for (int j = 0; j < numCornersVer; j++) {
            obj.push_back(Point3f(i * Rect_size, j * Rect_size, 0.0f));
        }
    }

    Mat img, gray;
    cap >> img;
    cout << "Image size: " << img.size() << endl;

    while (successes < numBoards) {
        cap >> img;
        if (img.empty()) break; // End of video stream

        cvtColor(img, gray, COLOR_RGB2GRAY);

        // Stop capturing on ESC key press
        if (waitKey(1) == 27) break;

        // Find the positions of internal corners of the chessboard
        bool found = findChessboardCorners(
            gray, 
            board_sz, 
            corners, 
            CALIB_CB_ADAPTIVE_THRESH
        );

        if (found) {
            // Refine corner locations
            cornerSubPix(
                gray,
                corners,
                Size(11, 11),
                Size(-1, -1),
                TermCriteria(TermCriteria::EPS | TermCriteria::MAX_ITER, 30, 0.1)
            );

            // Render detected chessboard corners
            drawChessboardCorners(img, board_sz, corners, found);

            image_points.push_back(corners);
            object_points.push_back(obj);
            printf("Snap stored!\n");
            successes++;
        }

        imshow("win1", img);
        imshow("win2", gray);
        waitKey(1000); // Wait 1 second
    }

    cout << "Complete!" << "\n";

    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    // Perform camera calibration
    calibrateCamera(
        object_points, 
        image_points, 
        img.size(), 
        intrinsic, 
        distCoeffs, 
        rvecs, 
        tvecs
    );

    // Print intrinsic parameters
    cout << "==================Intrinsic Parameter=====================" << "\n";
    for (int i = 0; i < intrinsic.rows; i++) {
        for (int j = 0; j < intrinsic.cols; j++) {
            cout << intrinsic.at<double>(i, j) << "\t\t";
        }
        cout << endl;
    }
    cout << "====================================================" << "\n";

    cap.release();
    waitKey();
    return 0;
}