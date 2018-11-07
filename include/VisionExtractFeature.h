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
    /* 默认析构函数 */
    virtual ~VisionExtractFeature() {
    }

    /* 初始化，输入配置文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path, int device_id = 0) = 0;

    /* 初始化，输入参数和所要使用的GPU编号device_id， 只能使用V3的模型 */
    virtual bool InitV3(const std::vector<char> &param, int device_id = 0) = 0;

    /* 获取特征维度 */
    virtual int GetFeatureDim() = 0;

    /* 输入数据，提取特征 */
    virtual std::vector<float> GetFeature(const std::vector<float> &data, int C, int H, int W, bool norm_flag = false) = 0;

    /* 批量提取数据特征 */
    virtual std::vector<std::vector<float>> GetFeatures(const std::vector<float> &data, int N, int C, int H, int W, bool norm_flag = false) = 0;
};

/* 构造对象，如果传入的model_str不为空，则直接对深度模型进行初始化；否则需要再次调用Init函数 */
VISION_API VisionExtractFeature *instantiateVisionExtractFeature(std::string model_str = "", int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionExtractFeature(VisionExtractFeature *ptr);
} // namespace vision
#endif // VISION_EXTRACT_FEATURE_H