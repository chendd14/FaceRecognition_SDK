#include "VisionFaceDetect.h"

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

void CameraTest() {
    vision::VisionFaceDetect *face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
    // face_detect->SetMaxWidthGlobal(1080);

    cv::VideoCapture cap(0);
    cv::Mat          frame;
    if (cap.isOpened()) {
        while (true) {
            cap >> frame;
            if (!frame.empty()) {
                // Perform Face Detect First
                double                          start = (double)cvGetTickCount();
                std::vector<vision::VisionFace> faces = face_detect->GetFaces(frame); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的
                double                          stop  = (double)cvGetTickCount();
                double                          time  = (stop - start) / (cvGetTickFrequency() * 1000);
                std::cout << "time cost: " << time << " ms." << std::endl;

                for (size_t i = 0; i < faces.size(); i++) {
                    face_detect->DrawFaceRect(frame, faces.at(i).GetBbox());
                    for (size_t j = 0; j < faces[i].key_pts.size(); j++) {
                        circle(frame, faces[i].key_pts[j], 2, cv::Scalar(255, 0, 0), 2);
                    }

                    std::cout << "pitch: " << faces[i].head_pose[0] << std::endl;
                    std::cout << "yaw  : " << faces[i].head_pose[1] << std::endl;
                    std::cout << "roll : " << faces[i].head_pose[2] << std::endl;
                }
                cv::imshow("Face Detect Result", frame);
                if (cv::waitKey(1) == 27) {
                    break;
                }
            }
        }
    }

    vision::destroyVisionFaceDetect(face_detect);
}

int main(int argc, char **argv) {
    vision::SetLicenseFile("d:/dip/license.lic");
    CameraTest();
    return 0;
}