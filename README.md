# 颜色识别实验报告

1.简介

    在正式比赛中，参赛队伍将会被分为红蓝阵营，故需要实现对颜色的识别来分辨敌我。
    
2.实验内容

    传入指定大小图片（例如 10*10 或者 20*20），判断图片区域颜色（以 肉眼感官为准），对其进行颜色判断（主要可分为红色、蓝色、杂色），尽可能优 化提高精确度。
    
3.实验要求

    使用 git 和 github，要求记录实验中所有代码和过程。 最终形成一份实验报告，不需太多文字叙述，体现主要解决方案、自我思考、不 足之处（如果有的话）、以及程序运行结果等，并附上 github 地址，如有参考文 献，将链接一并附上。
    
4.实验思路

    我们平时看到的彩图在计算机中是一个三维矩阵，每一点上的三个值分别对应RGB三原色值。通过改变三原色中RGB所占比例可以合成出任意颜色，但是除了RGB颜色  
    空  间，还存在HSV颜色空间，在识别颜色的时候我们需要在HSV空间进行。对于图像而言，识别相应的颜色在RGB空间、HSV空间或者其它颜色空间都是可行的。之所以  
    选 择HSV，是因为H代表的色调基本上可以确定某种颜色，再结合饱和度和亮度信息判断大于某一个阈值。而RGB由三个分量构成，需要判断每种分量的贡献比例，因此  
    在HSV空间中更易于划分颜色。下图是在HSV空间中不同颜色区域。
 ![color](https://github.com/TianXingchen/color_detect/blob/master/cmake-build-debug/color.jpg)

5.实验结果

检测蓝色方程序运行结果：  

![result](https://github.com/TianXingchen/color_detect/blob/master/cmake-build-debug/result.jpg)

6.源代码及github地址

#include "iostream"
    
#include "opencv2/highgui/highgui.hpp"  

#include "opencv2/imgproc/imgproc.hpp"  

using namespace cv;  

using namespace std ;

int main( int argc, char** argv )
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //创建名为“control”的窗口

    int iLowH = 100;
    int iHighH = 140;

    int iLowS = 90;
    int iHighS = 255;

    int iLowV = 90;
    int iHighV = 255;

    //在“control”窗口中创建滑动条
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); 	//H (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); 		//S (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); 	//V (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
     Mat img=imread("robot.jpg");		//读入名为“robot.jpg”的图片
    Size size=Size(400,400);
    Mat imgOriginal=Mat(size,CV_32S);
    resize(img,imgOriginal,size);		   //调整尺寸
    while (true)
    {
        Mat imgHSV;
        vector<Mat> hsvSplit;
        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //将原图从GRB空间转换的HSV空间 
        //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
        split(imgHSV, hsvSplit);		//将imgHSV进行通道分离
        equalizeHist(hsvSplit[2],hsvSplit[2]);//直方图均衡化，增强对比度
        merge(hsvSplit,imgHSV);		//将分离后的通道合并
        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //生成满足所要求颜色范围的二值化图像，所要检测的颜色区域为白色，其余为黑色。

        //开操作 (去除一些噪点)
        Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));// 函数会返回尺寸为5*5的矩形结构元素
        morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);// 先腐蚀后膨胀，放大裂缝和低密度区域，消除小物体，在平滑较大物体的边界时，不改变其面积

        //闭操作 (连接一些连通域)
        morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);// 先膨胀后腐蚀，排除小型黑洞，突触了比原图轮廓区域更暗的区域

        imshow("Thresholded Image", imgThresholded); 
        imshow("Original", imgOriginal); 
        char key = (char) waitKey(300);
        if(key == 27)
            break;
    }
    return 0;
}

GitHub地址：https://github.com/TianXingchen/ubiquitous-computing-machine/edit/master/README.md
