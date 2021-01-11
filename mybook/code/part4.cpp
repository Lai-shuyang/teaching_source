/*******************************************************************
***************************程序说明*********************************
本程序包含canny、Sobel、Scharr和Laplace算子的边缘检测算法以及
固定阈值化操作、自适应阈值化操作。
源图像窗口“video”中滑动条的mode表示不同的边缘检测算法、阈值化算法，
其中，0-3是四个边缘检测算法，4-5是两个阈值化操作。在进行4-5模式的运行
时，需要在程序的工程目录下放置图片part4.jpg，否则程序无法正常运行。
********************************************************************/

#include<opencv2/opencv.hpp> 
#include <iostream>

using namespace cv;
using namespace std;

VideoCapture capture;
Mat srcImage;
int mode = 0;
int last_mode = 0;
int value_canny = 20;
int thresh = 150;
int threshold_type = 0;
int adaptive_type = 0;
int C = 5;

void my_canny();//canny边缘检测算法
void my_Sobel();//Sobel算子
void my_Scharr();//Scharr算子
void my_Laplace();//Laplace算子
void my_threshold();//固定阈值化操作
void my_adaptiveThreshold();//自适应阈值化操作
void my_destroy_window();//销毁窗口

int main()
{
	capture.open(0); //打开摄像头
	if (!capture.isOpened())//如果视频不能正常打开则返回
	{
		cout << "摄像头打开失败！" << endl;
		return 0;
	}
	while (1)
	{
		double fps;//计算帧率
		double t = 0;
		t = cv::getTickCount();

		if (mode >= 0 && mode <= 3)
		{
			capture >> srcImage;//等价于 capture.read(srcImage);
			if (srcImage.empty())//如果某帧为空则退出循环
			{
				cout << "摄像头断开！" << endl;
				return 1;
			}
		}
		else
		{
			srcImage = imread("part4.jpg");
			if (srcImage.empty())
			{
				cout << "无法打开图片！" << endl;
				return 1;
			}
		}

		namedWindow("video");
		createTrackbar("mode", "video", &mode, 5);//滑动条表示不同的mode
		my_destroy_window();//销毁窗口

		//根据模式进行操作判断
		switch (mode)
		{
		case 0:
			my_canny();//canny边缘检测算法
			break;
		case 1:
			my_Sobel();//my_Sobel算子
			break;
		case 2:
			my_Scharr();//Scharr算子
			break;
		case 3:
			my_Laplace();//Laplace算子
			break;
		case 4:
			my_threshold();//固定阈值化操作
			break;
		case 5:
			my_adaptiveThreshold();//自适应阈值化操作
			break;
		}

		imshow("video", srcImage);
		waitKey(1);//每帧延时 1 毫秒，如果不延时，图像将无法显示

		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		fps = 1.0 / t;
		cout << "fps=" << fps << endl;
	}
	return 0;
}

//canny边缘检测算法
void my_canny()
{
	//创建窗口和滑动条
	namedWindow("canny");
	createTrackbar("g_canny", "canny", &value_canny, 100);

	//进行canny边缘检测
	Mat grayImage, imgCanny;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//将原图像转换为灰度图像
	Canny(grayImage, imgCanny, value_canny, 3 * value_canny, 3);//进行一次canny边缘检测
	imshow("canny", imgCanny);
}

//Sobel算子
void my_Sobel()
{
	Mat grayImage,dstImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//将原图像转换为灰度图像
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
	
	//求 X方向梯度
	Sobel(grayImage, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);//使用线性变换转换输入数组元素成8位无符号整型
	imshow("X方向Sobel", abs_grad_x);

	//求Y方向梯度
	Sobel(grayImage, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("Y方向Sobel", abs_grad_y);
		
	//合并梯度(近似)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dstImage);
	imshow("整体方向Sobel", dstImage);
}

//Scharr算子
void my_Scharr()
{
	Mat grayImage, dstImage;
	GaussianBlur(srcImage, srcImage, Size(5, 5), 0, 0, BORDER_DEFAULT);
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//将原图像转换为灰度图像
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y;

	//求 X方向梯度
	Scharr(grayImage, grad_x, CV_16S, 1, 0);
	convertScaleAbs(grad_x, abs_grad_x);//使用线性变换转换输入数组元素成8位无符号整型
	imshow("X方向Scharr", abs_grad_x);

	//求Y方向梯度
	Scharr(grayImage, grad_y, CV_16S, 0, 1);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("Y方向Scharr", abs_grad_y);

	//合并梯度(近似)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dstImage);
	imshow("整体方向Scharr", dstImage);
}

//Laplace算子
void my_Laplace()
{
	Mat grayImage, midImage, dstImage;
	//转化为灰度图
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//高斯滤波
	GaussianBlur(grayImage, midImage, Size(3, 3), 0, 0);
	//边缘检测
	Laplacian(midImage, dstImage, CV_16S, 3, 1, 0);
	Mat lastImage;
	convertScaleAbs(dstImage, lastImage);
	imshow("Laplace", lastImage);
}

//固定阈值化操作
void my_threshold()
{
	//创建窗口和滑动条
	namedWindow("threshold");
	createTrackbar("thresh", "threshold", &thresh, 255);
	createTrackbar("threshold_type", "threshold", &threshold_type, 4);

	Mat grayImage,dstImage;
	//转化为灰度图
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//阈值化
	threshold(grayImage, dstImage, thresh, 255, threshold_type);
	imshow("threshold", dstImage);
}

//自适应阈值化操作
void my_adaptiveThreshold()
{
	//创建窗口和滑动条
	namedWindow("adaptiveThreshold");
	createTrackbar("threshold_type", "adaptiveThreshold", &threshold_type, 1);
	createTrackbar("adaptive_type", "adaptiveThreshold", &adaptive_type, 1);
	createTrackbar("C", "adaptiveThreshold", &C, 20);
	if (threshold_type > 1)
		threshold_type = 1;

	Mat grayImage, dstImage;
	//转化为灰度图
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//高斯滤波
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	//阈值化
	adaptiveThreshold(grayImage, dstImage, 255, adaptive_type, threshold_type, 5, C);
	imshow("adaptiveThreshold", dstImage);
}

//销毁窗口
void my_destroy_window()
{
	//如果改变了模式，则需要销毁窗口
	if (last_mode != mode)
	{
		if (last_mode == 0)
		{
			destroyWindow("canny");
		}
		else if (last_mode == 1)
		{
			destroyWindow("X方向Sobel");
			destroyWindow("Y方向Sobel");
			destroyWindow("整体方向Sobel");
		}
		else if (last_mode == 2)
		{
			destroyWindow("X方向Scharr");
			destroyWindow("Y方向Scharr");
			destroyWindow("整体方向Scharr");
		}
		else if (last_mode == 3)
		{
			destroyWindow("Laplace");
		}
		else if (last_mode == 4)
		{
			destroyWindow("threshold");
		}
		else if (last_mode == 5)
		{
			destroyWindow("adaptiveThreshold");
		}
		last_mode = mode;
	}
}