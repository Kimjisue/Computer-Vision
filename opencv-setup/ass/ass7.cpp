#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat frame, grayframe;
    VideoCapture cap;
    vector<Rect> faces;
    Rect face;
    char curkey = 'r';  
    char key;
    bool t_key = false; 
    Mat bluebackground;

    //if (cap.open("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/10_week_dataset/Faces.mp4") == 0){
    if (cap.open("Faces.mp4") == 0) {
        cout << "No such file!" << endl;
        return -1;
    }

    CascadeClassifier face_classifier;
    //face_classifier.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml");
    face_classifier.load("haarcascade_frontalface_alt.xml");

    double fps = cap.get(CAP_PROP_FPS);
    int delay = 1000 / fps;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cout << "End of video" << endl;
            break;
        }

        cvtColor(frame, grayframe, COLOR_BGR2GRAY);
        key = waitKey(delay);

        if (curkey == 'n') {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(80, 80), Size(90, 90));
            if (!faces.empty()) {
                face = faces[0];
                rectangle(frame, face, Scalar(0, 255, 0), 3);
                putText(frame, "n", Point(face.x + 60,face.y + 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }

        } else if (curkey == 'm') {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(54, 54), Size(54, 54));
            if (!faces.empty()) {
                face = faces[0];
                rectangle(frame, face, Scalar(0, 255, 0), 3);
                putText(frame, "m", Point(face.x + 20, face.y + 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }

        } else if (curkey == 'f') {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(20, 20), Size(35, 35));
            if (!faces.empty()) {
                face = faces[0];
                rectangle(frame, face, Scalar(0, 255, 0), 3);
                putText(frame, "f", Point(face.x + 20, face.y + 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }
        }
        else if (curkey == 'r') {
            faces.clear(); 
            face = Rect(); 
            t_key = false; 
            destroyWindow("tracking");
        }

        if (t_key && !face.empty()) {
            bluebackground = Mat(frame.size(), CV_8UC3, Scalar(255, 0, 0));
            frame(face).copyTo(bluebackground(face));
            imshow("tracking", bluebackground);
        }

        if (key == 27) { 
            break;
        } else if (key == 't') {
            if (face.area() > 0) { 
                if (!t_key) {
                    t_key = true;
                } else {
                    t_key = false;
                    destroyWindow("tracking"); 
                }
            } else {
                putText(frame, "Detect before tracking", Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            }
        } else if (key == 'n' || key == 'm' || key == 'f' || key=='r') {
            curkey = key; 
        }
        imshow("Faces", frame);
    }

    return 0;
}
