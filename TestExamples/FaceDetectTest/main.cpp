#include "VisionFaceDetect/VisionFaceDetect.h"

#include "math.h"

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

inline cv::Rect Bbox2Square(const cv::Rect& rect, const cv::Mat& img) {
	cv::Rect result = rect;
	if (rect.width == rect.height)
	{
		return result;
	}

	float bbox_width = rect.width;
	float bbox_height = rect.height;
	float side = (std::max)(bbox_width, bbox_height);
	result.x += (bbox_width - side) * 0.5f;
	result.y += (bbox_height - side) * 0.5f;
	result.width = side;
	result.height = side;

	return result & cv::Rect(0, 0, img.cols, img.rows);
}

void EfficiencyTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	//face_detect->SetMinFaceSize(30);
	//face_detect->SetMaxWidthGlobal(960);

	cv::Mat frame = cv::imread("C:\\Users\\chendd14\\Desktop\\Tmp\\face_detect_lanqing_20180709.jpg");
	while (true)
	{
		if (!frame.empty())
		{
			// Perform Face Detect First
			double start = (double)cvGetTickCount();
			cv::Mat frame_show = frame.clone();
			std::vector<std::vector<cv::Point2f>> key_pts;
			std::vector<cv::Rect> faces = face_detect->GetFaces(frame, key_pts); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
			std::cout << faces.size() << " ";
			double stop = (double)cvGetTickCount();
			double time = (stop - start) / (cvGetTickFrequency() * 1000);
			std::cout << "face number: " << faces.size() << ", time cost: " << time << " ms." << std::endl;
			for (size_t i = 0; i < faces.size(); i++)
			{
				face_detect->DrawFaceRect(frame_show, faces[i]);
				for (size_t j = 0; j < key_pts[i].size(); j++)
				{
					cv::circle(frame_show, key_pts[i][j], 2, cv::Scalar(255, 0, 0), 2);
				}
			}

			cv::imshow("test", frame_show);
			if (cv::waitKey(1) == 27) {
				break;
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
}

void CameraTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	// face_detect->SetMaxWidthGlobal(1080);

	cv::VideoCapture cap(0);
	cv::Mat frame;
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> frame;
			if (!frame.empty())
			{
				// Perform Face Detect First
				double start = (double)cvGetTickCount();
				std::vector<vision::VisionFace> faces = face_detect->GetFaces(frame); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
				double stop = (double)cvGetTickCount();
				double time = (stop - start) / (cvGetTickFrequency() * 1000);
				std::cout << "time cost: " << time << " ms." << std::endl;

				for (size_t i = 0; i < faces.size(); i++)
				{
					face_detect->DrawFaceRect(frame, faces.at(i).GetBbox());
					for (size_t j = 0; j < faces[i].key_pts.size(); j++)
					{
						circle(frame, faces[i].key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
					}

					std::cout << "pitch: " << faces[i].head_pose[0] << std::endl;
					std::cout << "yaw  : " << faces[i].head_pose[1] << std::endl;
					std::cout << "roll : " << faces[i].head_pose[2] << std::endl;
				}
				cv::imshow("Face Detect Result", frame);
				if (cv::waitKey(1) == 27)
				{
					break;
				}
			}
		}
	}


	vision::destroyVisionFaceDetect(face_detect);
}

void ImageListTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect();

	int total_num = 0;
	int failed_num = 0;

	std::ifstream fin("G:\\Database\\FaceRecognition\\NIR-NIS\\NIR\\list.txt");
	std::ofstream fout("G:\\Database\\FaceRecognition\\NIR-NIS\\NIR\\list_labeled.txt");
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
					fout << image_name;
					total_num++;
					std::vector<vision::VisionFace> faces = face_detect->GetFaces(img);

					for (size_t i=0; i<faces.size(); i++)
					{
						fout << " " << faces[i].bbox.x << " " << faces[i].bbox.y << " " << faces[i].bbox.width << " " << faces[i].bbox.height;
					}
					fout << std::endl;

					if (faces.size() == 0)
					{
						failed_num++;
					}
					//for (size_t i = 0; i < faces.size(); i++)
					//{
					//	face_detect->DrawFaceRect(img, faces.at(i).GetBbox());
					//	for (size_t j = 0; j < faces[i].key_pts.size(); j++)
					//	{
					//		circle(img, faces[i].key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
					//	}
					//}
					//cv::imshow("Face Detect Result", img);
					//if (cv::waitKey(0) == 27)
					//{
					//	break;
					//}
				}
			}
		}
	}

	std::cout << failed_num << " / " << total_num << std::endl;
	fin.close();
	fout.close();
	vision::destroyVisionFaceDetect(face_detect);
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os;
	os << value;
	return os.str();
}

void ImageListTestTmp()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect();

	std::string dst_root = "C:\\Users\\chendd14\\Desktop\\Tmp\\NNIE_samples\\face_alignment\\";

	std::ifstream fin("C:\\Users\\chendd14\\Desktop\\Tmp\\samples\\list.txt");
	int idx = 0;
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
					idx++;
					std::vector<vision::VisionFace> faces = face_detect->GetMaximumFace(img);
					//if (faces.size() < 1)
					//{
					//	cv::imshow("test", img);
					//	cv::waitKey(0);
					//}
					for (size_t i = 0; i < faces.size(); i++)
					{
						cv::Rect face_rect = faces[i].bbox;
						face_rect = Bbox2Square(face_rect, img);

						std::string new_file_name = dst_root + to_string(idx) + ".jpg";
						cv::imwrite(new_file_name, img(face_rect));
						//face_detect->DrawFaceRect(img, faces.at(i).GetBbox());
						//for (size_t j = 0; j < faces[i].key_pts.size(); j++)
						//{
						//	circle(img, faces[i].key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
						//}
					}
					//cv::imshow("Face Detect Result", img);
					//if (cv::waitKey(0) == 27)
					//{
					//	break;
					//}
				}
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
}

int main(int argc, char** argv)
{
	ImageListTest();
	return 0;
}