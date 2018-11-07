/************************************************************************
 * VisionFaceFeature.h
 * Created by cdd on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_FEATURE_H
#define VISION_FACE_FEATURE_H

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
class VISION_API VisionFaceFeature {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceFeature() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_feature.json", int device_id = 0) = 0;

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id，针对V3的模型 */
    virtual bool InitV3(std::string param_path = "face_feature_v3.param", int device_id = 0) = 0;

    /* 获取特征维数 */
    virtual int GetFeatureDim() = 0;

    /* 获取输入归一化人脸图像img的特征 */
    virtual std::vector<float> GetFeature(const cv::Mat &img) = 0;

    /* 批量获取归一化人脸图像特征 */
    virtual std::vector<std::vector<float>> GetFeature(const std::vector<cv::Mat> &imgs) = 0;

    /* 获取两张人脸特征的相似度 */
    virtual float GetScore(const std::vector<float> &fea1, const std::vector<float> &fea2, bool origin_score = false) = 0;

    /* 设置人脸相似度转换参数。当原始人脸相似度等于mean时，新的相似度输出为0.5；var越大，分数变化越缓和 */
    virtual void SetScoreParam(float mean = 0.4, float var = 0.12) = 0;
};

/* 构造对象， 默认使用GPU 0 */
VISION_API VisionFaceFeature *instantiateVisionFaceFeature(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceFeature(VisionFaceFeature *ptr);
} // namespace vision
#endif // VISION_FACE_FEATURE_H
