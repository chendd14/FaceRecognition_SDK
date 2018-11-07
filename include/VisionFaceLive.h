/************************************************************************
 * VisionFaceLive.h
 * Created by chendd14 on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 *     modify:  2017-12-02， 初始化对象的时候，使用默认参数直接初始化，
 *                           可以不用再次初始化
 ************************************************************************/

#ifndef VISION_FACE_LIVE_H
#define VISION_FACE_LIVE_H

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
enum LIVE_STATE_CODE { LIVE_STATE_LIVE_SUCCEED, LIVE_STATE_FACE_TOO_SMALL, LIVE_STATE_NOT_DIRECTLY_FACING_THE_CAMERA, LIVE_STATE_LOW_COLOR_SATURATION, LIVE_STATE_LIVE_FAILED };

class VISION_API VisionFaceLive {
public:
    /* 默认析构函数 */
    virtual ~VisionFaceLive() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_live.json", int device_id = 0) = 0;

    /* 输入一张图像img和人脸关键点，判断是否是活体 */
    virtual int GetLiveState(const cv::Mat &rgb_img, const cv::Mat &nir_img, const cv::Rect &face_rect, const std::vector<cv::Point2f> &key_pts) = 0;

    /* 活体判断最小脸阈值，小于该阈值直接判定为假体 */
    virtual void SetFaceSizeThd(int min_face_size = 100) = 0;

    /* 人头姿态阈值，超过该阈值直接判定为假体 */
    virtual void SetHeadPoseThd(float head_pose_thd = 15.0f) = 0;

    /* 肤色颜色饱和度阈值，小于该阈值直接判定为假体 */
    virtual void SetSaturationThd(float saturation_thd = 6.5f) = 0;

    /* 高级特征分析，小于该阈值直接判定为假体 */
    virtual void SetHighLevelThd(float high_level_thd = 0.85f) = 0;
};

/* 构造对象，新增变量device_id，默认使用GPU 0 */
VISION_API VisionFaceLive *instantiateVisionFaceLive(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceLive(VisionFaceLive *ptr);
} // namespace vision
#endif // VISION_FACE_LIVE_H
