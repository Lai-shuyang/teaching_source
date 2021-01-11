/*******************************************************************
***************************����˵��*********************************
��������������˲�����ֵ�˲�����˹�˲�����ֵ�˲���˫���˲������˲���
ʽ��Դͼ�񴰿ڡ�video���л�������mode��ʾ��ͬ���˲�������0-4�ֱ����
�����˲�����ֵ�˲�����˹�˲�����ֵ�˲���˫���˲������˲���ʽ�����У�
0-2Ϊ�����˲���3-4Ϊ�����ԡ�
�������г���ǰ��������ͷ�������ʣ���������޷��������С�
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

void my_boxFilter();//�����˲�
void my_blur();//��ֵ�˲�
void my_GaussianBlur();//��˹�˲�
void my_medianBlur();//��ֵ�˲�
void my_bilateralBlur();//˫���˲�
void my_destroy_window();//���ٴ���

int main()
{
	capture.open(0); //������ͷ
	if (!capture.isOpened())//�����Ƶ�����������򷵻�
	{
		cout << "����ͷ��ʧ�ܣ�" << endl;
		return 1;
	}
	while (1)
	{
		double fps;//����֡��
		double t = 0;
		t = cv::getTickCount();

		capture >> srcImage;//�ȼ��� capture.read(srcImage);
		if (srcImage.empty())//���ĳ֡Ϊ�����˳�ѭ��
		{
			cout << "����ͷ�Ͽ���" << endl;
			break;
		}

		namedWindow("video");
		createTrackbar("mode", "video", &mode, 4);//��������ʾ��ͬ��mode
		my_destroy_window();//���ٴ���

		//����ģʽ���в����ж�
		switch (mode)
		{
		case 0:
			my_boxFilter();//�����˲�
			break;
		case 1:
			my_blur();//��ֵ�˲�
			break;
		case 2:
			my_GaussianBlur();//��˹�˲�
			break;
		case 3:
			my_medianBlur();//��ֵ�˲�
			break;
		case 4:
			my_bilateralBlur();//˫���˲�
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

//�����˲�
void my_boxFilter()
{
	Mat dstImage;
	namedWindow("boxFilter");//����һ������
	cvCreateTrackbar("boxFilter_element", "boxFilter", &boxFilter_size, 50);//����һ�������������Ե����˲������Ӵ�С
	if (boxFilter_size == 0)//����˵Ĵ�С����Ϊ0������ᱨ��
		boxFilter_size = 1;
	boxFilter(srcImage, dstImage, -1, Size(boxFilter_size, boxFilter_size));
	imshow("boxFilter", dstImage);//��ʾ���겨��ͼ��
}

//��ֵ�˲�
void my_blur()
{
	Mat element;
	Mat dstImage;
	namedWindow("blur");//����һ������
	cvCreateTrackbar("blur_element", "blur", &blur_size, 50);//����һ�������������Ե����˲������Ӵ�С
	if (blur_size == 0)//����˵Ĵ�С����Ϊ0������ᱨ��
		blur_size = 1;
	blur(srcImage, dstImage, Size(blur_size, blur_size));//��ֵ�˲�
	imshow("blur", dstImage);//��ʾ���겨��ͼ��
}

//��˹�˲�
void my_GaussianBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("GaussianBlur");//����һ������
	cvCreateTrackbar("GaussianBlur_element", "GaussianBlur", &GaussianBlur_size, 50);//����һ�������������Ե����˲������Ӵ�С
	if (GaussianBlur_size % 2 == 0)//����˵Ĵ�С����Ϊż��������ᱨ��
		GaussianBlur_size += 1;
	GaussianBlur(srcImage, dstImage, Size(GaussianBlur_size, GaussianBlur_size), 0, 0);//��˹�˲�
	imshow("GaussianBlur", dstImage);//��ʾ���겨��ͼ��
}

//��ֵ�˲�
void my_medianBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("medianBlur");//����һ������
	cvCreateTrackbar("medianBlur_element", "medianBlur", &medianBlur_size, 50);//����һ�������������Ե����˲������Ӵ�С
	if (medianBlur_size % 2 == 0)//����˵Ĵ�С����Ϊż��������ᱨ��
		medianBlur_size += 1;
	if (medianBlur_size == 1)//����˵Ĵ�С��Ҫ����1
		medianBlur_size += 2;
	medianBlur(srcImage, dstImage, medianBlur_size);//��ֵ�˲�
	imshow("medianBlur", dstImage);//��ʾ���겨��ͼ��
}

//˫���˲�
void my_bilateralBlur()
{
	Mat element;
	Mat dstImage;
	namedWindow("bilateralBlur");//����һ������
	cvCreateTrackbar("bilateralBlur_element", "bilateralBlur", &bilateral_size, 50);//����һ�������������Ե����˲������Ӵ�С
	if (bilateral_size == 0 || bilateral_size == 1)//����˵Ĵ�С��Ҫ����1
		bilateral_size = 2;
	bilateralFilter(srcImage, dstImage, bilateral_size, bilateral_size * 2, bilateral_size / 2);//˫���˲�
	imshow("bilateralBlur", dstImage);//��ʾ���겨��ͼ��
}

//���ٴ���
void my_destroy_window()
{
	//����ı���ģʽ������Ҫ���ٴ���
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