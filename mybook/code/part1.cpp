/*******************************************************************
***************************程序说明*********************************
本程序包含形态学滤波操作和画图操作。mode=0表示形态学滤波操作，mode=1
表示画图操作。mode=0时，morph_type中0-6分别表示MORPH_ERODE（腐蚀）、
MORPH_DILATE（膨胀）、MORPH_OPEN（开运算）、MORPH_CLOSE（闭运算）、
MORPH_GRADIENT（形态学梯度）、MORPH_TOPHAT（顶帽）、MORPH_BLACKHAT
（黑帽）这七个操作。element_size可调整形态学滤波所用算子大小。
请在运行程序前在工程目录下防止"SAST.jpg"图片，否则程序无法正常运行。
********************************************************************/

#include<opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat srcImage;
int mode = 0;
int last_mode = 0;
int morph_type = 0;
int element_size = 5;

void my_morph();//形态学滤波操作
void draw_image();//画图
void DrawEllipse(Mat Image, int window_size, double angle);//画椭圆
void DrawFilledCircle(Mat Image, int window_size, Point center);//绘制实心圆

int main()
{
	srcImage = imread("SAST.jpg");
	if (srcImage.empty())
	{
		cout << "无法找到文件！" << endl;
		return 1;
	}

	while (1)
	{
		//创建窗口和滑动条
		namedWindow("video");
		createTrackbar("mode", "video", &mode, 1);//滑动条表示不同的mode
		if (mode == 0)
		{
			my_morph();//形态学滤波操作
			last_mode = 0;
		}
		else
		{
			draw_image();//画图
			//销毁形态学滤波的窗口
			if (last_mode == 0)
			{
				destroyWindow("MORPH");
				last_mode = 1;
			}
		}

		waitKey(1);//每帧延时 1 毫秒，如果不延时，图像将无法显示
	}

	return 0;
}

//形态学滤波操作
void my_morph()
{
	//创建窗口和滑动条
	namedWindow("MORPH");
	createTrackbar("morph_type", "MORPH", &morph_type, 6);
	createTrackbar("element_size", "MORPH", &element_size, 30);
	if (element_size < 1)
		element_size = 1;
	Mat element;
	Mat dstImage;
	//提供形态学运算用的算子
	element = getStructuringElement(MORPH_RECT, Size(element_size, element_size));
	//形态学滤波操作
	morphologyEx(srcImage, dstImage, morph_type, element);
	imshow("MORPH", dstImage);//显示
	imshow("video", srcImage);//原图
}

//画图
void draw_image()
{
	int window_size = 600;
	//创建一个黑色的矩阵
	Mat image_white(window_size, window_size, CV_8UC3, Scalar(0, 0, 0));
	//等价于
	//Mat Image;
	//Image.create(window_size, window_size, CV_8UC3, Scalar(0, 0, 0));
	//绘制椭圆
	DrawEllipse(image_white, window_size, 90);
	DrawEllipse(image_white, window_size, 0);
	DrawEllipse(image_white, window_size, 45);
	DrawEllipse(image_white, window_size, -45);
	//绘制圆心
	DrawFilledCircle(image_white, window_size, Point(window_size / 2, window_size / 2));
	//显示画完图的图像
	imshow("video", image_white);
}

//画椭圆
void DrawEllipse(Mat Image, int window_size, double angle)
{
	ellipse(Image, //所操作的图像
		Point(window_size / 2, window_size / 2),//中心点
		Size(window_size / 4, window_size / 16),//大小位于此内
		angle,//旋转角度
		0, 360,//扩展弧度为【0,360】
		Scalar(200, 19, 12),//颜色
		2,//线宽
		8);//线条类型为8连通线型
}

//绘制实心圆
void DrawFilledCircle(Mat Image, int window_size, Point center)
{
	circle(Image,//所操作的图像
		center,//中心点坐标
		window_size / 32,//圆的半径
		Scalar(178, 0, 159),//颜色
		-1,//线宽为填充型
		8);//线条类型为8连通线型
}