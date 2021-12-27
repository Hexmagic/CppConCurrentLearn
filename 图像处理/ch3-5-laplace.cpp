#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;

int main()
{
    /*
    Laplace 算子原理: Sobel算子基于边缘部分像素值跳跃，导数较大，Laplace算子基于二阶导数为0

    Laplace算子属于二阶微分线性算子，在图像边缘处理中二阶微分定位能力更强，但是对噪声比较敏感，具有各自向性，能对任何方向的边界线条进行锐化。    
    */
    Mat lena = imread("data/lena.jpg");
    Mat blured_lena;
    GaussianBlur(lena, blured_lena, Size(3, 3), 0);
    Mat out;
    Sobel(blured_lena, out, -1, 1, 1);
    imshow("Sobel", out);
    Laplacian(blured_lena, out, -1, 3);
    imshow("Laplacian", out);
    waitKey(0);
}
