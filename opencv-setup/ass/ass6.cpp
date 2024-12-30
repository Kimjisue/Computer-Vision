#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{   
    Mat query, image, descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector<vector<DMatch>> matches;
    vector<DMatch> goodMatches, best_GoodMatches; 
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    vector<String> str; 

    string best_image, DB_Image;
    int size = 0;
    int prev_size = -1;
    float nndr = 0.6f;
    int k = 2; 
    string queryImage;
    string path("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/Assignment6_Datasets/DBs/*.jpg");
    //string path("Assignment6_Datasets/DBs/*.jpg");

    cout << "Enter query image name: ";
    cin >> queryImage;

    query = imread("/Users/kimjisue/Desktop/opencv/opencv-setup/dataset/Assignment6_Datasets/query_image/" + queryImage);
    //query = imread("Assignment6_Datasets/query_image/" + queryImage);
    if (query.empty()) {
        cout << "No file!" << endl;
        return 0;
    }
    resize(query, query, Size(640, 480)); 

    glob(path, str, false);
    cout << "Sample Image Load Size : " << str.size() << endl;
    if (str.empty()) {
        cout << "No file!\n" << endl;
        return 0;
    }

    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);

    for (int i = 0; i < str.size(); i++) 
    {   
        DB_Image = str[i];
        image = imread(DB_Image); 
        resize(image, image, Size(640, 480));

        if (image.empty()) {
            cout << "image empty!" << endl;
            return -1;
        }

        orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        for (int i = 0; i < matches.size(); i++) {
            if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
                goodMatches.push_back(matches[i][0]);
            }
        }

        cout << "Image number " << i + 1 << " Matching: " << goodMatches.size() << endl;
        size = goodMatches.size();

        if (size > prev_size) {
            prev_size = size;
            best_image = DB_Image;
            best_GoodMatches = goodMatches; 
        }

        matches.clear();
        goodMatches.clear();
    }


    image = imread(best_image);
    resize(image, image, Size(640, 480));
    orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

    cout << "Match found! " << endl;
    drawMatches(query, keypoints1, image, keypoints2, best_GoodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    imshow("Query", query);
    imshow("Best_matching", imgMatches);
    waitKey(0); 

    return 0;
}