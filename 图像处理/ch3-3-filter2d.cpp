#include <opencv2/imgproc.hpp>

#include <opencv2/opencv.hpp>
using namespace cv;

int main(){
    /*
    假如你想得到图像的某个特定位置的卷积值，可用下列方法计算：
        将核的锚点放在该特定位置的像素上，同时，核内的其他值与该像素邻域的各像素重合；
        将核内各值与相应像素值相乘，并将乘积相加；
        将所得结果放到与锚点对应的像素上；
        对图像所有像素重复上述过程。
    */
    Mat img = imread("img/apple.jpg");
    
    Mat ker = Mat::ones(Size(3, 5), CV_32F)/9;
    Mat dst;
    filter2D(img, dst, -1, ker);
    imshow("Filter2D", dst);
    waitKey(0);
}
