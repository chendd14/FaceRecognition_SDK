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
class VisionFaceFeature {
public:
    /**
     * @brief Init 参数初始化（该接口后期将被抛弃）
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool Init(std::string param_path = "models/face_feature.json", int device_id = 0) = 0;

    /**
     * @brief InitV3 参数初始化
     * @param param_path 参数文件路径（必须使用第3版的模型）
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool InitV3(std::string param_path = "face_feature_v3.param", int device_id = 0) = 0;

    /**
     * @brief GetFeatureDim 获取特征维数
     * @return 特征维数
     */
    VISION_API virtual int GetFeatureDim() = 0;

    /**
     * @brief GetFeature 获取输入归一化人脸图像的特征
     * @param img 归一化人脸图像
     * @return 人脸特征
     */
    VISION_API virtual std::vector<float> GetFeature(const cv::Mat &img) = 0;

    /**
     * @brief GetScore 获取两张人脸特征的相似度
     * @param fea1 人脸特征1
     * @param fea2 人脸特征2
     * @param origin_score 是否返回原始人脸相似度
     * @return 人脸相似度
     */
    VISION_API virtual float GetScore(const std::vector<float> &fea1, const std::vector<float> &fea2, bool origin_score = false) = 0;

    /**
     * @brief SetScoreParam 设置人脸相似度转换参数
     * @param mean 高斯分布的均值
     * @param var 高斯分布的方差
     */
    virtual void SetScoreParam(float mean = 0.4, float var = 0.12) = 0;

    /**
     * @brief GetFeature 批量获取归一化人脸图像特征
     * @param imgs 输入归一化人脸图像
     * @return 人脸特征
     */
    virtual std::vector<std::vector<float>> GetFeature(const std::vector<cv::Mat> &imgs) = 0;

    /**
     * @brief ~VisionFaceFeature 默认析构函数
     */
    virtual ~VisionFaceFeature() {
    }
};

/**
 * @brief instantiateVisionFaceFeature 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceFeature *instantiateVisionFaceFeature(int device_id = 0);

/**
 * @brief destroyVisionFaceFeature 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceFeature(VisionFaceFeature *ptr);
} // namespace vision
#endif // VISION_FACE_FEATURE_H
