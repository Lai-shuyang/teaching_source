/*******************************************************************
***************************����˵��*********************************
�����������̬ѧ�˲������ͻ�ͼ������mode=0��ʾ��̬ѧ�˲�������mode=1
��ʾ��ͼ������mode=0ʱ��morph_type��0-6�ֱ��ʾMORPH_ERODE����ʴ����
MORPH_DILATE�����ͣ���MORPH_OPEN�������㣩��MORPH_CLOSE�������㣩��
MORPH_GRADIENT����̬ѧ�ݶȣ���MORPH_TOPHAT����ñ����MORPH_BLACKHAT
����ñ�����߸�������element_size�ɵ�����̬ѧ�˲��������Ӵ�С��
�������г���ǰ�ڹ���Ŀ¼�·�ֹ"SAST.jpg"ͼƬ����������޷��������С�
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

void my_morph();//��̬ѧ�˲�����
void draw_image();//��ͼ
void DrawEllipse(Mat Image, int window_size, double angle);//����Բ
void DrawFilledCircle(Mat Image, int window_size, Point center);//����ʵ��Բ

int main()
{
	srcImage = imread("SAST.jpg");
	if (srcImage.empty())
	{
		cout << "�޷��ҵ��ļ���" << endl;
		return 1;
	}

	while (1)
	{
		//�������ںͻ�����
		namedWindow("video");
		createTrackbar("mode", "video", &mode, 1);//��������ʾ��ͬ��mode
		if (mode == 0)
		{
			my_morph();//��̬ѧ�˲�����
			last_mode = 0;
		}
		else
		{
			draw_image();//��ͼ
			//������̬ѧ�˲��Ĵ���
			if (last_mode == 0)
			{
				destroyWindow("MORPH");
				last_mode = 1;
			}
		}

		waitKey(1);//ÿ֡��ʱ 1 ���룬�������ʱ��ͼ���޷���ʾ
	}

	return 0;
}

//��̬ѧ�˲�����
void my_morph()
{
	//�������ںͻ�����
	namedWindow("MORPH");
	createTrackbar("morph_type", "MORPH", &morph_type, 6);
	createTrackbar("element_size", "MORPH", &element_size, 30);
	if (element_size < 1)
		element_size = 1;
	Mat element;
	Mat dstImage;
	//�ṩ��̬ѧ�����õ�����
	element = getStructuringElement(MORPH_RECT, Size(element_size, element_size));
	//��̬ѧ�˲�����
	morphologyEx(srcImage, dstImage, morph_type, element);
	imshow("MORPH", dstImage);//��ʾ
	imshow("video", srcImage);//ԭͼ
}

//��ͼ
void draw_image()
{
	int window_size = 600;
	//����һ����ɫ�ľ���
	Mat image_white(window_size, window_size, CV_8UC3, Scalar(0, 0, 0));
	//�ȼ���
	//Mat Image;
	//Image.create(window_size, window_size, CV_8UC3, Scalar(0, 0, 0));
	//������Բ
	DrawEllipse(image_white, window_size, 90);
	DrawEllipse(image_white, window_size, 0);
	DrawEllipse(image_white, window_size, 45);
	DrawEllipse(image_white, window_size, -45);
	//����Բ��
	DrawFilledCircle(image_white, window_size, Point(window_size / 2, window_size / 2));
	//��ʾ����ͼ��ͼ��
	imshow("video", image_white);
}

//����Բ
void DrawEllipse(Mat Image, int window_size, double angle)
{
	ellipse(Image, //��������ͼ��
		Point(window_size / 2, window_size / 2),//���ĵ�
		Size(window_size / 4, window_size / 16),//��Сλ�ڴ���
		angle,//��ת�Ƕ�
		0, 360,//��չ����Ϊ��0,360��
		Scalar(200, 19, 12),//��ɫ
		2,//�߿�
		8);//��������Ϊ8��ͨ����
}

//����ʵ��Բ
void DrawFilledCircle(Mat Image, int window_size, Point center)
{
	circle(Image,//��������ͼ��
		center,//���ĵ�����
		window_size / 32,//Բ�İ뾶
		Scalar(178, 0, 159),//��ɫ
		-1,//�߿�Ϊ�����
		8);//��������Ϊ8��ͨ����
}