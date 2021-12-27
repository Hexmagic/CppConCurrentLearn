#include <fstream>
#include <iostream>
#include <sstream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/dnn/layer.details.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

struct NetConfg{
    float configThreshold;// 置信度阈值
    float nmsThreshold; //NMS 阈值
    int inWidth; //输入宽
    int inHeight; //输入高
    string classesFile; // 类别文件名
    string weightFile; // 权重文件
    string netName; //网络名
};

class Yolo{
public:
    Yolo(NetConfg netConf){
        this->configThreshold = netConf.configThreshold;
        this->nmsThreshold = netConf.nmsThreshold;
        this->inWidth = netConf.inWidth;
        this->inHeight = netConf.inHeight;
        this->netName = netConf.netName;
        ifstream ifs(netConf.classesFile.c_str());
        string line;
        while(getline(ifs,line))
            this->classes.push_back(line);
        cout<<"Start Load Model"<<endl;
        this->net = readNetFromONNX(netConf.weightFile);
        cout<<"Load model Done"<<endl;
        this->net.setPreferableBackend(DNN_BACKEND_OPENCV);
        this->net.setPreferableTarget(DNN_TARGET_CPU);
    }
    void detect(Mat &frame){
        Mat blob;
        blobFromImage(frame, blob, double(1 / 255.0), Size(this->inWidth, this->inHeight), Scalar(0, 0, 0), true, false);//归一 resize 均值 交换蓝绿通道 不进行采集
        this->net.setInput(blob);
        vector<String> names = this->net.getUnconnectedOutLayersNames();
        //输出层信息
        vector<Mat> outs_blob;
        vector<Mat> outs;
        //输出层的数据
        this->net.forward(outs_blob, names);
        for (int i = 0; i < outs_blob.size();i++){
            vector<Mat> out;
            imagesFromBlob(outs_blob[i], out);
            outs.push_back(out[0]);
        }
        this->postProcess(frame,outs);
    }
    void setcapSize(int width, int height){
        this->capWidth = width;
        this->capHeight = height;
        this->scaleHeight = float(this->capHeight)/float(this->inHeight);
        this->scaleWidth = float(this->capWidth) / float(this->inWidth);
    }

private:
    float configThreshold;
    float nmsThreshold;
    int inWidth, inHeight;
    int capWidth, capHeight;
    float scaleHeight, scaleWidth;
    string netName;
    vector<string> classes;
    Net net;
    void postProcess(Mat &frame, const vector<Mat> &outs){
        vector<int> classIds;
        vector<float> confidences;
        vector<float> scores;
        vector<Rect> boxes;
        for(size_t i = 0; i < outs.size();i++){
            float *data = (float*)outs[i].data;
            for (int j = 0;j<outs[i].rows;j++,data+=outs[i].cols){
                Mat score = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double max_score;
                minMaxLoc(score, 0, &max_score,0, &classIdPoint);
                if(data[4]>this->configThreshold){
                    int centerX = (int)(data[0] * this->scaleWidth);
                    int centerY = (int)(data[1]*this->scaleHeight);
                    int width = (int)(data[2]*this->scaleWidth);
                    int height = (int)(data[3]*this->scaleHeight);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(data[classIdPoint.x]+5);
                    scores.push_back(max_score*data[4]);
                    boxes.push_back(Rect(left,top,width,height));
                }
            }
        }
        vector<int> indices;
        NMSBoxes(boxes,scores,this->configThreshold,this->nmsThreshold,indices);
        for (size_t i = 0; i < indices.size(); ++i) {
    		int idx = indices[i];
    		Rect box = boxes[idx];
    		this->drawPred(classIds[idx], confidences[idx], box.x, box.y,
    			box.x + box.width, box.y + box.height, frame);
    	}
    }
    void drawPred(int classId,float conf,int left,int top,int right,int bottom,Mat&frame){
        rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 0, 255), 3);

    	//Get the label for the class name and its confidence
    	string label = format("%.2f", conf);
    	if (!this->classes.empty()) {
    		CV_Assert(classId < (int)this->classes.size());
    		label = this->classes[classId] + ":" + label;
    	}

    	//Display the label at the top of the bounding box
    	int baseLine;
    	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    	top = max(top, labelSize.height);
    	// rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
    	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 255, 0), 2);
       }
};

