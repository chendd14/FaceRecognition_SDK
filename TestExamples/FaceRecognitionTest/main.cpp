#include "VisionFaceDetect.h"
#include "VisionFaceFeature.h"
#include "VisionFaceAlignment.h"
#include "VisionFaceCompare.h"

#include <iostream>
#include <fstream>

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG
#endif

vision::VisionFaceDetect *   face_detect    = nullptr;
vision::VisionFaceAlignment *face_alignment = nullptr;
vision::VisionFaceFeature *  face_feature   = nullptr;

cv::Mat GetNormFace(std::string image_name) {
    cv::Mat                         norm_face;
    cv::Mat                         img_tmp   = cv::imread(image_name);
    std::vector<vision::VisionFace> faces_tmp = face_detect->GetMaximumFace(img_tmp); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
    if (faces_tmp.size() < 1) {
        std::cout << "Failed to detect a face in " << image_name << ", exit." << std::endl;
        return norm_face;
    }
    norm_face = face_alignment->GetAlignedFace(img_tmp, faces_tmp.at(0).GetBbox());
    return norm_face;
}

int main(int argc, char **argv) {
    if (argc == 6) {
        std::string license_file           = std::string(argv[1]);
        std::string model_name             = std::string(argv[2]);
        std::string test_image_root_folder = std::string(argv[3]);
        std::string test_image_list        = std::string(argv[4]);
        std::string query_image_name       = std::string(argv[5]);

        vision::SetLicenseFile(license_file);

        face_detect    = vision::instantiateVisionFaceDetect();    // 包含默认初始化参数，无需再次初始化
        face_alignment = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
        face_feature   = vision::instantiateVisionFaceFeature();   // 针对不同的使用场景，需要用特定的模型进行初始化

        if (!face_feature->InitV3(model_name)) {
            std::cout << "Failed to init face feature, please check model path." << std::endl;
            exit(-1);
        }

        vision::VisionFaceCompare *face_compare = vision::instantiateVisionFaceCompare();

        std::vector<std::string> images;
        std::ifstream            fin(test_image_list);
        if (!fin.is_open()) {
            std::cout << "Failed to open file " << test_image_list << std::endl;
        }

        std::string image_name_tmp;
        while (!fin.eof()) {
            std::getline(fin, image_name_tmp);
            if (image_name_tmp != "") {
                images.push_back(image_name_tmp);
            }
        }

        std::vector<float> face_feature3;
        for (int i = 0; i < images.size(); i++) {
            // 一般情况下，处理的图片已经是经过检测和归一化处理
            cv::Mat norm_face_tmp = GetNormFace(test_image_root_folder + images.at(i));
            if (!norm_face_tmp.empty()) {
                std::vector<float> feature_tmp = face_feature->GetFeature(norm_face_tmp);
                // 将人脸特征写入Database
                if (face_compare->Add(feature_tmp) == -1) {
                    std::cout << "Failed to add feature for " << test_image_root_folder + images.at(i) << std::endl;
                }
            }
        }

        // 构建用于查询的feature
        cv::Mat            norm_face_tmp = GetNormFace(query_image_name);
        std::vector<float> tmp           = face_feature->GetFeature(norm_face_tmp);

        std::vector<vision::VisionFaceCompareResult> result = face_compare->GetResult(tmp, images.size());

        for (int i = 0; i < result.size(); i++) {
            std::cout << "Recognition result " << i << " : " << result.at(i).idx << " - " << result.at(i).score << std::endl;
        }

        vision::destroyVisionFaceDetect(face_detect);
        vision::destroyVisionFaceAlignment(face_alignment);
        vision::destroyVisionFaceFeature(face_feature);
        vision::destroyVisionFaceCompare(face_compare);
    } else {
        std::cout << "Usage: FaceRecognitionTest license_file model_name test_image_root_folder test_image_list query_image_name" << std::endl;
    }
    return 0;
}