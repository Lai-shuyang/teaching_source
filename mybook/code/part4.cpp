/*******************************************************************
***************************����˵��*********************************
���������canny��Sobel��Scharr��Laplace���ӵı�Ե����㷨�Լ�
�̶���ֵ������������Ӧ��ֵ��������
Դͼ�񴰿ڡ�video���л�������mode��ʾ��ͬ�ı�Ե����㷨����ֵ���㷨��
���У�0-3���ĸ���Ե����㷨��4-5��������ֵ���������ڽ���4-5ģʽ������
ʱ����Ҫ�ڳ���Ĺ���Ŀ¼�·���ͼƬpart4.jpg����������޷��������С�
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

void my_canny();//canny��Ե����㷨
void my_Sobel();//Sobel����
void my_Scharr();//Scharr����
void my_Laplace();//Laplace����
void my_threshold();//�̶���ֵ������
void my_adaptiveThreshold();//����Ӧ��ֵ������
void my_destroy_window();//���ٴ���

int main()
{
	capture.open(0); //������ͷ
	if (!capture.isOpened())//�����Ƶ�����������򷵻�
	{
		cout << "����ͷ��ʧ�ܣ�" << endl;
		return 0;
	}
	while (1)
	{
		double fps;//����֡��
		double t = 0;
		t = cv::getTickCount();

		if (mode >= 0 && mode <= 3)
		{
			capture >> srcImage;//�ȼ��� capture.read(srcImage);
			if (srcImage.empty())//���ĳ֡Ϊ�����˳�ѭ��
			{
				cout << "����ͷ�Ͽ���" << endl;
				return 1;
			}
		}
		else
		{
			srcImage = imread("part4.jpg");
			if (srcImage.empty())
			{
				cout << "�޷���ͼƬ��" << endl;
				return 1;
			}
		}

		namedWindow("video");
		createTrackbar("mode", "video", &mode, 5);//��������ʾ��ͬ��mode
		my_destroy_window();//���ٴ���

		//����ģʽ���в����ж�
		switch (mode)
		{
		case 0:
			my_canny();//canny��Ե����㷨
			break;
		case 1:
			my_Sobel();//my_Sobel����
			break;
		case 2:
			my_Scharr();//Scharr����
			break;
		case 3:
			my_Laplace();//Laplace����
			break;
		case 4:
			my_threshold();//�̶���ֵ������
			break;
		case 5:
			my_adaptiveThreshold();//����Ӧ��ֵ������
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

//canny��Ե����㷨
void my_canny()
{
	//�������ںͻ�����
	namedWindow("canny");
	createTrackbar("g_canny", "canny", &value_canny, 100);

	//����canny��Ե���
	Mat grayImage, imgCanny;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	Canny(grayImage, imgCanny, value_canny, 3 * value_canny, 3);//����һ��canny��Ե���
	imshow("canny", imgCanny);
}

//Sobel����
void my_Sobel()
{
	Mat grayImage,dstImage;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
	
	//�� X�����ݶ�
	Sobel(grayImage, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);//ʹ�����Ա任ת����������Ԫ�س�8λ�޷�������
	imshow("X����Sobel", abs_grad_x);

	//��Y�����ݶ�
	Sobel(grayImage, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("Y����Sobel", abs_grad_y);
		
	//�ϲ��ݶ�(����)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dstImage);
	imshow("���巽��Sobel", dstImage);
}

//Scharr����
void my_Scharr()
{
	Mat grayImage, dstImage;
	GaussianBlur(srcImage, srcImage, Size(5, 5), 0, 0, BORDER_DEFAULT);
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);//��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grad_x, grad_y, abs_grad_x, abs_grad_y;

	//�� X�����ݶ�
	Scharr(grayImage, grad_x, CV_16S, 1, 0);
	convertScaleAbs(grad_x, abs_grad_x);//ʹ�����Ա任ת����������Ԫ�س�8λ�޷�������
	imshow("X����Scharr", abs_grad_x);

	//��Y�����ݶ�
	Scharr(grayImage, grad_y, CV_16S, 0, 1);
	convertScaleAbs(grad_y, abs_grad_y);
	imshow("Y����Scharr", abs_grad_y);

	//�ϲ��ݶ�(����)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dstImage);
	imshow("���巽��Scharr", dstImage);
}

//Laplace����
void my_Laplace()
{
	Mat grayImage, midImage, dstImage;
	//ת��Ϊ�Ҷ�ͼ
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//��˹�˲�
	GaussianBlur(grayImage, midImage, Size(3, 3), 0, 0);
	//��Ե���
	Laplacian(midImage, dstImage, CV_16S, 3, 1, 0);
	Mat lastImage;
	convertScaleAbs(dstImage, lastImage);
	imshow("Laplace", lastImage);
}

//�̶���ֵ������
void my_threshold()
{
	//�������ںͻ�����
	namedWindow("threshold");
	createTrackbar("thresh", "threshold", &thresh, 255);
	createTrackbar("threshold_type", "threshold", &threshold_type, 4);

	Mat grayImage,dstImage;
	//ת��Ϊ�Ҷ�ͼ
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//��ֵ��
	threshold(grayImage, dstImage, thresh, 255, threshold_type);
	imshow("threshold", dstImage);
}

//����Ӧ��ֵ������
void my_adaptiveThreshold()
{
	//�������ںͻ�����
	namedWindow("adaptiveThreshold");
	createTrackbar("threshold_type", "adaptiveThreshold", &threshold_type, 1);
	createTrackbar("adaptive_type", "adaptiveThreshold", &adaptive_type, 1);
	createTrackbar("C", "adaptiveThreshold", &C, 20);
	if (threshold_type > 1)
		threshold_type = 1;

	Mat grayImage, dstImage;
	//ת��Ϊ�Ҷ�ͼ
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	//��˹�˲�
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	//��ֵ��
	adaptiveThreshold(grayImage, dstImage, 255, adaptive_type, threshold_type, 5, C);
	imshow("adaptiveThreshold", dstImage);
}

//���ٴ���
void my_destroy_window()
{
	//����ı���ģʽ������Ҫ���ٴ���
	if (last_mode != mode)
	{
		if (last_mode == 0)
		{
			destroyWindow("canny");
		}
		else if (last_mode == 1)
		{
			destroyWindow("X����Sobel");
			destroyWindow("Y����Sobel");
			destroyWindow("���巽��Sobel");
		}
		else if (last_mode == 2)
		{
			destroyWindow("X����Scharr");
			destroyWindow("Y����Scharr");
			destroyWindow("���巽��Scharr");
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