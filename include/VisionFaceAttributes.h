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
class VISION_API VisionFaceAttributes {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceAttributes() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_attributes.json", int device_id = 0) = 0;

	/* 初始化，输入参数文件param_path和所要使用的GPU编号device_id，针对V3的模型 */
    virtual bool InitV3(std::string param_path = "face_attributes_v3.param", int device_id = 0) = 0;

    /* 输入一张归一化人脸图像，输出人脸属性 */
    /*0 性别: 0.女, 1.男, -1. 中性;
    1 年龄段: 0.幼, 1.少, 2.青, 3.壮, 4.中, 5.中老, 6.老;
    2 眼镜: 0.无, 1.眼镜, 2.墨镜, -1.无法判断;
    3 种族: 0.黄, 1.白, 2. 黑, -1.无法判断;
    4 颜值: 百分制
    5 表情: 0.Suprise, 1.Fear, 2.Disgust, 3.Happiness, 4.Sadness, 5:Anger 6:Neural;
    6 笑容: 0.无, 1.有, -1.无法判断;
    7 口罩: 0.无, 1.有, -1.无法判断;
    8 年龄：具体岁数;
    9 是否黑色人种：0.否, 1.是, -1.无法判断*/
    virtual std::vector<int>   GetAttributes(const cv::Mat &img)                              = 0;
    virtual std::vector<float> GetAttributesRaw(const cv::Mat &img)                           = 0;
    virtual std::vector<int>   GetAttributes(const cv::Mat &img, std::vector<float> &feature) = 0;

    /* 批量获取人脸属性 */
    virtual std::vector<std::vector<int>>   GetAttributes(const std::vector<cv::Mat> &imgs)                                            = 0;
    virtual std::vector<std::vector<float>> GetAttributesRaw(const std::vector<cv::Mat> &imgs)                                         = 0;
    virtual std::vector<std::vector<int>>   GetAttributes(const std::vector<cv::Mat> &imgs, std::vector<std::vector<float>> &features) = 0;
};

/* 构造此类的一个对象， 默认使用GPU 0 */
VISION_API VisionFaceAttributes *instantiateVisionFaceAttributes(int device_id = 0);

/* 销毁此类的一个对象 */
VISION_API void destroyVisionFaceAttributes(VisionFaceAttributes *ptr);
} // namespace vision
#endif // VISION_FACE_ATTRIBUTES_H
