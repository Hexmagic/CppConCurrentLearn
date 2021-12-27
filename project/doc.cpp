#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

float distance(Point2f&p1,Point2f&p2){
    float x = p1.x - p2.x;
    float y = p1.y - p2.y;
    return sqrt(x*x+y*y);
}
float get_area(std::vector<Point>&points){
    RotatedRect rect = minAreaRect(points);
    Point2f P[4];
    rect.points(P);    
    return distance(P[0],P[1]);
}

bool cmp(std::vector<Point>&c1,std::vector<Point>&c2){
    return get_area(c1)>get_area(c2);
}

bool cmpPoint(Point2f p1,Point2f p2){
    if(p1.x<p2.x)
        return true;
    else if(p1.x==p2.x){
        return p1.y<p2.y;
    }
    return false;
}

int main(){
    Mat img = imread("img/doc.jpg");
    Mat th,gray;
    cvtColor(img,gray,COLOR_BGR2GRAY);
    threshold(gray,th,150,255,THRESH_BINARY_INV);
    Mat ker = getStructuringElement(MORPH_RECT,Size(7,7));
    morphologyEx(th,th,MORPH_CLOSE,ker);
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hier;
    findContours(th,contours,hier,MORPH_RECT,CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(),contours.end(),cmp);
    for(int i=0;i<1;i++){
        RotatedRect rect = minAreaRect(contours[i]);
        Point2f P[4];
        rect.points(P);
        for(int j=0;j<=3;j++){
            line(img,P[j],P[(j+1)%4],Scalar(0,0,255),2);
        }        
    }
    imshow("img",img);
    RotatedRect rect = minAreaRect(contours[0]);
    Point2f P[4];    
    rect.points(P);
    std::sort(P,P+4,cmpPoint);
    std::cout<<P[0]<<" "<<P[1]<<" "<<P[2]<<" "<<P[3]<<" "<<std::endl;
    float w = distance(P[0],P[1]);
    float h = min(distance(P[1],P[2]),distance(P[1],P[3]));
    std::cout<<"w:"<<w<<"h:"<<h<<std::endl;
    if(w<h){
        swap(w,h);
    }
    Mat empty = Mat::zeros(gray.size(),CV_8U)*255;
    int starth = gray.rows - h-20;
    Point2f dst[4] = {
        Point2f(10, starth+h),
        Point2f(10, starth),
        Point2f(w+10, starth+h),
        Point2f(w+10, starth)};
    Mat rot_mat = getAffineTransform(P,dst);
    warpAffine(gray,empty,rot_mat,Size(gray.cols,gray.rows),1,0,Scalar(255,255,255));
    imshow("doc", empty);
    Mat out;
    bilateralFilter(empty,out,31,30,31/2);
    imshow("out", out);
    imwrite("test.png", out);
    waitKey(0);
}
