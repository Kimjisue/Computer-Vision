#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


struct MouseParams{
    Mat img;
    vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void* param){
    MouseParams* mp = (MouseParams*)param;
    Mat img = mp->img;
    if (event == EVENT_LBUTTONDOWN){ // left button
        Mat result;
        //Insert position from LT. Direction is clock-wise
        mp->in.push_back(Point2f(x, y));
        if (mp->in.size() == 4)
        {
        // Calculate perspective transform matrix(=homo_mat) from 4 matching pairs of points
        Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);
        // apply perspective transformation to img using homo_mat
        // result will have the same size of Size(300, 300) and the same type of img
        warpPerspective(img, result, homo_mat, Size(300, 300));
        imshow("output", result);
    }
    else{
        result = img.clone();
        for (size_t i = 0; i < mp->in.size(); i++){
            circle(result, mp->in[i], 3, Scalar(0, 0, 255), 5);
        }
        imshow("input", result);
    }
}
    //Reset positions
    if (event == EVENT_RBUTTONDOWN){
        mp->in.clear();
        imshow("input", img);
    }
}

int main()
{
    Mat imput = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/12_week_dataset/book.jpg");
    imshow("input", imput);
    MouseParams mp;
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(300, 0));
    mp.out.push_back(Point2f(300, 300));
    mp.out.push_back(Point2f(0, 300));
    mp.img = imput;
    setMouseCallback("input", onMouse, (void*)&mp);
    waitKey();
    return 0;
}
