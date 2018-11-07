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
    /* 默认析构函数 */
    virtual ~VisionFaceAlignment() {
    }

    /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
    virtual bool Init(std::string param_path = "models/face_align.json", int device_id = 0) = 0;

    /* 输入一张图像img和人脸框face_rect，输出人脸的关键点位置 */
    virtual std::vector<cv::Point2f> GetKeyPoints(const cv::Mat &img, const cv::Rect &face_rect) = 0;

    /* 批量获取人脸的关键点位置 */
    virtual std::vector<std::vector<cv::Point2f>> GetKeyPoints(const cv::Mat &img, const std::vector<cv::Rect> &face_rects) = 0;

    /* 输入一张图像img和人脸框face_rect，输出一张归一化人脸（180x220）。若padding是true，则在图像变换的时候采用周边的像素进行补充 */
    virtual cv::Mat GetAlignedFace(const cv::Mat &img, const cv::Rect &face_rect, bool padding = false) = 0;

    /* 批量获取归一化人脸 */
    virtual std::vector<cv::Mat> GetAlignedFace(const cv::Mat &img, const std::vector<cv::Rect> &face_rect, bool padding = false) = 0;

    /* 输入一张图像img和人脸关键点key_pts，输出一张归一化人脸 */
    virtual cv::Mat GetAlignedFace(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts, bool padding = false) = 0;

    /* 批量获取归一化人脸 */
    virtual std::vector<cv::Mat> GetAlignedFace(const cv::Mat &img, const std::vector<std::vector<cv::Point2f>> &key_pts, bool padding = false) = 0;

    /* 根据人脸关键点，获取人脸姿态，pitch、yaw、roll */
    virtual std::vector<float> GetHeadPose(const std::vector<cv::Point2f> &key_pts) = 0;

    /* 批量获取人脸姿态 */
    virtual std::vector<std::vector<float>> GetHeadPose(const std::vector<std::vector<cv::Point2f>> &key_pts) = 0;

    /* 获取归一化人脸，并计算关键点在归一化人脸图像上的位置 */
    virtual cv::Mat GetAlignedFace(const cv::Mat &img, const std::vector<cv::Point2f> &key_pts, std::vector<cv::Point2f> &key_pts_new, float scale = 1.0f, bool padding = false) = 0;
};

/* 构造对象，新增变量device_id，默认使用GPU 0 */
VISION_API VisionFaceAlignment *instantiateVisionFaceAlignment(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceAlignment(VisionFaceAlignment *ptr);
} // namespace vision
#endif // VISION_FACE_ALIGNMENT_H
