/*******************************************************************
***************************程序说明*********************************
本程序包含方框滤波、均值滤波、高斯滤波、中值滤波、双边滤波五种滤波方
式，源图像窗口“video”中滑动条的mode表示不同的滤波方法，0-4分别代表
方框滤波、均值滤波、高斯滤波、中值滤波、双边滤波五种滤波方式。其中，
0-2为线性滤波，3-4为非线性。
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
int boxFilter_size = 3;
int blur_size = 3;
int GaussianBlur_size = 3;
int medianBlur_size = 3;
int bilateral_size = 3;

void my_boxFilter();//方框滤波
void my_blur();//均值滤波
void my_GaussianBlur();//高斯滤波
void my_medianBlur();//中值滤波
void my_bilateralBlur();//双边滤波
void my_destroy_window();//销毁窗口

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
		createTrackbar("mode", "video", &mode, 4);//滑动条表示不同的mode
		my_destroy_window();//销毁窗口

		//根据模式进行操作判断
		switch (mode)
		{
		case 0:
			my_boxFilter();//方框滤波
			break;
		case 1:
			my_blur();//均值滤波
			break;
		case 2:
			my_GaussianBlur();//高斯滤波
			break;
		case 3:
			my_medianBlur();//中值滤波
			break;
		case 4:
			my_bilateralBlur();//双边滤波
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

//方框滤波
void my_boxFilter()
{
	Mat dstImage;
	namedWindow("boxFilter");//创建一个窗口
	createTrackbar("boxFilter_element", "boxFilter", &boxFilter_size, 50);//创建一个滑动条，用以调整滤波的算子大小
	if (boxFilter_size == 0)//运算核的大小不能为0，否则会报错
		boxFilter_size = 1;
	boxFilter(srcImage, dstImage, -1, Size(boxFilter_size, boxFilter_size));
	imshow("boxFilter", dstImage);//显示滤完波的图像
}

//均值滤波
void my_blur()
{
	Mat element;
	Mat dstImage;
	namedWindow("blur");//创建一个窗口
	createTrackbar("blur_element", "blur", &blur_size, 50);//创建一个滑动条，用以调整滤波的算子大小
	if (blur_size == 0)//运算核的大小不能为0，否则会报错
		blur_size = 1;
	blur(srcImage, dstImage, Size(blur_size, blur_size));//均值滤波
	imshow("blur", dstImage);//显示滤完波的图像
}

//高斯滤波
void my_GaussianBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("GaussianBlur");//创建一个窗口
	createTrackbar("GaussianBlur_element", "GaussianBlur", &GaussianBlur_size, 50);//创建一个滑动条，用以调整滤波的算子大小
	if (GaussianBlur_size % 2 == 0)//运算核的大小不能为偶数，否则会报错
		GaussianBlur_size += 1;
	GaussianBlur(srcImage, dstImage, Size(GaussianBlur_size, GaussianBlur_size), 0, 0);//高斯滤波
	imshow("GaussianBlur", dstImage);//显示滤完波的图像
}

//中值滤波
void my_medianBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("medianBlur");//创建一个窗口
	createTrackbar("medianBlur_element", "medianBlur", &medianBlur_size, 50);//创建一个滑动条，用以调整滤波的算子大小
	if (medianBlur_size % 2 == 0)//运算核的大小不能为偶数，否则会报错
		medianBlur_size += 1;
	if (medianBlur_size == 1)//运算核的大小需要大于1
		medianBlur_size += 2;
	medianBlur(srcImage, dstImage, medianBlur_size);//中值滤波
	imshow("medianBlur", dstImage);//显示滤完波的图像
}

//双边滤波
void my_bilateralBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("bilateralBlur");//创建一个窗口
	createTrackbar("bilateralBlur_element", "bilateralBlur", &bilateral_size, 50);//创建一个滑动条，用以调整滤波的算子大小
	if (bilateral_size == 0 || bilateral_size == 1)//运算核的大小需要大于1
		bilateral_size = 2;
	bilateralFilter(srcImage, dstImage, bilateral_size, bilateral_size * 2, bilateral_size / 2);//双边滤波
	imshow("bilateralBlur", dstImage);//显示滤完波的图像
}

//销毁窗口
void my_destroy_window()
{
	//如果改变了模式，则需要销毁窗口
	if (last_mode != mode)
	{
		if (last_mode == 0)
		{
			destroyWindow("boxFilter");
		}
		else if (last_mode == 1)
		{
			destroyWindow("blur");
		}
		else if (last_mode == 2)
		{
			destroyWindow("GaussianBlur");
		}
		else if (last_mode == 3)
		{
			destroyWindow("medianBlur");
		}
		else if (last_mode == 4)
		{
			destroyWindow("bilateralBlur");
		}
		last_mode = mode;
	}
}
