#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams {
    Mat img;              
    vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void* param) {
    MouseParams* mp = (MouseParams*)param;
    if (event == EVENT_LBUTTONDOWN) {
        mp->out.push_back(Point2f(x, y));
    }

    if (event == EVENT_RBUTTONDOWN) {
        mp->out.clear();
    }
}

int main() {
    Mat t_frame, c_frame, result, mask;

    //VideoCapture timesquare("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/12_week_dataset/Timesquare.mp4");
    //VideoCapture contest("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/12_week_dataset/contest.mp4");
    VideoCapture timesquare("Timesquare.mp4");
    VideoCapture contest("contest.mp4");
    
    if (!timesquare.isOpened() || !contest.isOpened()) {
        cout << "no such file" << endl;
        return -1;
    }

    double fps = timesquare.get(CAP_PROP_FPS);
    int delay = 1000 / fps;

    MouseParams mp;

    contest >> c_frame;
    mp.in.push_back(Point2f(0, 0));
    mp.in.push_back(Point2f(c_frame.cols, 0));
    mp.in.push_back(Point2f(c_frame.cols, c_frame.rows));
    mp.in.push_back(Point2f(0, c_frame.rows));

    namedWindow("background");
    namedWindow("input");

    setMouseCallback("background", onMouse, (void*)&mp);

    while (true) {
        timesquare >> t_frame;
        contest >> c_frame;

        if (t_frame.empty() || c_frame.empty()) {
            cout << "End of video" << endl;
            break;
        }

        mp.img = t_frame.clone();

        if (mp.out.size() == 4) { 
            Point trapezoid[1][4];
            trapezoid[0][0] = mp.out[0];
            trapezoid[0][1] = mp.out[1];
            trapezoid[0][2] = mp.out[2];
            trapezoid[0][3] = mp.out[3];

            const Point* ppt[1] = { trapezoid[0] };
            int npt[] = { 4 };

            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
            warpPerspective(c_frame, result, homo_mat, t_frame.size());

            mask = Mat::zeros(t_frame.size(), CV_8U);
            fillPoly(mask, ppt, npt, 1, Scalar(255), 8);

            result.copyTo(t_frame, mask);

            imshow("background", t_frame);
        } else {

            Mat tmp = t_frame.clone();
            for (size_t i = 0; i < mp.out.size(); i++) {
                circle(tmp, mp.out[i], 5, Scalar(0, 0, 255), 5);
            }
            imshow("background", tmp);
        }

        imshow("input", c_frame);

        if (waitKey(delay) == 27) break; 
    }

    return 0;
}