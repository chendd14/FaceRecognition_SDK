#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceAlignment/VisionFaceAlignment.h"

#include <vector>
#include <fstream>

using namespace vision;
using namespace cv;
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

void EfficiencyTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化

	cv::Mat test_img = cv::imread("test.jpg");
	double start, stop, time;
	while (true)
	{
		if (!test_img.empty())
		{
			cv::Mat img = test_img.clone();
			// Perform Face Detect First
			std::vector<vision::VisionFace> faces = face_detect->GetFaces(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
			for (size_t i = 0; i < faces.size(); i++)
			{
				start = (double)cvGetTickCount();
				std::vector<cv::Point2f> key_pts = face_align->GetKeyPoints(img, faces[i].bbox);
				stop = (double)cvGetTickCount();
				time = (stop - start) / (cvGetTickFrequency() * 1000);
				std::cout << "time cost: " << time << " ms." << std::endl;

				face_detect->DrawFaceRect(img, faces[i].bbox);
				for (size_t j = 0; j < key_pts.size(); j++)
				{
					cv::circle(img, key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
				}
			}

			cv::imshow("Face Alignment Result", img);
			if (cv::waitKey(1) == 27)
			{
				break;
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_align);
}

void CameraTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
	face_align->Init("models/algn_106pts_rd2_iter_1000000.json");

	cv::VideoCapture cap(1);
	cv::Mat img;
	double start, stop, time;
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> img;
			if (!img.empty())
			{
				// Perform Face Detect First
				std::vector<vision::VisionFace> faces = face_detect->GetFaces(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的

				for (size_t i = 0; i < faces.size(); i++)
				{
					start = (double)cvGetTickCount();
					std::vector<cv::Point2f> key_pts = face_align->GetKeyPoints(img, faces[i].bbox);
					stop = (double)cvGetTickCount();
					time = (stop - start) / (cvGetTickFrequency() * 1000);
					std::cout << "time cost: " << time << " ms." << std::endl;

					face_detect->DrawFaceRect(img, faces.at(i).bbox);
					for (size_t j = 0; j < key_pts.size(); j++)
					{
						circle(img, key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
					}
				}
				cv::imshow("Face Alignment Result", img);
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

void ImageListTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect();
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化

	std::ifstream fin("F:\\Database\\FaceDatabase\\FaceDetect\\HY_detect_failed_20180607\\list.txt");
	double start, stop, time;
	if (fin.is_open())
	{
		std::string image_name = "";
		while (!fin.eof())
		{
			getline(fin, image_name);
			if (image_name != "")
			{
				cv::Mat img = cv::imread(image_name);
				if (!img.empty())
				{
					std::vector<vision::VisionFace> faces = face_detect->GetFaces(img);
					for (size_t i = 0; i < faces.size(); i++)
					{
						start = (double)cvGetTickCount();
						std::vector<cv::Point2f> key_pts = face_align->GetKeyPoints(img, faces[i].bbox);
						stop = (double)cvGetTickCount();
						time = (stop - start) / (cvGetTickFrequency() * 1000);
						std::cout << "time cost: " << time << " ms." << std::endl;

						face_detect->DrawFaceRect(img, faces.at(i).GetBbox());
						for (size_t j = 0; j < key_pts.size(); j++)
						{
							circle(img, key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
						}

						for (size_t j = 0; j < faces[i].key_pts.size(); j++)
						{
							circle(img, faces[i].key_pts[j], 2, cv::Scalar(0, 255, 0), 2);
						}
					}
					cv::imshow("Face Detect Result", img);
					if (cv::waitKey(0) == 27)
					{
						break;
					}
				}
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_align);
}

int main(int argc, char** argv)
{
	EfficiencyTest();
	return 0;
}