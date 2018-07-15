#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    //红色在HSV空间中有两个分布区间
    int R_LowH1 = 0;     //red1 in HSV
    int R_HighH1 = 10;

    int R_LowS1 = 43;
    int R_HighS1 = 255;

    int R_LowV1 = 46;
    int R_HighV1 = 255;

    int R_LowH2 = 156;    //red2 in HSV
    int R_HighH2 = 180;

    int R_LowS2 = 43;
    int R_HighS2 = 255;

    int R_LowV2 = 46;
    int R_HighV2 = 255;

    int B_LowH = 100;       //blue in HSV
    int B_HighH = 124;

    int B_LowS = 43;
    int B_HighS = 255;

    int B_LowV = 46;
    int B_HighV = 255;

    Mat imgOriginal=imread("blue.jpg");

    Mat B_imgThresholded,R_imgThresholded1,R_imgThresholded2;   //用来存储红色和蓝色图片的黑白图
    unsigned int number=imgOriginal.cols*imgOriginal.rows;
    unsigned int number_threshold=number*2/3;                   //像素点个数阈值
    //用来存储红色区域和蓝色区域像素点的个数
    unsigned int B_not_zero_num=0,R_not_zero_num=0,R_not_zero_num1=0,R_not_zero_num2=0;

    Mat imgHSV;
    vector<Mat> hsvSplit;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //将原图转换到HSV空间

    //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
    split(imgHSV, hsvSplit);
    equalizeHist(hsvSplit[2],hsvSplit[2]);  //增强对比度
    merge(hsvSplit,imgHSV);
    
    //确定红蓝像素点，存储为黑白图
    inRange(imgHSV, Scalar(B_LowH, B_LowS, B_LowV), Scalar(B_HighH, B_HighS, B_HighV), B_imgThresholded); 
    inRange(imgHSV, Scalar(R_LowH1, R_LowS1, R_LowV1), Scalar(R_HighH1, R_HighS1, R_HighV1), R_imgThresholded1);
    inRange(imgHSV, Scalar(R_LowH2, R_LowS2, R_LowV2), Scalar(R_HighH2, R_HighS2, R_HighV2), R_imgThresholded2);
    
    //对红蓝像素点计数，个数超过阈值即认为是红色或蓝色
    B_not_zero_num=countNonZero(B_imgThresholded);
    R_not_zero_num1=countNonZero(R_imgThresholded1);
    R_not_zero_num2=countNonZero(R_imgThresholded2);
    R_not_zero_num=R_not_zero_num1+R_not_zero_num2;

    if(B_not_zero_num>=number_threshold)
    {
        cout<<"picture color is:blue"<<endl;
        return 1;
    }

    if(R_not_zero_num>=number_threshold)
    {
        cout<<"picture color is:red"<<endl;
        return 2;
    }

    cout<<"picture color is not blue or red"<<endl;

    return 0;
}

