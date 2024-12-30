
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double PSNR(Mat original, Mat frame) {
    double psnr = 0.0;
    double mse = 0.0;

    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            double diff = original.at<uchar>(i, j) - frame.at<uchar>(i, j);
            mse += diff * diff;
        }
    }
    mse /= 512*512;
    psnr = 20 * log10(255.0) - 10 * log10(mse);

    return psnr;
}

int main() {
    Mat image;
    Mat image_Ycbcr;
    Mat Ycbcr_channels[3];
    Mat y(512, 512, CV_8UC1);
    Mat qm1(512,512,CV_32FC1);
    Mat qm2(512,512,CV_32FC1);
    Mat qm3(512,512,CV_32FC1);
        
    Mat quantization_mat1 = (Mat_<double>(8, 8) <<
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
    );

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


    //image = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/1_week_dataset/lena.png", IMREAD_COLOR);
    image = imread("lena.png", IMREAD_COLOR);
    cvtColor(image, image_Ycbcr, COLOR_BGR2YCrCb);
    split(image_Ycbcr, Ycbcr_channels);

    for (int j = 0; j < 512; j++)
    {
        for (int i = 0; i < 512; i++)
        {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j, i);
        }
    }
    imshow("Original Y", y);

    y.convertTo(y, CV_32FC1);

    for (int i = 0; i < 512; i += 8) {
        for (int j = 0; j < 512; j += 8) {
            Rect block_rect(j, i, 8, 8);
            Mat block = y(block_rect);

            Mat dct_block;
            dct(block, dct_block);
            Mat block1 = dct_block.clone();     
            Mat block2 = dct_block.clone();
            Mat block3 = dct_block.clone();
            
            for (int x = 0; x < 8; ++x) {
                for (int y = 0; y < 8; ++y) {
                    block1.at<float>(x, y) = round(block1.at<float>(x, y) / quantization_mat1.at<double>(x, y)) * quantization_mat1.at<double>(x, y);
                }
            }

            for (int x = 0; x < 8; ++x) {
                for (int y = 0; y < 8; ++y) {
                    block2.at<float>(x, y) = round(block2.at<float>(x, y) / quantization_mat2.at<double>(x, y)) * quantization_mat2.at<double>(x, y);
                }
            }

            for (int x = 0; x < 8; ++x) {
                for (int y = 0; y < 8; ++y) {
                    block3.at<float>(x, y) = round(block3.at<float>(x, y) / quantization_mat3.at<double>(x, y)) * quantization_mat3.at<double>(x, y);
                }
            }

            dct(block1, block1, DCT_INVERSE);
            dct(block2, block2, DCT_INVERSE);
            dct(block3, block3, DCT_INVERSE);

            block1.copyTo(qm1(block_rect));
            block2.copyTo(qm2(block_rect));
            block3.copyTo(qm3(block_rect));
        }
    }
    qm1.convertTo(qm1, CV_8UC1);
    qm2.convertTo(qm2, CV_8UC1);
    qm3.convertTo(qm3, CV_8UC1);

    Ycbcr_channels[0].convertTo(Ycbcr_channels[0], CV_8UC1);

    double psnr1 = PSNR(Ycbcr_channels[0], qm1);
    double psnr2 = PSNR(Ycbcr_channels[0], qm2);
    double psnr3 = PSNR(Ycbcr_channels[0], qm3);

    cout << "QM1: psnr = " << psnr1 << endl;
    cout << "QM2: psnr = " << psnr2 << endl;
    cout << "QM3: psnr = " << psnr3 << endl;
    imshow("QM1", qm1);
    imshow("QM2", qm2);
    imshow("QM3", qm3);

    waitKey(0);
    return 0;
}
