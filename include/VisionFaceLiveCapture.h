/************************************************************************
 * VisionFaceLiveCapture.h
 * Created by chendd14 on 2018-03-27
 * Copyright (c) 2018 Vision.EE.Thu, All rights reserved.
 ************************************************************************/
#ifndef VISION_FACE_LIVECAPTURE_H
#define VISION_FACE_LIVECAPTURE_H

#include <string>

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

typedef unsigned char uchar;

namespace vision {
// 摄像头分辨率
enum CameraResolution { W640_H480 = 0, W800_H600 = 1, W1024_H768 = 2, W1920_H1080 = 3 };
// 图像旋转类型
enum ImageRotateType { NO_ROTATE = 0, CLOCK_WISE_90 = 1, CLOCK_WISE_180 = 2, CLOCK_WISE_270 = 3 };

class VisionFaceLiveCapture {
public:
    /**
     * @brief OpenCamera 打开摄像头
     * @param type 图像分辨率类型
     * @return true打开成功，false打开失败
     */
    VISION_API virtual bool OpenCamera(CameraResolution type = W640_H480) = 0;

    /**
     * @brief CloseCamera 关闭摄像头
     */
    VISION_API virtual void CloseCamera() = 0;

    /**
     * @brief CheckCameraConnectStatus 检查摄像头是否连接并被识别
     * @return true连接正常，false连接异常
     */
    VISION_API virtual bool CheckCameraConnectStatus() = 0;

    /**
     * @brief GetImgInfo 获取图像信息
     * @param img_width 图像width
     * @param img_height 图像height
     * @param img_channels 图像channels
     * @return
     */
    VISION_API virtual bool GetImgInfo(int &img_width, int &img_height, int &img_channels) = 0;

    /**
     * @brief operator >> 获取一帧图像
     * @param image 引用返回图像
     * @return
     */
    VISION_API virtual VisionFaceLiveCapture &operator>>(cv::Mat &image) = 0;

    /**
     * @brief GetMaximumFace 获取最大人脸
     * @param img 引用返回Camera图像
     * @return 人脸信息，包含活体信息
     */
    VISION_API virtual std::vector<VisionFace> GetMaximumFace(cv::Mat &img) = 0;

    /**
     * @brief Processing 获取图像和人脸信息
     * @param img_data 图像数据
     * @param living_status 活体状态
     * @param x1 人脸框左上角x
     * @param y1 人脸框左上角y
     * @param x2 人脸框右下角x
     * @param y2 人脸框右下角y
     * @return
     */
    VISION_API virtual bool Processing(uchar *img_data, int &living_status, int &x1, int &y1, int &x2, int &y2) = 0;

    /**
     * @brief SetImageRotateType 设置图像旋转
     * @param type 图像旋转类型
     */
    VISION_API virtual void SetImageRotateType(ImageRotateType type) = 0;

    /**
     * @brief SetResolution 设置图像采集分辨率
     * @param type 分辨率类型
     */
    VISION_API virtual void SetResolution(CameraResolution type) = 0;

    /**
     * @brief SetMinFaceSize 设置最小的人脸大小
     * @param value 最小人脸值
     */
    VISION_API virtual void SetMinFaceSize(int value = 140) = 0;

    /**
     * @brief SetLiveThd 设置活体置信度
     * @param live_thd 活体置信度
     */
    VISION_API virtual void SetLiveThd(float live_thd = 0.85f) = 0;
};

/**
 * @brief instantiateVisionFaceLiveCapture 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceLiveCapture *instantiateVisionFaceLiveCapture(int device_id = 0);

/**
 * @brief destroyVisionFaceLiveCapture 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceLiveCapture(VisionFaceLiveCapture *ptr);
} // namespace vision
#endif // VISION_FACE_LIVECAPTURE_H
