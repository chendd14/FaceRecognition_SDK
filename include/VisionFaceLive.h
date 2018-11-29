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
enum LIVE_STATE_CODE {
    LIVE_STATE_LIVE_SUCCEED,                   // 活体检测成功
    LIVE_STATE_FACE_TOO_SMALL,                 // 人脸太小，请靠近摄像头
    LIVE_STATE_NOT_DIRECTLY_FACING_THE_CAMERA, // 人脸角度太大，没有正对摄像头，请直视摄像头
    LIVE_STATE_LOW_COLOR_SATURATION,           // 人脸区域色彩饱和度太低
    LIVE_STATE_LIVE_FAILED                     // 活体检测失败
};

class VISION_API VisionFaceLive {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path = "models/face_live.json", int device_id = 0) = 0;

    /**
     * @brief GetLiveState 获取活体状态
     * @param rgb_img RGB图像（BGR format）
     * @param nir_img 红外图像（BGR format）
     * @param face_rect 红外图像人脸框所在的位置
     * @param key_pts 红外图像人脸关键点位置
     * @return 详见LIVE_STATE_CODE的说明
     */
    virtual LIVE_STATE_CODE
    GetLiveState(const cv::Mat &rgb_img, const cv::Mat &nir_img, const cv::Rect &face_rect, const std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief SetFaceSizeThd 设置活体最小人脸阈值，小于该阈值将被判为假体
     * @param min_face_size 最小人脸阈值
     */
    virtual void SetFaceSizeThd(int min_face_size = 100) = 0;

    /**
     * @brief SetHeadPoseThd 设置头部姿态阈值，超过该阈值将被判为假体
     * @param head_pose_thd 头部姿态阈值
     */
    virtual void SetHeadPoseThd(float head_pose_thd = 20.0f) = 0;

    /**
     * @brief SetSaturationThd 设置人脸饱和度阈值，小于该阈值将被判定为假体
     * @param saturation_thd 人脸饱和度阈值
     */
    virtual void SetSaturationThd(float saturation_thd = 6.5f) = 0;

    /**
     * @brief SetHighLevelThd 设置人脸活体深度学习分类器阈值
     * @param high_level_thd 深度学习分类器阈值
     */
    virtual void SetHighLevelThd(float high_level_thd = 0.85f) = 0;

    /**
     * @brief ~VisionFaceLive 默认析构函数
     */
    virtual ~VisionFaceLive() {
    }
};

/**
 * @brief instantiateVisionFaceLive 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceLive *instantiateVisionFaceLive(int device_id = 0);

/**
 * @brief destroyVisionFaceLive 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceLive(VisionFaceLive *ptr);
} // namespace vision
#endif // VISION_FACE_LIVE_H
