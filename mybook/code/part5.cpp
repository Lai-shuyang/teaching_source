/*******************************************************************
***************************����˵��*********************************
�����������׼�����߱任���ۻ����ʻ����߱任������Բ�任��Ѱ��������
Ѱ������������ĵ�����������ǰ��������任�ֱ��Ӧvideo�����л�����
mode��0-2����������Ӧ3-4��
��������ǰ���ڹ���Ŀ¼�·�����part5��ͷ������ͼƬ����������޷�������
�С�
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
int g_houghcircles1 = 14;//����Բ�任�Ĳ���
int g_houghcircles2 = 66;
int g_houghcircles3 = 66;
int thresh_line_1 = 150;//��׼�����߱任����ֵ
int g_houghlines1 = 80;//�ۻ����ʻ����߱任�Ĳ���
int g_houghlines2 = 50;
int g_houghlines3 = 10;

void my_hough_line1();//��׼�����߱任
void my_hough_line2();//�ۻ����ʻ����߱任
void my_hough_circles();//����Բ�任
void my_find_contours();//Ѱ������
void my_rect_contours();//Ѱ������������ĵ�
void my_destroy_window();//���ٴ���

int main()
{
	srcImage = imread("part5_1.png");
	if (srcImage.empty())
	{
		cout << "���ڹ���Ŀ¼�·��ö�ӦͼƬ��" << endl;
		return 1;
	}
	while (1)
	{
		double fps;//����֡��
		double t = 0;
		t = cv::getTickCount();

		//�������ںͻ�����
		namedWindow("video");
		createTrackbar("mode", "video", &mode, 4);//��������ʾ��ͬ��mode
		my_destroy_window();//���ٴ���

		//����ģʽ���в����ж�
		switch (mode)
		{
		case 0:
			my_hough_line1();//��׼�����߱任
			break;
		case 1:
			my_hough_line2();//�ۻ����ʻ����߱任
			break;
		case 2:
			my_hough_circles();//����Բ�任
			break;
		case 3:
			my_find_contours();//Ѱ������
			break;
		case 4:
			my_rect_contours(); //Ѱ������������ĵ�
			break;
		}
		imshow("video", srcImage);
		waitKey(1);//ÿ֡��ʱ 1 ���룬�������ʱ��ͼ���޷���ʾ

		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		fps = 1.0 / t;
		cout << "fps=" << fps << endl;
	}
	return 0;
}

//��׼�����߱任
void my_hough_line1()
{
	namedWindow("line1");
	createTrackbar("�ۼ�����ֵ", "line1", &thresh_line_1, 300);
	if (thresh_line_1 < 1)//��ֵ����Ϊ1
		thresh_line_1 = 1;

	srcImage = imread("part5_1.png");
	Mat grayImage, imgCanny;
	//���лҶ�ת������Ե���
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
	Canny(grayImage, imgCanny, 50, 200, 3);//����canny��Ե���

	//���л����߱任
	vector<Vec2f> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
	HoughLines(imgCanny, lines, 1, CV_PI / 180, thresh_line_1, 0, 0);
	//������ͼ�л��Ƴ�ÿ���߶�
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
		//������
		line(srcImage, pt1, pt2, Scalar(155, 50, 255), 1, LINE_AA);
	}
	imshow("line1", imgCanny);//��ʾcanny��Ե���Ľ��
}

//�ۻ����ʻ����߱任
void my_hough_line2()
{
	namedWindow("line2");
	createTrackbar("�ۼ�ƽ�����ֵ����", "line2", &g_houghlines1, 300);
	createTrackbar("����߶γ���", "line2", &g_houghlines2, 250);
	createTrackbar("�������ӵ�������", "line2", &g_houghlines3, 250);
	if (g_houghlines1 < 1)//��ֵ����Ϊ1
		g_houghlines1 = 1;

	srcImage = imread("part5_1.png");
	Mat grayImage, imgCanny;
	//���лҶ�ת������Ե���
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ
	Canny(grayImage, imgCanny, 50, 200, 3);//����canny��Ե���

	//���л����߱任
	vector<Vec4i> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������
	HoughLinesP(imgCanny, lines, 1, CV_PI / 180, g_houghlines1, g_houghlines2, g_houghlines3);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		//������
		line(srcImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(155, 50, 255), 1, LINE_AA);
	}
	imshow("line2", imgCanny);//��ʾcanny��Ե���Ľ��
}

//����Բ�任
void my_hough_circles()
{
	srcImage = imread("part5_2.png");
	//�������ںͻ�����
	// ������ʾ����
	namedWindow("Circles", WINDOW_AUTOSIZE);
	// ����������
	createTrackbar("��С�뾶��", "Circles", &g_houghcircles1, 300);
	createTrackbar("���뾶", "Circles", &g_houghcircles2, 1000);
	createTrackbar("�ۼ�����ֵ", "Circles", &g_houghcircles3, 300);
	if (g_houghcircles3 < 1)//���ֵ����Ϊ0����ֹ����
		g_houghcircles3 = 1;

	Mat grayImage, imgThreshold;
	//תΪ�Ҷ�ͼ�����з���ֵ������
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, imgThreshold, 100, 255, 1);

	//���л���Բ�任
	vector<Vec3f> circles;
	HoughCircles(grayImage, circles, HOUGH_GRADIENT, 1.5, 30, 100, g_houghcircles3, g_houghcircles1, g_houghcircles2);

	//������ͼ�л��Ƴ�Բ
	for (size_t i = 0; i < circles.size(); i++)
	{
		//��������
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		//����Բ��
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		//����Բ����
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
	}
	imshow("Circles", imgThreshold);//��ʾ����ֵ�����ͼ��
}

//Ѱ������
void my_find_contours()
{
	srcImage = imread("part5_3.png");
	namedWindow("contours");
	createTrackbar("contour_mode", "contours", &contour_mode, 3);//��������ģʽ

	// ��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grayImage, imgCanny;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//����һ��canny��Ե���
	Canny(grayImage, imgCanny, g_canny_hough, 3 * g_canny_hough, 3);
	//Ѱ������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgCanny, contours, hierarchy, contour_mode, CHAIN_APPROX_NONE);

	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		//���˸��������ɫ��Ϊ���ô�ҿ���������Ĳ�ι�ϵ
		Scalar color(min(index * 40, 255), 200, min(max(index*40-250,0),255));
		//��������
		drawContours(srcImage, contours, index, color, 3, 8, hierarchy);
	}
	imshow("contours", imgCanny);//��ʾcanny��Ե���Ľ��
}

//Ѱ������������ĵ�
void my_rect_contours()
{
	srcImage = imread("part5_4.png");
	namedWindow("contours");
	createTrackbar("contour_mode", "contours", &contour_mode, 3);//��������ģʽ

	//����������ֵ������Ϊͼ������Ч������Ϊ�ǰ�ɫ
	Mat grayImage, imgThreshold;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	threshold(grayImage, imgThreshold, 100, 255, 1);
	//Ѱ������
	vector<vector<Point>> contours;
	findContours(imgThreshold, contours, RETR_LIST, CHAIN_APPROX_NONE);

	//Ѱ���������
	int i, k;
	double length_max = 0;
	//�������õ��Ǽ����������ȣ���Ҳ���Լ����������
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
		//�����������
		drawContours(srcImage, contours, k, Scalar(189, 30, 50), 3);
		//��������������ĵ�����
		Point2f vertices[4];
		Point2f center_box;
		RotatedRect box = minAreaRect(contours[k]);//Ѱ����С��Χ����
		box.points(vertices);
		//�������ĵ�����
		center_box.x = (vertices[0].x + vertices[2].x) / 2.0;
		center_box.y = (vertices[0].y + vertices[2].y) / 2.0;
		circle(srcImage, center_box, 3, Scalar(0, 255, 0), -1, 8, 0);
		//������С�����Χ����
		for (int i = 0; i < 4; i++)
			line(srcImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2, 8);
	}
	imshow("contours", imgThreshold);//��ʾ����ֵ�����ͼ��
}

//���ٴ���
void my_destroy_window()
{
	//����ı���ģʽ������Ҫ���ٴ���
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