
#include <string>
#include <opencv2/dnn.hpp>
#include <iostream>
#include "yolo.h"
using namespace std;

int main()
{
    NetConfg config = {
        0.5f, 0.3f, 320, 320, "coco.names", "yolo-fastest-xl.onnx", "yolo-fastest-xl"};
    Yolo yolo_model(config);
    cout<<"Load Model"<<endl;
    VideoCapture cap;
    cap.open(0);
    Mat img;
    while (1)
    {
        cap >> img;
        yolo_model.setcapSize(img.cols,img.rows);
        yolo_model.detect(img);
        imshow("result", img);
        waitKey(10);
    }
    destroyAllWindows();
    return 0;
}
