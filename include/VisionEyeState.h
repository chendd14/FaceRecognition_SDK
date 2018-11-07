/************************************************************************
 * VisionEyeState.h
 * Created by chendd14 on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 *     modify:  2017-12-02， 初始化对象的时候，使用默认参数直接初始化，
 *                           可以不用再次初始化
 ************************************************************************/

#ifndef VISION_EYE_STATE_H
#define VISION_EYE_STATE_H

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
class VISION_API VisionEyeState {
public:
    /* 默认析构函数 */
    virtual ~VisionEyeState() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/eye_state.json", int device_id = 0) = 0;

    /* 输入一张图像img和人脸关键点key_pts，输出左眼和右眼闭合的概率 */
    virtual std::vector<float> GetEyeState(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts) = 0;
};

/* 构造对象，新增变量device_id，默认使用GPU 0 */
VISION_API VisionEyeState *instantiateVisionEyeState(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionEyeState(VisionEyeState *ptr);
} // namespace vision
#endif // VISION_EYE_STATE_H
