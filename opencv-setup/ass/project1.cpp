#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//Negative transformation on the user input ‘n’
Mat negativeTransformation(Mat frame) {
    vector<Mat> channels(3);
    uchar* h;

    cvtColor(frame, frame, COLOR_BGR2HSV);
    split(frame, channels);
    for (int j = 0; j < frame.rows; j++) {
        h = channels[2].ptr<uchar>(j);
        for (int i = 0; i < frame.cols; i++)
            h[i] = 255 - h[i];
    }
    merge(channels, frame);
    cvtColor(frame, frame, COLOR_HSV2BGR);
    return frame;
}

//Gamma transformation with the value of gamma as 2.5 on the user input ‘g’ 
Mat gammaTransformation(Mat frame){
    vector<Mat> channels(3);
    uchar* v;
    float gamma = 2.5;
    unsigned char pix[256] = { 0 };
    cvtColor(frame, frame, COLOR_BGR2HSV);
    split(frame, channels);
    for (int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
    }
        for (int j = 0; j < frame.rows; j++) {
            v = channels[2].ptr<uchar>(j);
            for (int i = 0; i < frame.cols; i++) {
                v[i] = pix[v[i]];
            }
        }
    merge(channels, frame);
    cvtColor(frame, frame, COLOR_HSV2BGR);
    return frame;
}
//Histogram equalization on the user input ‘h’
Mat histogramEqualization(Mat frame){
    vector<Mat> channels(3);
    uchar* v;
    cvtColor(frame, frame, COLOR_BGR2HSV);
    split(frame, channels);
    equalizeHist(channels[2], channels[2]);
    merge(channels, frame);
    cvtColor(frame, frame, COLOR_HSV2BGR);
    return frame;
} 

//Color slicing on the user input ‘s’
//Hue value: 9<hue<23
Mat colorSlicing(Mat frame){
    vector<Mat> mo(3);
    uchar* h;
    uchar* s;
    cvtColor(frame, frame, COLOR_BGR2HSV);
    split(frame, mo);
    for (int j = 0; j < frame.rows; j++) {
        h = mo[0].ptr<uchar>(j); 
        s = mo[1].ptr<uchar>(j);
        for (int i = 0; i < frame.cols; i++) {
            if (h[i] > 9 && h[i] < 23) s[i] = s[i];
            else s[i] = 0; 
        }
    }
    merge(mo, frame);
    cvtColor(frame, frame, COLOR_HSV2BGR);
    return frame;
}
//Color conversion on the user input ‘c’
//Increase Hue value by 50
//For hue values bigger than 129, subtract 129 instead 
Mat colorConversion(Mat frame){
    vector<Mat> cc(3);
    uchar* h;
    cvtColor(frame, frame, COLOR_BGR2HSV);
    split(frame, cc);
    for (int j = 0; j < frame.rows; j++) {
        h = cc[0].ptr<uchar>(j);
        for (int i = 0; i < frame.cols; i++) {
            if (h[i] > 129) h[i] = h[i] - 129; 
            else h[i] += 50;
        }
    }
    merge(cc, frame);
    cvtColor(frame, frame, COLOR_HSV2BGR);
    return frame;
}

//Average filtering on the user input ‘a’
//Use “blur” function with mask size as 9X9
Mat averageFiltering(Mat frame){
    blur(frame, frame, Size(9, 9));
    return frame;
}

//Sharpening by unsharp masking on the user input ‘u’
//Use “blur” function with mask size as 9X9
Mat sharpening(Mat frame){
    Mat blured;
    blur(frame, blured, Size(9, 9));
    Mat unsharp = frame - blured;
    frame = frame + unsharp;
    return frame;
}

Mat white_balacing(Mat frame){
    Mat bgr_channels[3];
    split(frame,bgr_channels);

    double avg;
    int sum,temp,i,j,c;

    for(c = 0; c < frame.channels(); c++){
        sum = 0;
        avg = 0.0f;
            for(i=0;i<frame.rows;i++){
                for(j=0;j<frame.cols;j++){
                    sum += bgr_channels[c].at<uchar>(i,j);
                }
            }
        avg = sum / (frame.rows * frame.cols);
            for(i=0;i<frame.rows; i++){
                for(j=0;j<frame.cols;j++){
                    temp = (128/avg) * bgr_channels[c].at<uchar>(i,j);
                    if(temp > 255) bgr_channels[c].at<uchar>(i,j) = 255;
                    else bgr_channels[c].at<uchar>(i,j) = temp;
                }
            }
        }
        merge(bgr_channels, 3, frame);
        return frame;
}

Mat reset(Mat frame) {
    return frame;
}

int main()
{
    Mat frame,result;
    int fps;
    int delay;
    char curkey = 'r';
    char key;

    VideoCapture cap;
    // check if file exists. if none program ends
    //if (cap.open("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/video.mp4") == 0){
    if (cap.open("video.mp4") == 0){
        cout << "no such file!" << endl;
        return -1;
    }

    fps = cap.get(CAP_PROP_FPS);
    delay = 1000 / fps;
    
    while (1) {
        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }

        switch (curkey) {
        case 'n': 
            result = negativeTransformation(frame);
            break;
        case 'g': 
            result = gammaTransformation(frame);
            break;
        case 'h': 
            result = histogramEqualization(frame);
            break;
        case 's':  
            result = colorSlicing(frame);
            break;
        case 'c': 
            result = colorConversion(frame);
            break;
        case 'a':  
            result = averageFiltering(frame);
            break;
        case 'u':  
            result = sharpening(frame);
            break;
        case 'w': 
            result = white_balacing(frame);
            break;
        case 'r':
            result = reset(frame);
            break;
        default:
            break;
        }

    // Esc->27 
    if (key == 27) {
            break;
    }
    // else if(key == -1) {
    //     break;
        
    // }
    else if(key != -1) {
        curkey = key;
    
    }

    key = waitKey(delay);  

    imshow("video", result);
    }

}


