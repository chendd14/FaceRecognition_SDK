/************************************************************************
 * VisionExtractFeature.h
 * Created by cdd on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 *     modify： 2017-10-19， 更新到版本2，和版本1的model不再兼容（WARNING）
 *     modify:  2017-12-02， 初始化对象的时候，可以传入model，此时直接初始化，
 *                           不用再次调用Init()方法
 ************************************************************************/

#ifndef VISION_EXTRACT_FEATURE_H
#define VISION_EXTRACT_FEATURE_H

#include <vector>

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
class VISION_API VisionExtractFeature {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path, int device_id = 0) = 0;

    /**
     * @brief InitV3 参数初始化
     * @param param_path 参数文件路径（必须使用第3版的模型）
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool InitV3(const std::vector<char> &param, int device_id = 0) = 0;

    /**
     * @brief GetFeatureDim 获取特征维数
     * @return 特征维数
     */
    virtual int GetFeatureDim() = 0;

    /**
     * @brief GetFeature 获取特征
     * @param data 输入数据
     * @param C Channel
     * @param H Height
     * @param W Width
     * @param norm_flag 是否对特征进行归一化
     * @return 特征
     */
    virtual std::vector<float> GetFeature(const std::vector<float> &data, int C, int H, int W, bool norm_flag = false) = 0;

    /**
     * @brief GetFeatures 获取特征
     * @param data 输入数据
     * @param N BatchSize
     * @param C Channel
     * @param H Height
     * @param W Width
     * @param norm_flag 是否对特征进行归一化
     * @return 特征
     */
    virtual std::vector<std::vector<float>>
    GetFeatures(const std::vector<float> &data, int N, int C, int H, int W, bool norm_flag = false) = 0;

    /**
     * @brief ~VisionExtractFeature 默认析构函数
     */
    virtual ~VisionExtractFeature() {
    }
};

/**
 * @brief instantiateVisionExtractFeature 构造对象
 * @param model_str 如果model_str不为空，则直接对深度模型进行初始化
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionExtractFeature *instantiateVisionExtractFeature(std::string model_str = "", int device_id = 0);

/**
 * @brief destroyVisionExtractFeature 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionExtractFeature(VisionExtractFeature *ptr);
} // namespace vision
#endif // VISION_EXTRACT_FEATURE_H
