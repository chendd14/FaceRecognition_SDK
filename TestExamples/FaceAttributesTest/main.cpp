#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceAlignment/VisionFaceAlignment.h"
#include "VisionFaceAttributes/VisionFaceAttributes.h"

#include <vector>
#include <iostream>

using namespace vision;
using namespace std;
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

void ShowResult(vector<int> results)
{
	if (results.size() == 2)
	{
		if (results[0] == 0)
			cout << "Ů" << endl;
		else
			cout << "��" << endl;

		cout << "����" << results[1] << endl;
	}
	else
	{
		if (results[0] == 0)
			cout << "Ů" << endl;
		else
			cout << "��" << endl;
		if (results[1] == 0)
			cout << "��" << endl;
		else if (results[1] == 1)
			cout << "��" << endl;
		else if (results[1] == 2)
			cout << "��" << endl;
		else if (results[1] == 3)
			cout << "׳" << endl;
		else if (results[1] == 4)
			cout << "��" << endl;
		else if (results[1] == 5)
			cout << "����" << endl;
		else if (results[1] == 6)
			cout << "��" << endl;
		else
			cout << "δ֪" << endl;

		if (results[2] == 0)
			cout << "���۾�" << endl;
		else if (results[2] == 1)
			cout << "���۾�" << endl;
		else
			cout << "��ī��" << endl;

		if (results[3] == 0)
			cout << "��" << endl;
		else if (results[3] == 1)
			cout << "��" << endl;
		else if (results[3] == 2)
			cout << "��" << endl;
		else
			cout << "�޷��ж�" << endl;

		cout << "��ֵ" << results[4] << endl;

		if (results[5] == 0)
			cout << "Suprise" << endl;
		else if (results[5] == 1)
			cout << "Fear" << endl;
		else if (results[5] == 2)
			cout << "Disgust" << endl;
		else if (results[5] == 3)
			cout << "Happiness" << endl;
		else if (results[5] == 4)
			cout << "Sadness" << endl;
		else if (results[5] == 5)
			cout << "Anger" << endl;
		else if (results[5] == 6)
			cout << "Netural" << endl;

		if (results[6] == 0)
			cout << "û��΢Ц" << endl;
		else if (results[6] == 1)
			cout << "΢Ц" << endl;
		else
			cout << "�޷��ж�" << endl;

		if (results[7] == 0)
			cout << "�޿���" << endl;
		else if (results[7] == 1)
			cout << "������" << endl;
		else
			cout << "�޷��ж�" << endl;

		cout << "����" << results[8] << endl;
	}
}

void CameraTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // ����Ĭ�ϳ�ʼ�������������ٴγ�ʼ��
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // ����Ĭ�ϳ�ʼ�������������ٴγ�ʼ��
	VisionFaceAttributes *face_attributes = instantiateVisionFaceAttributes();
	VisionFaceAttributes *face_attributes1 = instantiateVisionFaceAttributes();
	
	//face_attributes->Init("models/face_attributes_size_10_crop_30_50_120_140_180329.json");
	//face_attributes->Init("models/Face_Attr_0404_iter_2000000.json");
	//face_attributes1->Init("models/face_attributes_1s_only_gender_and_age_180427.json");

	cv::VideoCapture cap(0);
	cv::Mat img;
	double start, stop, time;
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> img;
			//img = cv::imread("C:\\Users\\chendd14\\Desktop\\Tmp\\jiadu4.jpg");
			if (!img.empty())
			{
				// Perform Face Detect First
				std::vector<vision::VisionFace> faces = face_detect->GetMaximumFace(img); // ע�⣬��ʱVisionFace�����г��˼���������ؼ��㣬������������Ч��

				if (faces.size() == 1)
				{
					cv::Mat norm_face = face_align->GetAlignedFace(img, faces[0].bbox);
					std::vector<int> results = face_attributes->GetAttributes(norm_face);
					std::vector<int> results1 = face_attributes1->GetAttributes(norm_face);
					//results[0] = results1[0];
					//results[8] = results1[1];

					ShowResult(results);

					face_detect->DrawFaceRect(img, faces[0].bbox);
				}
				cv::imshow("Test", img);
				if (cv::waitKey(1) == 27)
				{
					break;
				}
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_align);
}

int main()
{
	CameraTest();
	return 0;
	Mat im = imread("demo.jpg");
	VisionFaceAttributes *face_attributes = instantiateVisionFaceAttributes();
	face_attributes->Init("models/Face_Attr_0404_iter_2000000.json");
	vector<int> results = face_attributes->GetAttributes(im);
	ShowResult(results);
	imshow("Test", im);
	waitKey();
}