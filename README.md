# 颜色识别实验报告

1.简介

    在正式比赛中，参赛队伍将会被分为红蓝阵营，故需要实现对颜色的识别来分辨敌我。
    
2.实验内容

    传入指定大小图片（例如 10*10 或者 20*20），判断图片区域颜色（以 肉眼感官为准），对其进行颜色判断(主要可分为红色、蓝色、杂  
    色),尽可能优化提高精确度。
    
3.实验要求

    使用 git 和 github，要求记录实验中所有代码和过程。 最终形成一份实验报告，不需太多文字叙述，体现主要解决方案、自我思考、不  
    足之处（如果有的话）、以及程序运行结果等，并附上 github 地址，如有参考文 献，将链接一并附上。
    
4.实验思路

    我们平时看到的彩图在计算机中是一个三维矩阵，每一点上的三个值分别对应RGB三原色值。通过改变三原色中RGB所占比例可以合成出任意  
    颜色，但是除了RGB颜色 空间，还存在HSV颜色空间，在识别颜色的时候我们需要在HSV空间进行。对于图像而言，识别相应的颜色在RGB空  
    间、HSV空间或者其它颜色空间都是可行的。之所以选择HSV，是因为H代表的色调基本上可以确定某种颜色，再结合饱和度和亮度信息判断大  
    于某一个阈值。而RGB由三个分量构成，需要判断每种分量的贡献比例，因此在HSV空间中更易于划分颜色。下图是在HSV空间中不同颜色区域。
 ![color](https://github.com/TianXingchen/color_detect/blob/master/cmake-build-debug/color.jpg)  
 使用inRange()函数将红色和蓝色像素点标记出来，是红色或蓝色则像素值记为255，否则记为0，最后对红色和蓝色像素点进行计数，如果像素点
 个数超过全部像素点的三分之二，则判断整幅图为红色或蓝色。  
 
5.实验结果

检测蓝色方程序运行结果：  

![result](https://github.com/TianXingchen/color_detect/blob/master/cmake-build-debug/result.JPG)

6.源代码及github地址  

#include "iostream"  

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
    split(imgHSV, hsvSplit);                //分离通道
    equalizeHist(hsvSplit[2],hsvSplit[2]);  //增强对比度
    merge(hsvSplit,imgHSV);                 //合并通道
    
    //确定红蓝像素点，存储为黑白图
    inRange(imgHSV, Scalar(B_LowH, B_LowS, B_LowV), Scalar(B_HighH, B_HighS, B_HighV), B_imgThresholded); 
    inRange(imgHSV, Scalar(R_LowH1, R_LowS1, R_LowV1), Scalar(R_HighH1, R_HighS1, R_HighV1), R_imgThresholded1);
    inRange(imgHSV, Scalar(R_LowH2, R_LowS2, R_LowV2), Scalar(R_HighH2, R_HighS2, R_HighV2), R_imgThresholded2);
    
    //对红蓝像素点计数，个数超过阈值即认为是红色或蓝色
    B_not_zero_num=countNonZero(B_imgThresholded);
    R_not_zero_num1=countNonZero(R_imgThresholded1);
    R_not_zero_num2=countNonZero(R_imgThresholded2);
    R_not_zero_num=R_not_zero_num1+R_not_zero_num2;

    //输出结果
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


GitHub地址：https://github.com/TianXingchen/color_detect/blob/master/README.md
