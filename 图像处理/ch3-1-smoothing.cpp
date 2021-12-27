#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void show(String win_name ,Mat &m){
    imshow(win_name, m);
    waitKey(0);
}

int main(){
    Mat lena = imread("img/lena.jpg");
    Mat dst;
    // 均值模糊，输出是核窗口内像素的均值
    
    blur(lena, dst,Size(5,5));
    show("blur",dst);
    // 高斯滤波，距离中心越近权重越大
    // x y 方向标准差， 如果是0，则由核大小确定
    GaussianBlur(lena, dst,Size(5,5),0,0);
    show("GaussianBlur",dst);
    // 取邻域的像素的中值，核大小
    medianBlur(lena, dst, 5);
    show("medianBlur",dst);
    // 前面的模糊削弱了噪声的同时削弱了边，双边滤波和高斯滤波类似，距离越近权重越大，另外差值越大权重越大，,邻域直径，颜色标准差，坐标空间标准差
    bilateralFilter(lena, dst, 31, 40,31/2);
    show("bilateralFilter",dst);
}
