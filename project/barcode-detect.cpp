#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
int main(int argc, char **argv)
{
    Mat img = imread("img/bar.jpg");
    Mat gray, th, edges;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    std::vector<std::vector<Point>> counters;
    std::vector<Vec4i> hierarchy;
    findContours(gray, counters,hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for(int i = 0; i < counters.size(); i++){
        if(hierarchy[i][3]!=-1){
            drawContours(img, counters, i, Scalar(0, 255, 0), 2);
        }
    }
    imshow("img", img);
    waitKey(0);
    return 0;
}
