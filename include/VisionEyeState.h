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
class VisionEyeState {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool Init(std::string param_path = "models/eye_state.json", int device_id = 0) = 0;

    /**
     * @brief GetEyeState 获取眼睛闭合的概率
     * @param img 输入图像
     * @param key_pts 人脸关键点
     * @return  左眼和右眼闭合的概率
     */
    VISION_API virtual std::vector<float> GetEyeState(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts) = 0;
    }
};

/**
 * @brief instantiateVisionEyeState 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionEyeState *instantiateVisionEyeState(int device_id = 0);

/**
 * @brief destroyVisionEyeState 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionEyeState(VisionEyeState *ptr);
} // namespace vision
#endif // VISION_EYE_STATE_H
