#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int erosion_size = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;
Mat j;
void Dilate(int, void *)
{
    Mat dst;
    Mat ker = getStructuringElement(MORPH_RECT, Size(dilation_size*2+1, dilation_size*2+1 ));
    dilate(j, dst,ker);
    imshow("Dilate", dst);
}

void Erode(int, void *)
{
    Mat dst;
    Mat ker = getStructuringElement(MORPH_RECT, Size(erosion_size*2+1, erosion_size*2+1 ));
    erode(j, dst,ker);
    imshow("Erode", dst);
}
int main()
{
   j = imread("img/j.png");
    //形态学的操作有膨胀和腐蚀，他们的运用有消除噪声，分割独立
    //元素以及连接相邻的元素。寻找极大极小值
    namedWindow( "Erode", WINDOW_AUTOSIZE );
    namedWindow( "Dilate", WINDOW_AUTOSIZE );   
    // 膨胀会用内核B覆盖区域的最大像素值替换锚点位置的值
    createTrackbar("EKernel Size","Erode", &erosion_size, max_kernel_size,Erode);
    createTrackbar("DKernel Size","Dilate", &dilation_size, max_kernel_size,Dilate);
    Dilate(0, 0);
    Erode(0, 0);
    waitKey(0);
    std::cout <<"end"<<std::endl;
    return 0;
}
