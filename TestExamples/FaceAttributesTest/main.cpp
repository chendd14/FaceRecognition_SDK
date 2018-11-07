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
			cout << "女" << endl;
		else
			cout << "男" << endl;

		cout << "年龄" << results[1] << endl;
	}
	else
	{
		if (results[0] == 0)
			cout << "女" << endl;
		else
			cout << "男" << endl;
		if (results[1] == 0)
			cout << "幼" << endl;
		else if (results[1] == 1)
			cout << "少" << endl;
		else if (results[1] == 2)
			cout << "青" << endl;
		else if (results[1] == 3)
			cout << "壮" << endl;
		else if (results[1] == 4)
			cout << "中" << endl;
		else if (results[1] == 5)
			cout << "中老" << endl;
		else if (results[1] == 6)
			cout << "老" << endl;
		else
			cout << "未知" << endl;

		if (results[2] == 0)
			cout << "无眼镜" << endl;
		else if (results[2] == 1)
			cout << "戴眼镜" << endl;
		else
			cout << "戴墨镜" << endl;

		if (results[3] == 0)
			cout << "黄" << endl;
		else if (results[3] == 1)
			cout << "白" << endl;
		else if (results[3] == 2)
			cout << "黑" << endl;
		else
			cout << "无法判断" << endl;

		cout << "颜值" << results[4] << endl;

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
			cout << "没有微笑" << endl;
		else if (results[6] == 1)
			cout << "微笑" << endl;
		else
			cout << "无法判断" << endl;

		if (results[7] == 0)
			cout << "无口罩" << endl;
		else if (results[7] == 1)
			cout << "戴口罩" << endl;
		else
			cout << "无法判断" << endl;

		cout << "年龄" << results[8] << endl;
	}
}

void CameraTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
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
				std::vector<vision::VisionFace> faces = face_detect->GetMaximumFace(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的

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