#ifndef VISION_FACE_SINGLETON_H
#define VISION_FACE_SINGLETON_H

#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceAlignment/VisionFaceAlignment.h"
#include "VisionFaceFeature/VisionFaceFeature.h"
#include "SubModules/VisionFaceLiveCapture/VisionFaceLiveCapture.h"

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#ifdef VISIONFACE_EXPORTS
#define VISION_API __declspec(dllexport)
#else
#define VISION_API __declspec(dllimport)
#endif // VISION_API
#else  // defined (windows)
#define VISION_API
#endif

#ifdef __cplusplus // if used by C++ code
extern "C" {       // we need to export the C interface
#endif

enum VISION_STATE_CODE {
    VISION_COMPARE_SUCCEED          = 0,
    VISION_FACE_DETECT_FAILED       = 1,
    VISION_FACE_ALIGNMENT_FAILED    = 2,
    VISION_FACE_FEATURE_FAILED      = 3,
    VISION_CANNOT_FIND_LIVE_CAMERA  = 4,
    VISION_OPEN_LIVE_CAMERA_FAILED  = 5,
    VISION_OPEN_LIVE_CAMERA_SUCCEED = 6
};

// 图像需要时BGR 3通道的图像
int VISION_API VisionFaceCompare(int w1, int h1, uchar *data1, int w2, int h2, uchar *data2, float &score);
int VISION_API VisionLiveOpenCamera(int &img_w, int &img_h);
void VISION_API VisionLiveCloseCamera();
int VISION_API VisionLiveCapture(uchar *img_data, int &living_status, int &face_rect_x, int &face_rect_y, int &face_rect_w, int &face_rect_h);

#ifdef __cplusplus
}
#endif

namespace vision {
// void VISION_API GetErrorCode()

class VISION_API VisionFaceSingleton {
public:
    ~VisionFaceSingleton();
    bool InitFaceDetect(std::string face_detect_model = "models/face_detect.json");
    bool InitFaceAlignment(std::string face_align_model = "models/face_align.json");
    bool InitFaceFeature(std::string face_feature_model = "models/face_feature.json");

    std::vector<VisionFace>  GetMaximumFace(const cv::Mat &img);
    std::vector<cv::Point2f> GetKeyPoints(const cv::Mat &img, cv::Rect face_rect);
    cv::Mat                  GetNormalizedFace(const cv::Mat &img, std::vector<cv::Point2f> key_pts);
    std::vector<float>       GetFaceFeature(const cv::Mat &img);
    float                    GetScore(const std::vector<float> &fea1, const std::vector<float> &fea2);
    int                      LiveOpenCamera(int &img_w, int &img_h);
    void                     LiveCloseCamera();
    int                      LiveCapture(uchar *img_data, int &living_status, int &face_rect_x, int &face_rect_y, int &face_rect_w, int &face_rect_h);

    static VisionFaceSingleton *Get();

    std::string error_info_;

protected:
    static VisionFaceSingleton *face_singleton_;
    VisionFaceDetect *          face_detect_;
    VisionFaceAlignment *       face_align_;
    VisionFaceFeature *         face_feature_;
    VisionFaceLiveCapture *     live_capture_;

private:
    VisionFaceSingleton();
    int img_w_, img_h_, img_c_;
};
} // namespace vision
#endif // VISION_FACE_SINGLETON_H
