#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceAlignment/VisionFaceAlignment.h"
#include "VisionFaceFeature/VisionFaceFeature.h"
#include "VisionFaceCompare/VisionFaceCompare.h"

#define USE_DETECT_KEYPTS

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

int main(int argc, char** argv)
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceAlignment* face_alignment = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceFeature* face_feature = vision::instantiateVisionFaceFeature(); // 针对不同的使用场景，需要用特定的模型进行初始化
	face_feature->Init("models/1s_combined_20180402.json");
	vision::VisionFaceCompare* face_compare = vision::instantiateVisionFaceCompare();

	std::string image_root_folder = "";
	std::string image1_name = image_root_folder + "2.jpg";
	std::string image2_name = image_root_folder + "3.jpg";

	cv::Mat img1 = cv::imread(image1_name);
	cv::Mat img2 = cv::imread(image2_name);

	// Perform Face Detect First
#ifdef USE_DETECT_KEYPTS
	std::vector<std::vector<cv::Point2f>> key_pts1;
	std::vector<cv::Rect> faces1 = face_detect->GetMaximumFace(img1, key_pts1);
#else
	std::vector<vision::VisionFace> faces1 = face_detect->GetMaximumFace(img1); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
#endif
	if (faces1.size() < 1)
	{
		std::cout << "Failed to detect a face in " << image1_name << ", exit." << std::endl;
		exit(-1);
	}
#ifdef USE_DETECT_KEYPTS
	std::vector<std::vector<cv::Point2f>> key_pts2;
	std::vector<cv::Rect> faces2 = face_detect->GetMaximumFace(img2, key_pts2);
#else
	std::vector<vision::VisionFace> faces2 = face_detect->GetMaximumFace(img2);
#endif
	if (faces2.size() < 1)
	{
		std::cout << "Failed to detect a face in " << image2_name << ", exit." << std::endl;
		exit(-1);
	}

	// Face Alignment
#ifdef USE_DETECT_KEYPTS
	std::vector<cv::Mat> norm_face1 = face_alignment->GetAlignedFace(img1, key_pts1, false);
	std::vector<cv::Mat> norm_face2 = face_alignment->GetAlignedFace(img2, key_pts2, false);
	cv::imshow("norm1", norm_face1[0]);
	cv::imshow("norm2", norm_face2[0]);
#else
	cv::Mat norm_face1 = face_alignment->GetAlignedFace(img1, faces1.at(0).GetBbox());
	cv::Mat norm_face2 = face_alignment->GetAlignedFace(img2, faces2.at(0).GetBbox());
	cv::imshow("norm1", norm_face1);
	cv::imshow("norm2", norm_face2);
#endif


	// Extract Face Feature
#ifdef USE_DETECT_KEYPTS
	std::vector<float> fea1 = face_feature->GetFeature(norm_face1[0]);
	std::vector<float> fea2 = face_feature->GetFeature(norm_face2[0]);
#else
	std::vector<float> fea1 = face_feature->GetFeature(norm_face1);
	std::vector<float> fea2 = face_feature->GetFeature(norm_face2);
#endif

	std::cout << "======================" << std::endl;
	for (size_t i = 0; i < fea1.size(); ++i)
		std::cout << fea1[i] << ' ';
	std::cout << "\n======================" << std::endl;

	// Feature Comapre
	std::cout << "Similarity: " << face_compare->GetScore(fea1, fea2) << std::endl;

#ifdef USE_DETECT_KEYPTS
	face_detect->DrawFaceRect(img1, faces1[0]);
	face_detect->DrawFaceRect(img2, faces2[0]);
#else
	face_detect->DrawFaceRect(img1, faces1.at(0).GetBbox());
	face_detect->DrawFaceRect(img2, faces2.at(0).GetBbox());
#endif
	cv::imshow("test1", img1);
	cv::imshow("test2", img2);
	cv::waitKey(0);

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_alignment);
	vision::destroyVisionFaceFeature(face_feature);
	vision::destroyVisionFaceCompare(face_compare);
	return 0;
}