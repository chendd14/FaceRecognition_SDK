#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceFeature/VisionFaceFeature.h"
#include "VisionFaceAlignment/VisionFaceAlignment.h"
#include "VisionFaceCompare/VisionFaceCompare.h"

#include <iostream>
#include <fstream>

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // ����Ĭ�ϳ�ʼ�������������ٴγ�ʼ��
vision::VisionFaceAlignment* face_alignment = vision::instantiateVisionFaceAlignment(); // ����Ĭ�ϳ�ʼ�������������ٴγ�ʼ��
vision::VisionFaceFeature* face_feature = vision::instantiateVisionFaceFeature(); // ��Բ�ͬ��ʹ�ó�������Ҫ���ض���ģ�ͽ��г�ʼ��

cv::Mat GetNormFace(std::string image_name)
{
	cv::Mat norm_face;
	cv::Mat img_tmp = cv::imread(image_name);
	std::vector<vision::VisionFace> faces_tmp = face_detect->GetMaximumFace(img_tmp); // ע�⣬��ʱVisionFace�����г��˼���������ؼ��㣬������������Ч��
	if (faces_tmp.size() < 1)
	{
		std::cout << "Failed to detect a face in " << image_name << ", exit." << std::endl;
		return norm_face;
	}
	norm_face = face_alignment->GetAlignedFace(img_tmp, faces_tmp.at(0).GetBbox());
	return norm_face;
}

int main(int argc, char** argv)
{
	//if (!face_feature->InitV3("D:\\GitLab\\VisionFaceProject\\Resources\\models\\facerog_128_tracking100000_20180918.param"))
	//{
	//	std::cout << "Failed to init face feature, please check model path." << std::endl;
	//	exit(-1);
	//}

	std::cout << "Feature dim: " << face_feature->GetFeatureDim() << std::endl;
	vision::VisionFaceCompare* face_compare = vision::instantiateVisionFaceCompare();

	int NUMBER = 10; // ��������ͼƬ��
	int FEATURE_DIM = face_feature->GetFeatureDim();
	// ΪDatabase����ռ�
	face_compare->Init(FEATURE_DIM, NUMBER);

	std::string image_root_folder = "../../Resources/test_images/";

	std::vector<std::string> images;
	std::ifstream fin(image_root_folder + "reglist.txt");
	if (!fin.is_open())
	{
		std::cout << "Failed to open file " << image_root_folder + "reglist.txt" << std::endl;
	}
	std::string image_name_tmp;

	while (!fin.eof())
	{
		std::getline(fin, image_name_tmp);
		if (image_name_tmp != "")
		{
			images.push_back(image_name_tmp);
		}
	}

	for (int i = 0; i < images.size(); i++)
	{
		// һ������£������ͼƬ�Ѿ��Ǿ������͹�һ������
		cv::Mat norm_face_tmp = GetNormFace(image_root_folder + images.at(i));
		if (!norm_face_tmp.empty())
		{
			std::vector<float> feature_tmp = face_feature->GetFeature(norm_face_tmp);
			// ����������д��Database
			std::cout << face_compare->Add(feature_tmp) << std::endl;
		}
	}

	// �������ڲ�ѯ��feature
	cv::Mat norm_face_tmp = GetNormFace(image_root_folder + "face_rog_query.jpg");
	std::vector<float> tmp = face_feature->GetFeature(norm_face_tmp);

	std::vector<vision::VisionFaceCompareResult> result = face_compare->GetResult(tmp, 5);

	for (int i = 0; i < result.size(); i++)
	{
		std::cout << "Recognition result " << i << " : " <<  result.at(i).idx << " - " << result.at(i).score << std::endl;
	}

	cv::waitKey(0);

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_alignment);
	vision::destroyVisionFaceFeature(face_feature);
	vision::destroyVisionFaceCompare(face_compare);
	return 0;
}