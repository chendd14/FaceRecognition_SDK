/************************************************************************
 * VisionMouthState.h
 * Created by chendd14 on 2018-1-10
 * Copyright (c) 2018 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_MOUTH_STATE_H
#define VISION_MOUTH_STATE_H

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
class VISION_API VisionMouthState {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path = "models/mouth_state.json", int device_id = 0) = 0;

    /**
     * @brief GetMouthState 获取闭嘴的概率
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点
     * @return 闭嘴的概率
     */
    virtual float GetMouthState(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief ~VisionMouthState 默认析构函数
     */
    virtual ~VisionMouthState() {
    }
};

/**
 * @brief instantiateVisionMouthState 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionMouthState *instantiateVisionMouthState(int device_id = 0);

/**
 * @brief destroyVisionMouthState 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionMouthState(VisionMouthState *ptr);
} // namespace vision
#endif // VISION_MOUTH_STATE_H
