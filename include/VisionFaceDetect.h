/************************************************************************
 * @brief face detect use improved cnn cascade
 * @author chendd14@163.com
 * @date 2017-05-31
 * @copyright 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_DETECT_H
#define VISION_FACE_DETECT_H

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#ifdef VISIONFACE_EXPORTS
#define VISION_API __declspec(dllexport)
#else
#define VISION_API __declspec(dllimport)
#endif // VISION_API
#else  // defined (windows)
#define VISION_API
#endif

#include "VisionCommon.h"

namespace vision {
class VisionFaceDetect {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    VISION_API virtual bool Init(std::string param_path = "models/face_detect.json", int device_id = 0) = 0;

    /**
     * @brief GetFaces 获取图片中的人脸信息
     * @param img 输入图片（in BRG format）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸信息，包括人脸框、关键点和人脸姿态（**注意**：通过此接口获取的VisionFace中其他变量的值无效）
     */
    VISION_API virtual std::vector<VisionFace> GetFaces(const cv::Mat &img, bool is_video_stream = false) = 0;

    /**
     * @brief GetFaces 获取图片中的人脸信息
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点信息（引用返回，对应人脸框）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框的信息
     */
    VISION_API virtual std::vector<cv::Rect>
    GetFaces(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /**
     * @brief GetMaximumFace 获取图像中的最大人脸信息
     * @param img 输入图像（in BGR format）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框信息
     */
    VISION_API virtual std::vector<VisionFace> GetMaximumFace(const cv::Mat &img, bool is_video_stream = false) = 0;

    /**
     * @brief GetMaximumFace GetMaximumFace 获取图像中的最大人脸信息
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点信息（引用返回，对应人脸框）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框信息
     */
    VISION_API virtual std::vector<cv::Rect>
    GetMaximumFace(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /**
     * @brief SetMinFaceSize 设置最小检测人脸大小，需要根据使用场景确定合理值，以获得最好的人脸检测效果
     * @param size 人脸大小（像素），值越大检测速度越快
     */
    VISION_API virtual void SetMinFaceSize(int size = 40) = 0;

    /**
     * @brief SetMaxWidthGlobal 设置检测图像最长边大小，大于改值，将对图像进行缩放，然后在小图像上进行人脸检测（不影响人脸检测输出结果）
     * @param size 最长边大小
     */
    VISION_API virtual void SetMaxWidthGlobal(int size = 640) = 0;
};

/**
 * @brief instantiateVisionFaceDetect 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceDetect *instantiateVisionFaceDetect(int device_id = 0);

/**
 * @brief destroyVisionFaceDetect 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceDetect(VisionFaceDetect *ptr);
} // namespace vision
#endif // VISION_FACE_DETECT_H
