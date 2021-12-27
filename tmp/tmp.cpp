#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
using namespace cv;

int main()
{
    Mat img = imread("data/j.png");        
    Mat img_gray;
    Mat board = Mat::ones(img.size(), CV_8U)*255;
    cvtColor(board, board, COLOR_GRAY2BGR);
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    
    GaussianBlur(img_gray, img_gray, Size(5, 5), 0);
    threshold(img_gray, img_gray, 90, 255, THRESH_BINARY_INV);
    imshow("Th", img_gray);
    //Mat ker = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    //erode(img_gray, img_gray, ker, Point(-1, -1), 2);
    imshow("Erode", img_gray);
    Mat edges;
    std::cout<<img.size()<<std::endl;
    std::cout<<img.rows<<","<<img.cols<<std::endl;
    Canny(img_gray,edges, 20,20*3,3,true);
    std::vector<std::vector<Point>> counters;
    std::vector<Vec4i> hierarchy;
    findContours(edges, counters,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
    std::cout<<"find counters "<<counters.size()<<std::endl;
    for (int i = 0; i < counters.size(); ++i)
        drawContours(board, counters,i , Scalar(0, 255, 0));
    std::cout<<img.size()<<std::endl;
    std::cout<<img.rows<<","<<img.cols<<std::endl;
    imshow("Cunters", board);
    
    std::vector<std::vector<Point>> counters_poly(counters.size());
    for (int i = 0; i < counters.size(); i++){
        if(hierarchy[i][2]==-1){
            std::cout<<i<<std::endl;
            approxPolyDP(counters[i], counters_poly[i], 100, true);        
            drawContours(board, counters_poly, i, Scalar(255, i*30, i*10), 2*i+1, 8);
        }
    }
    imshow("appprox",board);
    waitKey(0);
    return 0;
}
