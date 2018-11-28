#include "VisionFaceDetect.h"
#include "VisionFaceAlignment.h"
#include "VisionFaceFeature.h"

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG
#endif

int main(int argc, char **argv) {
    if (argc == 4) {
        std::string recognition_model = std::string(argv[1]);
        std::string image1_name       = std::string(argv[2]);
        std::string image2_name       = std::string(argv[3]);

        vision::VisionFaceDetect *   face_detect    = vision::instantiateVisionFaceDetect();    // 包含默认初始化参数，无需再次初始化
        vision::VisionFaceAlignment *face_alignment = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
        vision::VisionFaceFeature *  face_feature   = vision::instantiateVisionFaceFeature();   // 针对不同的使用场景，需要用特定的模型进行初始化
        face_feature->InitV3(recognition_model);

        cv::Mat img1 = cv::imread(image1_name);
        cv::Mat img2 = cv::imread(image2_name);

        // Perform Face Detect First
        std::vector<vision::VisionFace> faces1 = face_detect->GetMaximumFace(img1);
        if (faces1.size() < 1) {
            std::cout << "Failed to detect a face in " << image1_name << ", exit." << std::endl;
            exit(-1);
        } else {
            std::cout << "Face rectangle in " << image1_name << " is: " << faces1[0].bbox << std::endl;
        }

        std::vector<vision::VisionFace> faces2 = face_detect->GetMaximumFace(img2);
        if (faces2.size() < 1) {
            std::cout << "Failed to detect a face in " << image2_name << ", exit." << std::endl;
            exit(-1);
        } else {
            std::cout << "Face rectangle in " << image2_name << " is: " << faces2[0].bbox << std::endl;
        }

        // Face Alignment
        cv::Mat norm_face1 = face_alignment->GetAlignedFace(img1, faces1.at(0).key_pts);
        cv::Mat norm_face2 = face_alignment->GetAlignedFace(img2, faces2.at(0).key_pts);

        // Extract Face Feature
        std::vector<float> fea1 = face_feature->GetFeature(norm_face1);
        std::vector<float> fea2 = face_feature->GetFeature(norm_face2);

        // Feature Comapre
        std::cout << "Similarity: " << face_feature->GetScore(fea1, fea2) << std::endl;

#ifdef SHOW_RESULT
        face_detect->DrawFaceRect(img1, faces1.at(0).GetBbox());
        face_detect->DrawFaceRect(img2, faces2.at(0).GetBbox());

        cv::imshow("test1", img1);
        cv::imshow("test2", img2);
        cv::waitKey(0);
#endif

		vision::destroyVisionFaceDetect(face_detect);
        vision::destroyVisionFaceAlignment(face_alignment);
        vision::destroyVisionFaceFeature(face_feature);
    } else {
        std::string cmd = std::string(argv[0]);
        std::cout << "Usage: " << cmd << " recognition_model image1 image2" << std::endl;
    }
    return 0;
}