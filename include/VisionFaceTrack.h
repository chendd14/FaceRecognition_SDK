/************************************************************************
 * VisionFaceTrack.h
 * Created by cdd on 2017-09-29
 * Copyright (c) 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_TRACK_H
#define VISION_FACE_TRACK_H

#include <vector>
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
class VISION_API VisionFaceTrack {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceTrack() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_track.json", int device_id = 0) = 0;

    /* 输入视频序列img，返回此时丢失的跟踪对象 */
    virtual std::vector<VisionFace> Track(const cv::Mat &img) = 0;

    /* 输入视频序列img，跟踪最大脸，并返回跟踪信息 */
    virtual std::vector<VisionFace> TrackMaximumFace(const cv::Mat &img) = 0;

    /* 获取当前的所有跟踪信息 */
    virtual std::vector<VisionFace> GetTrackInfo() = 0;

    /* 高级接口 */
    virtual void SetMinFaceSize(int size = 40)               = 0;
    virtual void SetMaxWidthGlobal(int size = 640)           = 0;
    virtual void SetRedetectInterval(int interval = 10)      = 0;
    virtual void SetDetectionNonoverlapIou(float iou = 0.5f) = 0;
    virtual void SetFaceVerifyCalScoreThd(float thd = 0.5f)  = 0;
    virtual void SetFaceQualityThd(float thd = 0.85f)        = 0;
    virtual void SetUseRefind(bool flag = false)             = 0;
    virtual void SetRefindThd(float thd = 0.55f)             = 0;
};

/* 构造对象 */
VISION_API VisionFaceTrack *instantiateVisionFaceTrack(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceTrack(VisionFaceTrack *ptr);
} // namespace vision
#endif // VISION_FACE_TRACK_H
