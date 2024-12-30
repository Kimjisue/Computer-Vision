#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture cap;
    int delay;

    //Read a video "background.mp4"
    //if (cap.open("/Users/kimjisue/Desktop/opencv/opencv-setup/background.mp4") == 0){
    if (cap.open("background.mp4") == 0){
        cout << "no such file!" << endl;
        waitKey(0);
    }
    
    double fps = cap.get(CAP_PROP_FPS);
    double time_in_msec = 0; 
    int curr_frame = 0;
    int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
    delay = 1000/fps;

    // Displau video for the first 3 seconds
    while (time_in_msec < 3000) {
        cap >> frame;
        if(frame.empty()){
            cout << "end of video" << endl;
            break;
        }

        time_in_msec = cap.get(CAP_PROP_POS_MSEC);
        curr_frame = cap.get(CAP_PROP_POS_FRAMES);
        // current frames/total frames
        cout << "frames:" << curr_frame <<" / "<< total_frames << endl;

        imshow("video", frame);
        waitKey(delay);
    }
    waitKey(0);
    return 0;
}