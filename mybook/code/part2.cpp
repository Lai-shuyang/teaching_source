/*******************************************************************
***************************����˵��*********************************
������������ַ���ͼ�����ص�ķ�ʽ��Դͼ�񴰿ڡ�video���л�������
mode��ʾ��ͬ�ķ��ʷ��������ע��Ƚ�һ�����ַ�ʽ��֡�ʴ�С��0-2�ֱ��
��ʹ��at�������ص㡢ʹ��ָ��������ص㡢�������������ַ�ʽ��
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

void Image_white_1();//ʹ��at�������ص�
void Image_white_2();//ʹ��ָ��������ص�
void Image_white_3();//����������

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
		createTrackbar("mode", "video", &mode, 2);//��������ʾ��ͬ��mode

		//����ģʽ���в����ж�
		switch (mode)
		{
		case 0:
			Image_white_1();//ʹ��at�������ص�
			break;
		case 1:
			Image_white_2();//ʹ��ָ��������ص�
			break;
		case 2:
			Image_white_3();//����������
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

//ʹ��at�������ص�
void Image_white_1()
{
	int row = srcImage.rows;	//��ȡ�������
	int col = srcImage.cols;	//��ȡ�������
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			srcImage.at<cv::Vec3b>(i, j)[0] = 255;	//�ѵ�һ��ͨ������ɫ���������
			srcImage.at<cv::Vec3b>(i, j)[1] = 255;	//�ѵڶ���ͨ������ɫ���������
			srcImage.at<cv::Vec3b>(i, j)[2] = 255;	//�ѵ�����ͨ������ɫ���������
		}
	}
}

//ʹ��ָ��������ص�
void Image_white_2()
{
	int row = srcImage.rows;	//��ȡ�������
	int col = srcImage.cols;	//��ȡ�������
	int nStep = srcImage.cols * srcImage.channels();//�����ܹ���Ҫ���������ص�ĸ���
	for (int i = 0; i < row; i++)
	{
		uchar *Data = srcImage.ptr<uchar>(i);
		for (int j = 0; j < nStep; j++)
		{
			Data[j] = saturate_cast<uchar>(255);
		}
	}
}

//����������
void Image_white_3()
{
	MatIterator_<Vec3b> IterStart = srcImage.begin<Vec3b>();	//��ʾָ�����������ʼλ��
	MatIterator_<Vec3b> IterEnd = srcImage.end<Vec3b>();		//��ʾָ�����������ֹλ��
	while (IterStart != IterEnd)
	{
		(*IterStart)[0] = 255;	//�ѵ�һ��ͨ������ɫ���������
		(*IterStart)[1] = 255;	//�ѵڶ���ͨ������ɫ���������
		(*IterStart)[2] = 255;	//�ѵ�����ͨ������ɫ���������
		IterStart++;
	}
}