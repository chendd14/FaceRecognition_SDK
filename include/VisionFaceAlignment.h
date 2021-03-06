/************************************************************************
 * VisionFaceAlignment.h
 * Created by chendd14 on 2015-11-18
 * Copyright (c) 2015 Vision.EE.Thu, All rights reserved.
 *     modify:  2017-12-02， 初始化对象的时候，使用默认参数直接初始化，
 *                           可以不用再次初始化
 ************************************************************************/

#ifndef VISION_FACE_ALIGNMENT_H
#define VISION_FACE_ALIGNMENT_H

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
class VISION_API VisionFaceAlignment {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path = "models/face_align.json", int device_id = 0) = 0;

    /**
     * @brief GetKeyPoints 获取人脸关键点信息
     * @param img 输入图像（in BGR format）
     * @param face_rect 人脸检测模块获取的人脸框
     * @return 人脸关键点
     */
    virtual std::vector<cv::Point2f> GetKeyPoints(const cv::Mat &img, const cv::Rect &face_rect) = 0;

    /**
     * @brief GetAlignedFace 获取归一化人脸图像
     * @param img 输入图像（in BGR format）
     * @param face_rect 人脸检测模块获取的人脸框
     * @param padding 缺少的信息是否采用缺失像素周边的像素值进行填充，或者填充0
     * @return 归一化人脸图像（180x220）
     */
    virtual cv::Mat GetAlignedFace(const cv::Mat &img, const cv::Rect &face_rect, bool padding = false) = 0;

    /**
     * @brief GetAlignedFace 获取归一化人脸图像
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点
     * @param padding 缺少的信息是否采用缺失像素周边的像素值进行填充，或者填充0
     * @return 归一化人脸图像（180x220）
     */
    virtual cv::Mat GetAlignedFace(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts, bool padding = false) = 0;

    /**
     * @brief GetHeadPose 获取人脸姿态
     * @param key_pts 人脸关键点
     * @return 人脸姿态，pitch、yaw、roll
     */
    virtual std::vector<float> GetHeadPose(const std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief GetAlignedFace 获取归一化人脸，并引用返回归一化图像上人脸关键点为位置
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点
     * @param key_pts_new 引用返回人脸关键点在归一化图像上的位置
     * @param scale 是否对归一化人脸进行放缩
     * @param padding 缺少的信息是否采用缺失像素周边的像素值进行填充，或者填充0
     * @return 归一化人脸图像（180*scalex220*scale）
     */
    virtual cv::Mat GetAlignedFace(const cv::Mat &                 img,
                                   const std::vector<cv::Point2f> &key_pts,
                                   std::vector<cv::Point2f> &      key_pts_new,
                                   float                           scale   = 1.0f,
                                   bool                            padding = false) = 0;

    /**
     * @brief GetKeyPoints 批量获取人脸关键点
     * @param img 输入图像（in BGR format）
     * @param face_rects 多个人脸框
     * @return 人脸关键点
     */
    virtual std::vector<std::vector<cv::Point2f>> GetKeyPoints(const cv::Mat &img, const std::vector<cv::Rect> &face_rects) = 0;

    /**
     * @brief GetAlignedFace 批量获取归一化人脸图像
     * @param img 输入图像（in BGR format）
     * @param key_pts 多个人的人脸关键点
     * @param padding 缺少的信息是否采用缺失像素周边的像素值进行填充，或者填充0
     * @return 归一化人脸图像（180x220）
     */
    virtual std::vector<cv::Mat>
    GetAlignedFace(const cv::Mat &img, const std::vector<std::vector<cv::Point2f>> &key_pts, bool padding = false) = 0;

    /**
     * @brief GetAlignedFace 批量获取归一化人脸图像
     * @param img 输入图像（in BGR format）
     * @param face_rect 多个人脸框
     * @param padding 缺少的信息是否采用缺失像素周边的像素值进行填充，或者填充0
     * @return 归一化人脸
     */
    virtual std::vector<cv::Mat> GetAlignedFace(const cv::Mat &img, const std::vector<cv::Rect> &face_rect, bool padding = false) = 0;

    /**
     * @brief GetHeadPose 批量获取头部姿态
     * @param key_pts 多个人的人脸关键点
     * @return 头部姿态
     */
    virtual std::vector<std::vector<float>> GetHeadPose(const std::vector<std::vector<cv::Point2f>> &key_pts) = 0;

    /**
     * @brief ~VisionFaceAlignment 默认析构函数
     */
    virtual ~VisionFaceAlignment() {
    }
};

/**
 * @brief instantiateVisionFaceAlignment 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceAlignment *instantiateVisionFaceAlignment(int device_id = 0);

/**
 * @brief destroyVisionFaceAlignment 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceAlignment(VisionFaceAlignment *ptr);
} // namespace vision
#endif // VISION_FACE_ALIGNMENT_H
