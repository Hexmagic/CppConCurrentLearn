#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
int main()
{
    /*
    Sobel原理在与边缘处像素值的显著变化，表明这一变化的方法是使用导数，导数越大表明变化越明显；
    
    Sobel算子检测效果一般般，断断续续。 它是一阶离散性算子，Sobel算子没有讲图像的主题
    和背景严格区分。即Sobel算子没有基于图像灰度进行处理。 因为Sobel算子没有模拟人的视觉生理特征，所以提取的轮廓有时不能让人满意
    */
    Mat lena = imread("img/lena.jpg");
    Mat blured_lena;
    GaussianBlur(lena, blured_lena, Size(3, 3), 0);
    Mat soble_out;
    Sobel(blured_lena, soble_out, -1, 1, 1 );
    imshow("Sobel",soble_out);
    Sobel(blured_lena, soble_out, -1, 0, 1 );
    imshow("SobelY",soble_out);
    Sobel(blured_lena, soble_out, -1, 1, 0 );
    imshow("SobelX",soble_out);
    waitKey(0);
}
