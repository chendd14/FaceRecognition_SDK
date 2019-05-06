#include "VisionFaceDetect.h"
#include "VisionFaceAlignment.h"

#include <vector>
#include <fstream>

using namespace vision;
using namespace cv;
using namespace std;

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG
#endif

void EfficiencyTest(std::string image_name) {
    vision::VisionFaceDetect *   face_detect = vision::instantiateVisionFaceDetect();    // 包含默认初始化参数，无需再次初始化
    vision::VisionFaceAlignment *face_align  = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化

    cv::Mat test_img = cv::imread(image_name);
    double  start, stop, time;
    while (true) {
        if (!test_img.empty()) {
            cv::Mat img = test_img.clone();
            // Perform Face Detect First
            std::vector<vision::VisionFace> faces = face_detect->GetFaces(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
            for (size_t i = 0; i < faces.size(); i++) {
                start                            = (double)cvGetTickCount();
                std::vector<cv::Point2f> key_pts = face_align->GetKeyPoints(img, faces[i].bbox);
                stop                             = (double)cvGetTickCount();
                time                             = (stop - start) / (cvGetTickFrequency() * 1000);
                std::cout << "time cost: " << time << " ms." << std::endl;

                face_detect->DrawFaceRect(img, faces[i].bbox);
                for (size_t j = 0; j < key_pts.size(); j++) {
                    cv::circle(img, key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
                }
            }

            cv::imshow("Face Alignment Result", img);
            if (cv::waitKey(1) == 27) {
                break;
            }
        }
    }

    vision::destroyVisionFaceDetect(face_detect);
    vision::destroyVisionFaceAlignment(face_align);
}

void CameraTest(int camera_id) {
    vision::VisionFaceDetect *   face_detect = vision::instantiateVisionFaceDetect();    // 包含默认初始化参数，无需再次初始化
    vision::VisionFaceAlignment *face_align  = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化

    cv::VideoCapture cap(camera_id);
    cv::Mat          img;
    double           start, stop, time;
    if (cap.isOpened()) {
        while (true) {
            cap >> img;
            if (!img.empty()) {
                // Perform Face Detect First
                std::vector<vision::VisionFace> faces = face_detect->GetFaces(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的

                for (size_t i = 0; i < faces.size(); i++) {
                    start                            = (double)cvGetTickCount();
                    std::vector<cv::Point2f> key_pts = face_align->GetKeyPoints(img, faces[i].bbox);
                    stop                             = (double)cvGetTickCount();
                    time                             = (stop - start) / (cvGetTickFrequency() * 1000);
                    std::cout << "time cost: " << time << " ms." << std::endl;

                    face_detect->DrawFaceRect(img, faces.at(i).bbox);
                    for (size_t j = 0; j < key_pts.size(); j++) {
                        circle(img, key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
                    }
                }
                cv::imshow("Face Alignment Result", img);
                if (cv::waitKey(1) == 27) {
                    break;
                }
            }
        }
    }

    vision::destroyVisionFaceDetect(face_detect);
    vision::destroyVisionFaceAlignment(face_align);
}

int main(int argc, char **argv) {
    if (argc == 3)
    {
        std::string license_file = std::string(argv[1]);
		int camera_id = 0;
		sscanf_s(argv[2], "%d", &camera_id);

		vision::SetLicenseFile(license_file);
		CameraTest(camera_id);
    } else {
		std::cout << "Usage: FaceAlignmentTest license_file camera_id" << std::endl;
    }
    return 0;
}