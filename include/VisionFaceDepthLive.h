/************************************************************************
 * VisionFaceDepthLive.h
 * Created by chendd14 on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 *     modify:  2017-12-02， 初始化对象的时候，使用默认参数直接初始化，
 *                           可以不用再次初始化
 ************************************************************************/

#ifndef VISION_FACE_DEPTH_LIVE_H
#define VISION_FACE_DEPTH_LIVE_H

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
enum DEPTH_LIVE_STATE_CODE {
    LIVE_STATE_SUCCEED,        // 活体检测成功
    LIVE_STATE_FACE_TOO_CLOSE, // 人脸距离太近
    LIVE_STATE_FACE_TOO_FAR,   // 人脸距离太远
    LIVE_STATE_FACE_TOO_FLAT,  // 非3D人脸
    LIVE_STATE_FAILED          // 非人脸活体
};

class VISION_API VisionFaceDepthLive {
public:
    /**
     * @brief ConvertToRGB 可视化信息
     * @param depth 深度信息
     * @return 深度图
     */
    virtual cv::Mat ConvertToRGB(const cv::Mat &depth) = 0;

    /**
     * @brief GetLiveState 获取深度活体状态
     * @param depth 深度信息一通道
     * @param key_pts 红外图像上人脸关键点位置（对应深度图上人脸关键点位置）
     * @return 详见DEPTH_LIVE_STATE_CODE的说明
     */
    virtual DEPTH_LIVE_STATE_CODE GetLiveState(const cv::Mat &depth, const cv::Rect &face_rect, const std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief SetFaceDistanceMinimum 设置最近活体人脸检测距离
     * @param min_face_dis 默认200mm
     */
    virtual void SetFaceDistanceMinimum(int min_face_dist = 200) = 0;

    /**
     * @brief SetFaceDistanceMaximum 设置最远活体人脸检测距离
     * @param max_face_dist 默认1000mm
     */
    virtual void SetFaceDistanceMaximum(int max_face_dist = 1000) = 0;

    /**
     * @brief SetFaceDepthThreshold 设置人脸深度变化阈值
     * @param depth_thd 默认阈值0.001f，值越大说明越趋向于立体人脸
     */
    virtual void SetFaceDepthThreshold(float depth_thd = 0.001f) = 0;

    /**
     * @brief ~VisionFaceDepthLive 默认析构函数
     */
    virtual ~VisionFaceDepthLive() {
    }
};

/**
 * @brief instantiateVisionFaceDepthLive 构造对象
 * @return 对象指针
 */
VISION_API VisionFaceDepthLive *instantiateVisionFaceDepthLive(int device_id = 0);

/**
 * @brief destroyVisionFaceDepthLive 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceDepthLive(VisionFaceDepthLive *ptr);
} // namespace vision
#endif // VISION_FACE_DEPTH_LIVE_H
