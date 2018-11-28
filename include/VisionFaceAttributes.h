/************************************************************************
 * VisionFaceAttributes.h
 * Created by cdd on 2017-09-30
 * Copyright (c) 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_ATTRIBUTES_H
#define VISION_FACE_ATTRIBUTES_H

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
class VisionFaceAttributes {
public:
    /**
     * @brief Init 参数初始化（该接口后期将被抛弃）
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool Init(std::string param_path = "models/face_attributes.json", int device_id = 0) = 0;

    /**
     * @brief InitV3 参数初始化
     * @param param_path 参数文件路径（必须使用第3版的模型）
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool InitV3(std::string param_path = "face_attributes_v3.param", int device_id = 0) = 0;

    /**
     * @brief GetAttributes 获取人脸属性信息
     * @param img 输入图像（in BGR format）
     * @return
     * 0 性别: 0.女, 1.男, -1. 中性;
     * 1 年龄段: 0.幼, 1.少, 2.青, 3.壮, 4.中, 5.中老, 6.老;
     * 2 眼镜: 0.无, 1.眼镜, 2.墨镜, -1.无法判断;
     * 3 种族: 0.黄, 1.白, 2. 黑, -1.无法判断;
     * 4 颜值: 百分制
     * 5 表情: 0.Suprise, 1.Fear, 2.Disgust, 3.Happiness, 4.Sadness, 5:Anger 6:Neural;
     * 6 笑容: 0.无, 1.有, -1.无法判断;
     * 7 口罩: 0.无, 1.有, -1.无法判断;
     * 8 年龄：具体岁数;
     * 9 是否黑色人种：0.否, 1.是, -1.无法判断
     */
    VISION_API virtual std::vector<int> GetAttributes(const cv::Mat &img) = 0;

    /**
     * @brief GetAttributesRaw 获取人脸属性原始信息（属性概率值）
     * @param img 输入图像（in BGR format）
     * @return 对应属性的概率值
     */
    VISION_API virtual std::vector<float> GetAttributesRaw(const cv::Mat &img) = 0;
};

/**
 * @brief instantiateVisionFaceAttributes 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceAttributes *instantiateVisionFaceAttributes(int device_id = 0);

/**
 * @brief destroyVisionFaceAttributes 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceAttributes(VisionFaceAttributes *ptr);
} // namespace vision
#endif // VISION_FACE_ATTRIBUTES_H
