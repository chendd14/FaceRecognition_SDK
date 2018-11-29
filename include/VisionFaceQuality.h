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
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path = "models/face_quality.json", int device_id = 0) = 0;

    /**
     * @brief GetFaceQuality 获取人脸质量评分
     * @param img 输入图像（in BGR format）
     * @param face 输入图像中的人脸信息
     * @return 人脸质量评分
     */
    virtual float GetFaceQuality(const cv::Mat &img, VisionFace face) = 0;

    /**
     * @brief GetFaceQuality 获取人脸质量评分
     * @param img 输入图像（in BGR format）
     * @param face_rect 人脸框
     * @param key_pts 人脸关键点
     * @return 人脸质量评分
     */
    virtual float GetFaceQuality(const cv::Mat &img, const cv::Rect face_rect, const std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief GetFaceQuality 批量获取人脸质量评分
     * @param img 输入图像（in BGR format）
     * @param faces
     * @return 人脸质量评分
     */
    virtual std::vector<float> GetFaceQuality(const cv::Mat &img, const std::vector<VisionFace> &faces) = 0;

    /**
     * @brief GetFaceQuality 批量获取人脸质量评分
     * @param img 输入图像（in BGR format）
     * @param face_rects 人脸框
     * @param key_pts 人脸关键点
     * @return 人脸质量评分
     */
    virtual std::vector<float>
    GetFaceQuality(const cv::Mat &img, const std::vector<cv::Rect> face_rects, const std::vector<std::vector<cv::Point2f>> &key_pts) = 0;

    /**
     * @brief ~VisionFaceQuality 默认析构函数
     */
    virtual ~VisionFaceQuality() {
    }
};

/**
 * @brief instantiateVisionFaceQuality 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceQuality *instantiateVisionFaceQuality(int device_id = 0);

/**
 * @brief destroyVisionFaceQuality 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceQuality(VisionFaceQuality *ptr);
} // namespace vision
#endif // _VISION_FACE_QUALITY_H
