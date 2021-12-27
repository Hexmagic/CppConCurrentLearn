#include <iostream>
#include <opencv2/opencv.hpp>



using namespace cv;
int main()
{
    Mat one = Mat::ones(Size(3, 3), CV_8U);
    std::cout << (int)one.at<Vec3b>(3,3)[0]<<std::endl;
    return 0;
}
