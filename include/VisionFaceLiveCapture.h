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

enum ImageRotateType { NO_ROTATE = 0, CLOCK_WISE_90 = 1, CLOCK_WISE_180 = 2, CLOCK_WISE_270 = 3 };

class VISION_API VisionFaceLiveCapture {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceLiveCapture() {
    }

    /* 是否在SDK内部打开RGB摄像头 */
    virtual void SetOpenRGBCameraInternal(bool flag = true) = 0;

    /* 打开摄像头，打开成功返回true，否则返回false */
    virtual bool OpenCamera(CameraResolution type = W640_H480) = 0;

    /* 关闭摄像头 */
    virtual void CloseCamera() = 0;

    /* 检查摄像头是否连接并被识别 */
    virtual bool CheckCameraConnectStatus() = 0;

    /* 预览拍照 */
    virtual void TaskSnapshot(std::string img_name) = 0;

    /* 获取图像的宽高和通道数 */
    virtual bool GetImgInfo(int &img_width, int &img_height, int &img_channels) = 0;

    /* 预览RGB图像 */
    virtual VisionFaceLiveCapture &operator>>(cv::Mat &image) = 0;

    /* 开始活体检测，并获取人脸框的位置 */
    virtual std::vector<VisionFace> GetMaximumFace(cv::Mat &img) = 0;

    /* 获取摄像头数据，img_data需要提前分配好内存空间，其中x1、y1、x2、y2是人脸框的位置 */
    virtual bool Processing(uchar *img_data, int &living_status, int &x1, int &y1, int &x2, int &y2) = 0;

    /* 获取一帧红外图像 */
    virtual bool CaptureIRImage() = 0;

    /* 输入人脸检测框的位置，判断是否是活体 */
    virtual bool LiveDetect(int x, int y, int w, int h) = 0;

    /* 设置图像旋转角度 */
    virtual void SetImageRotateType(ImageRotateType type) = 0;

    /* 设置图像采集分辨率 */
    virtual void SetResolution(CameraResolution type) = 0;

    /* 设置最小的人脸大小 */
    virtual void SetMinFaceSize(int value = 140) = 0;

    /* 设置活体置信度 */
    virtual void SetLiveThd(float live_thd = 0.85f) = 0;

    /* 在预览的时候，是否关闭红外摄像头 */
    virtual void PreviewCloseIRCamera(bool flag = false) = 0;
};
/* 构造对象，默认使用GPU 0 */
VISION_API VisionFaceLiveCapture *instantiateVisionFaceLiveCapture(int device_id = 0);
/* 销毁对象 */
VISION_API void destroyVisionFaceLiveCapture(VisionFaceLiveCapture *ptr);
} // namespace vision
#endif // VISION_FACE_LIVECAPTURE_H