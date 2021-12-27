#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
int main()
{
    /*
    Canny 原理:
    1. 高斯降噪
    2. 依照Sobel算子计算梯度Gx Gy和方向 delta=arctan(Gy/Gx),
    3. 非极大值抑制，排除非边缘
    4. 滞后阈值 min max：
        1> 大于max保留
        2> 小于min移除
        3>  大于min小于max 且连接到max保留
    > 1/3 <max/min <1/2

    Canny 边缘检测算法 是 John F. Canny 于 1986年开发出来的一个多级边缘检测算法，也被很多人认为是边缘检测的 最优算法, 最优边缘检测的三个主要评价标准是:
        低错误率: 标识出尽可能多的实际边缘，同时尽可能的减少噪声产生的误报。
        高定位性: 标识出的边缘要与图像中的实际边缘尽可能接近。
        最小响应: 图像中的边缘只能标识一次。
    */    
    Mat lena = imread("img/lena.jpg");
    Mat blured_lena;
    GaussianBlur(lena, blured_lena, Size(3, 3), 0);
    Mat out;
    Sobel(blured_lena, out, -1, 1, 1);
    imshow("Sobel", out);
    Laplacian(blured_lena, out, -1, 3);
    imshow("Laplacian", out);
    Canny(blured_lena, out, 40,120, 3, true);
    imshow("Canny", out);
    waitKey(0);
}
