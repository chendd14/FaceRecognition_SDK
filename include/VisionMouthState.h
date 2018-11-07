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
    /* 默认析构函数 */
    virtual ~VisionMouthState() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/mouth_state.json", int device_id = 0) = 0;

    /* 输入一张图像img和人脸关键点key_pts，输出闭嘴的概率 */
    virtual float GetMouthState(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts) = 0;
};

/* 构造对象，新增变量device_id，默认使用GPU 0 */
VISION_API VisionMouthState *instantiateVisionMouthState(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionMouthState(VisionMouthState *ptr);
} // namespace vision
#endif // VISION_MOUTH_STATE_H
