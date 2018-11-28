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
class VisionFaceTrack {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool Init(std::string param_path = "models/face_track.json", int device_id = 0) = 0;

    /**
     * @brief Track 人脸跟踪
     * @param img 输入视频序列帧
     * @return 当前帧判定丢失的人脸信息（如果人脸跟踪正常，将不被返回）
     */
    VISION_API virtual std::vector<VisionFace> Track(const cv::Mat &img) = 0;

    /**
     * @brief TrackMaximumFace 跟踪最大人脸
     * @param img 输入视频序列帧
     * @return 当前帧判定丢失的人脸信息（如果人脸跟踪正常，将不被返回）
     */
    VISION_API virtual std::vector<VisionFace> TrackMaximumFace(const cv::Mat &img) = 0;

    /**
     * @brief GetTrackInfo 获取当前的所有跟踪信息
     * @return 跟踪信息（每一个跟踪对象）
     */
    VISION_API virtual std::vector<VisionFace> GetTrackInfo() = 0;

    /**
     * @brief SetMinFaceSize 设置最小跟踪人脸
     * @param size 最小跟踪人脸（像素），默认40
     */
    VISION_API virtual void SetMinFaceSize(int size = 40) = 0;

    /**
     * @brief SetMaxWidthGlobal 设置人脸检测中图像最长边的大小（超过该值，图像将被缩放）
     * @param size 最长边大小
     */
    VISION_API virtual void SetMaxWidthGlobal(int size = 640) = 0;

    /**
     * @brief SetRedetectInterval 设置重新进行检测的帧间隔
     * @param interval 帧间隔
     */
    VISION_API virtual void SetRedetectInterval(int interval = 10) = 0;

    /**
     * @brief SetFaceQualityThd 设置人脸质量阈值，超过该阈值的人脸可以计算人脸特征等
     * @param thd 人脸质量阈值
     */
    VISION_API virtual void SetFaceQualityThd(float thd = 0.85f) = 0;

    /**
     * @brief SetUseRefind 设置是否启用重找回
     * @param flag true进行重找回，false不进行重找回
     */
    VISION_API virtual void SetUseRefind(bool flag = false) = 0;

    /**
     * @brief SetRefindThd 设置重找回的阈值
     * @param thd 重找回阈值
     */
    VISION_API virtual void SetRefindThd(float thd = 0.55f) = 0;
};

/**
 * @brief instantiateVisionFaceTrack 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceTrack *instantiateVisionFaceTrack(int device_id = 0);

/**
 * @brief destroyVisionFaceTrack 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceTrack(VisionFaceTrack *ptr);
} // namespace vision
#endif // VISION_FACE_TRACK_H
