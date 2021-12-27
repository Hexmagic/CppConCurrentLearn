#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;

int main()
{
    Mat img = imread("data/harris.jpg");
    Mat gray;
    Mat dst_norm, dst_norm_scaled;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    
    Mat dst = Mat::zeros(gray.size(), CV_32FC1);
    cornerHarris( gray, dst, 2, 3, 0.04, BORDER_DEFAULT );
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX,CV_32FC1,Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    int thresh = 200;
    imshow("img", img);
    for (int i = 0; i < dst_norm.rows;i++)
    {
        for(int j = 0;j<dst_norm.cols;j++){

            if( (int) dst_norm.at<float>(j,i) > thresh )
              {
               circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0,255,0), 2, 8, 0 );
              }
        }
    }
    imshow("corners", img);
    waitKey(0);
    return 0;
}
