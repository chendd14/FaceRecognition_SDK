/************************************************************************
 * VisionFaceQuality.h
 * Created by chendd14 on 2017-10-01
 * Copyright (c) 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_QUALITY_H
#define VISION_FACE_QUALITY_H

#include "VisionCommon.h"

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#ifdef VISIONFACE_EXPORTS
#define VISION_API __declspec(dllexport)
#else
#define VISION_API __declspec(dllimport)
#endif // VISION_API
#else  // defined (windows)
#define VISION_API
#endif

namespace vision {
class VISION_API VisionFaceQuality {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceQuality() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_quality.json", int device_id = 0) = 0;

    /* 获取人脸质量 */
    virtual float GetFaceQuality(const cv::Mat &img, VisionFace face) = 0;

    /* 获取人脸质量, 输入图片、人脸框和人脸关键点 */
    virtual float GetFaceQuality(const cv::Mat &img, const cv::Rect face_rect, const std::vector<cv::Point2f> &key_pts) = 0;

    /* 批量获取人脸图像质量 */
    virtual std::vector<float> GetFaceQuality(const cv::Mat &img, const std::vector<VisionFace> &faces) = 0;

    /* 批量获取人脸图像质量, 输入图片、人脸框和人脸关键点 */
    virtual std::vector<float> GetFaceQuality(const cv::Mat &img, const std::vector<cv::Rect> face_rects, const std::vector<std::vector<cv::Point2f>> &key_pts) = 0;
};

/* 构造对象，默认使用GPU 0 */
VISION_API VisionFaceQuality *instantiateVisionFaceQuality(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceQuality(VisionFaceQuality *ptr);
} // namespace vision
#endif // _VISION_FACE_QUALITY_H
