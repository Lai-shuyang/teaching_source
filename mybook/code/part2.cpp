/*******************************************************************
***************************程序说明*********************************
本程序包含三种访问图像像素点的方式，源图像窗口“video”中滑动条的
mode表示不同的访问方法，大家注意比较一下三种方式的帧率大小。0-2分别代
表使用at访问像素点、使用指针访问像素点、迭代器操作三种方式。
请在运行程序前设置摄像头允许被访问，否则程序无法正常运行。
********************************************************************/

#include<opencv2/opencv.hpp> 
#include <iostream>

using namespace cv;
using namespace std;

VideoCapture capture;
Mat srcImage;
int mode = 0;
int last_mode = 0;

void Image_white_1();//使用at访问像素点
void Image_white_2();//使用指针访问像素点
void Image_white_3();//迭代器操作

int main()
{
	capture.open(0); //打开摄像头
	if (!capture.isOpened())//如果视频不能正常打开则返回
	{
		cout << "摄像头打开失败！" << endl;
		return 1;
	}
	while (1)
	{
		double fps;//计算帧率
		double t = 0;
		t = cv::getTickCount();

		capture >> srcImage;//等价于 capture.read(srcImage);
		if (srcImage.empty())//如果某帧为空则退出循环
		{
			cout << "摄像头断开！" << endl;
			break;
		}

		namedWindow("video");
		createTrackbar("mode", "video", &mode, 2);//滑动条表示不同的mode

		//根据模式进行操作判断
		switch (mode)
		{
		case 0:
			Image_white_1();//使用at访问像素点
			break;
		case 1:
			Image_white_2();//使用指针访问像素点
			break;
		case 2:
			Image_white_3();//迭代器操作
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

//使用at访问像素点
void Image_white_1()
{
	int row = srcImage.rows;	//获取矩阵的行
	int col = srcImage.cols;	//获取矩阵的列
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			srcImage.at<cv::Vec3b>(i, j)[0] = 255;	//把第一个通道（蓝色）置于最大
			srcImage.at<cv::Vec3b>(i, j)[1] = 255;	//把第二个通道（绿色）置于最大
			srcImage.at<cv::Vec3b>(i, j)[2] = 255;	//把第三个通道（红色）置于最大
		}
	}
}

//使用指针访问像素点
void Image_white_2()
{
	int row = srcImage.rows;	//获取矩阵的行
	int col = srcImage.cols;	//获取矩阵的列
	int nStep = srcImage.cols * srcImage.channels();//计算总共需要遍历的像素点的个数
	for (int i = 0; i < row; i++)
	{
		uchar *Data = srcImage.ptr<uchar>(i);
		for (int j = 0; j < nStep; j++)
		{
			Data[j] = saturate_cast<uchar>(255);
		}
	}
}

//迭代器操作
void Image_white_3()
{
	MatIterator_<Vec3b> IterStart = srcImage.begin<Vec3b>();	//表示指向迭代器的起始位置
	MatIterator_<Vec3b> IterEnd = srcImage.end<Vec3b>();		//表示指向迭代器的终止位置
	while (IterStart != IterEnd)
	{
		(*IterStart)[0] = 255;	//把第一个通道（蓝色）置于最大
		(*IterStart)[1] = 255;	//把第二个通道（绿色）置于最大
		(*IterStart)[2] = 255;	//把第三个通道（红色）置于最大
		IterStart++;
	}
}