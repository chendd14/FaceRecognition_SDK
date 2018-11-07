/************************************************************************
 * @brief face detect use improved cnn cascade
 * @author chendd14@163.com
 * @date 2017-05-31
 * @copyright 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_DETECT_H
#define VISION_FACE_DETECT_H

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#ifdef VISIONFACE_EXPORTS
#define VISION_API __declspec(dllexport)
#else
#define VISION_API __declspec(dllimport)
#endif // VISION_API
#else  // defined (windows)
#define VISION_API
#endif

#include "VisionCommon.h"

namespace vision {
class VISION_API VisionFaceDetect {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceDetect() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_detect_cascade.json", int device_id = 0) = 0;

    /* 输入一张图像img，获取人脸信息（包括人脸框和关键点）；在输入视频的情况下，将is_video_stream设置为true，能够极大提升检测速度 */
    virtual std::vector<VisionFace> GetFaces(const cv::Mat &img, bool is_video_stream = false) = 0;

    /* 输入一张图像img，返回所有的人脸框以及对应的人脸关键点（引用形式返回） */
    virtual std::vector<cv::Rect> GetFaces(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /* 获取图像img中最大人脸的人脸框 */
    virtual std::vector<VisionFace> GetMaximumFace(const cv::Mat &img, bool is_video_stream = false) = 0;

    /* 输入一张图像img，图像img中最大人脸的人脸框以及对应的人脸关键点（引用形式返回） */
    virtual std::vector<cv::Rect> GetMaximumFace(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /* 输入图像img和人脸框face_rect，返回人脸框的置信度和新的人脸框和关键点位置（引用返回）*/
    virtual float VerifyCalibrate(const cv::Mat &img, cv::Rect &face_rect, std::vector<cv::Point2f> &key_pts) = 0;

    /* 输入图像红外摄像头采集到的图像infr_img和自然光摄像头的人脸检测人脸框位置face_rect，返回是否是活体*/
    virtual bool LiveDetect(const cv::Mat &rgb_img, const cv::Mat &nir_img, cv::Rect face_rect) = 0;

    /* 批量处理 */
    virtual std::vector<float> VerifyCalibrate(const cv::Mat &img, std::vector<cv::Rect> &face_rects, std::vector<std::vector<cv::Point2f>> &key_pts) = 0;

    /* 将人脸框face_rect绘制在图像img上 */
    virtual void DrawFaceRect(cv::Mat &img, cv::Rect face_rect) = 0;

    /* 以下接口为高级接口，一般情况下不需要使用 */
    virtual void SetRedetectInterval(int interval = 8)                                    = 0;
    virtual void SetMinFaceSize(int size = 40)                                            = 0;
    virtual void SetMaxFaceSize(int size = 0)                                             = 0;
    virtual void SetScaleFactor(float factor = 0.71f)                                     = 0;
    virtual void SetMaxWidthGlobal(int size = 640)                                        = 0;
    virtual void SetDetectThd(float thd1 = 0.60f, float thd2 = 0.75f, float thd3 = 0.85f) = 0;
    virtual void SetStep1NMSThreshold(float thd = 0.50f)                                  = 0;
    virtual void SetStep2NMSThreshold(float thd = 0.70f)                                  = 0;
    virtual void SetStep3NMSThreshold(float thd = 0.70f)                                  = 0;
    virtual void SetBboxes2Squares(bool flag = true)                                      = 0;
    virtual void SetPreNMSTopN(int pre_top_n = 0)                                         = 0;
    virtual void SetPostNMSTopN(int post_top_n = 0)                                       = 0;
    virtual void SetLiveThd(float live_thd = 0.90f)                                       = 0;
};

/* 构造对象，默认使用GPU 0 */
VISION_API VisionFaceDetect *instantiateVisionFaceDetect(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceDetect(VisionFaceDetect *ptr);
} // namespace vision
#endif // VISION_FACE_DETECT_H
