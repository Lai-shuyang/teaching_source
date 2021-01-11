/*******************************************************************
***************************程序说明*********************************
本程序包含标准霍夫线变换、累积概率霍夫线变换、霍夫圆变换和寻找轮廓、
寻找最大轮廓中心点的五个函数，前三个霍夫变换分别对应video窗口中滑动条
mode的0-2，后两个对应3-4。
程序运行前需在工程目录下放置以part5开头的所有图片，否则程序无法正常运
行。
********************************************************************/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat srcImage;
int mode = 0;
int last_mode = 0;
int g_canny_hough = 50;
int contour_mode = 0;
int g_houghcircles1 = 14;//霍夫圆变换的参数
int g_houghcircles2 = 66;
int g_houghcircles3 = 66;
int thresh_line_1 = 150;//标准霍夫线变换的阈值
int g_houghlines1 = 80;//累积概率霍夫线变换的参数
int g_houghlines2 = 50;
int g_houghlines3 = 10;

void my_hough_line1();//标准霍夫线变换
void my_hough_line2();//累积概率霍夫线变换
void my_hough_circles();//霍夫圆变换
void my_find_contours();//寻找轮廓
void my_rect_contours();//寻找最大轮廓中心点
void my_destroy_window();//销毁窗口

int main()
{
	srcImage = imread("part5_1.png");
	if (srcImage.empty())
	{
		cout << "请在工程目录下放置对应图片！" << endl;
		return 1;
	}
	while (1)
	{
		double fps;//计算帧率
		double t = 0;
		t = cv::getTickCount();

		//创建窗口和滑动条
		namedWindow("video");
		createTrackbar("mode", "video", &mode, 4);//滑动条表示不同的mode
		my_destroy_window();//销毁窗口

		//根据模式进行操作判断
		switch (mode)
		{
		case 0:
			my_hough_line1();//标准霍夫线变换
			break;
		case 1:
			my_hough_line2();//累积概率霍夫线变换
			break;
		case 2:
			my_hough_circles();//霍夫圆变换
			break;
		case 3:
			my_find_contours();//寻找轮廓
			break;
		case 4:
			my_rect_contours(); //寻找最大轮廓中心点
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

//标准霍夫线变换
void my_hough_line1()
{
	namedWindow("line1");
	createTrackbar("累加器阈值", "line1", &thresh_line_1, 300);
	if (thresh_line_1 < 1)//该值不能为1
		thresh_line_1 = 1;

	srcImage = imread("part5_1.png");
	Mat grayImage, imgCanny;
	//进行灰度转化、边缘检测
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//转化为灰度图
	Canny(grayImage, imgCanny, 50, 200, 3);//进行canny边缘检测

	//进行霍夫线变换
	vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLines(imgCanny, lines, 1, CV_PI / 180, thresh_line_1, 0, 0);
	//依次在图中绘制出每条线段
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		//画出来
		line(srcImage, pt1, pt2, Scalar(155, 50, 255), 1, LINE_AA);
	}
	imshow("line1", imgCanny);//显示canny边缘检测的结果
}

//累积概率霍夫线变换
void my_hough_line2()
{
	namedWindow("line2");
	createTrackbar("累加平面的阈值参数", "line2", &g_houghlines1, 300);
	createTrackbar("最短线段长度", "line2", &g_houghlines2, 250);
	createTrackbar("允许连接的最大距离", "line2", &g_houghlines3, 250);
	if (g_houghlines1 < 1)//该值不能为1
		g_houghlines1 = 1;

	srcImage = imread("part5_1.png");
	Mat grayImage, imgCanny;
	//进行灰度转化、边缘检测
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//转化为灰度图
	Canny(grayImage, imgCanny, 50, 200, 3);//进行canny边缘检测

	//进行霍夫线变换
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
	HoughLinesP(imgCanny, lines, 1, CV_PI / 180, g_houghlines1, g_houghlines2, g_houghlines3);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//画出来
		line(srcImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(155, 50, 255), 1, LINE_AA);
	}
	imshow("line2", imgCanny);//显示canny边缘检测的结果
}

//霍夫圆变换
void my_hough_circles()
{
	srcImage = imread("part5_2.png");
	//创建窗口和滑动条
	// 创建显示窗口
	namedWindow("Circles", WINDOW_AUTOSIZE);
	// 创建滑动条
	createTrackbar("最小半径：", "Circles", &g_houghcircles1, 300);
	createTrackbar("最大半径", "Circles", &g_houghcircles2, 1000);
	createTrackbar("累加器阈值", "Circles", &g_houghcircles3, 300);
	if (g_houghcircles3 < 1)//这个值不能为0，防止报错
		g_houghcircles3 = 1;

	Mat grayImage, imgThreshold;
	//转为灰度图并进行反阈值化操作
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, imgThreshold, 100, 255, 1);

	//进行霍夫圆变换
	vector<Vec3f> circles;
	HoughCircles(grayImage, circles, HOUGH_GRADIENT, 1.5, 30, 100, g_houghcircles3, g_houghcircles1, g_houghcircles2);

	//依次在图中绘制出圆
	for (size_t i = 0; i < circles.size(); i++)
	{
		//参数定义
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//绘制圆心
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制圆轮廓
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
	}
	imshow("Circles", imgThreshold);//显示反二值化后的图像
}

//寻找轮廓
void my_find_contours()
{
	srcImage = imread("part5_3.png");
	namedWindow("contours");
	createTrackbar("contour_mode", "contours", &contour_mode, 3);//轮廓检测的模式

	// 将原图像转换为灰度图像
	Mat grayImage, imgCanny;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//进行一次canny边缘检测
	Canny(grayImage, imgCanny, g_canny_hough, 3 * g_canny_hough, 3);
	//寻找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgCanny, contours, hierarchy, contour_mode, CHAIN_APPROX_NONE);

	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		//调了个诡异的颜色，为了让大家看清楚轮廓的层次关系
		Scalar color(min(index * 40, 255), 200, min(max(index*40-250,0),255));
		//绘制轮廓
		drawContours(srcImage, contours, index, color, 3, 8, hierarchy);
	}
	imshow("contours", imgCanny);//显示canny边缘检测的结果
}

//寻找最大轮廓中心点
void my_rect_contours()
{
	srcImage = imread("part5_4.png");
	namedWindow("contours");
	createTrackbar("contour_mode", "contours", &contour_mode, 3);//轮廓检测的模式

	//反二进制阈值化，因为图像中有效区域被认为是白色
	Mat grayImage, imgThreshold;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, imgThreshold, 100, 255, 1);
	//寻找轮廓
	vector<vector<Point>> contours;
	findContours(imgThreshold, contours, RETR_LIST, CHAIN_APPROX_NONE);

	//寻找最大轮廓
	int i, k;
	double length_max = 0;
	//这里我用的是计算轮廓长度，你也可以计算轮廓面积
	if (contours.size()>0)
	{
		k = 0;
		for (i = 0; i<contours.size(); i++)
		{
			drawContours(srcImage, contours, i, Scalar(189, 30, 50), 3);
			if (arcLength(contours[i], 0)>length_max)
			{
				length_max = arcLength(contours[i], 0);
				k = i;
			}
		}
		//绘制最大轮廓
		drawContours(srcImage, contours, k, Scalar(189, 30, 50), 3);
		//计算最大轮廓中心点坐标
		Point2f vertices[4];
		Point2f center_box;
		RotatedRect box = minAreaRect(contours[k]);//寻找最小包围矩形
		box.points(vertices);
		//计算中心点坐标
		center_box.x = (vertices[0].x + vertices[2].x) / 2.0;
		center_box.y = (vertices[0].y + vertices[2].y) / 2.0;
		circle(srcImage, center_box, 3, Scalar(0, 255, 0), -1, 8, 0);
		//绘制最小面积包围矩形
		for (int i = 0; i < 4; i++)
			line(srcImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2, 8);
	}
	imshow("contours", imgThreshold);//显示反二值化后的图像
}

//销毁窗口
void my_destroy_window()
{
	//如果改变了模式，则需要销毁窗口
	if (last_mode != mode)
	{
		if (last_mode == 0)
		{
			destroyWindow("line1");
		}
		else if (last_mode == 1)
		{
			destroyWindow("line2");
		}
		else if (last_mode == 2)
		{
			destroyWindow("Circles");
		}
		else if (last_mode == 3)
		{
			destroyWindow("contours");
		}
		else if (last_mode == 4)
		{
			destroyWindow("contours");
		}
		last_mode = mode;
	}
}