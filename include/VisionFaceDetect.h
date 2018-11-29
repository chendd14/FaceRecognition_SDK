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
class VISION_API VisionFaceDetect {
public:
    /**
     * @brief Init 参数初始化
     * @param param_path 参数文件路径，推荐使用相对路径，将参数文件放在models文件夹中
     * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
     * @return true初始化成功，false初始化失败
     */
    virtual bool Init(std::string param_path = "models/face_detect.json", int device_id = 0) = 0;

    /**
     * @brief GetFaces 获取图片中的人脸信息
     * @param img 输入图片（in BRG format）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸信息，包括人脸框、关键点和人脸姿态（**注意**：通过此接口获取的VisionFace中其他变量的值无效）
     */
    virtual std::vector<VisionFace> GetFaces(const cv::Mat &img, bool is_video_stream = false) = 0;

    /**
     * @brief GetFaces 获取图片中的人脸信息
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点信息（引用返回，对应人脸框）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框的信息
     */
    virtual std::vector<cv::Rect>
    GetFaces(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /**
     * @brief GetMaximumFace 获取图像中的最大人脸信息
     * @param img 输入图像（in BGR format）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框信息
     */
    virtual std::vector<VisionFace> GetMaximumFace(const cv::Mat &img, bool is_video_stream = false) = 0;

    /**
     * @brief GetMaximumFace GetMaximumFace 获取图像中的最大人脸信息
     * @param img 输入图像（in BGR format）
     * @param key_pts 人脸关键点信息（引用返回，对应人脸框）
     * @param is_video_stream 是否是连续的video帧。在输入为连续video帧的情况下，将此参数设置为true，能够加速检测过程
     * @return 返回人脸框信息
     */
    virtual std::vector<cv::Rect>
    GetMaximumFace(const cv::Mat &img, std::vector<std::vector<cv::Point2f>> &key_pts, bool is_video_stream = false) = 0;

    /**
     * @brief SetMinFaceSize 设置最小检测人脸大小，需要根据使用场景确定合理值，以获得最好的人脸检测效果
     * @param size 人脸大小（像素），值越大检测速度越快
     */
    virtual void SetMinFaceSize(int size = 40) = 0;

    /**
     * @brief SetMaxWidthGlobal 设置检测图像最长边大小，大于改值，将对图像进行缩放，然后在小图像上进行人脸检测（不影响人脸检测输出结果）
     * @param size 最长边大小
     */
    virtual void SetMaxWidthGlobal(int size = 640) = 0;

	/**
     * @brief DrawFaceRect 绘制人脸框
     * @param img 需要绘制人脸框的图像
     * @param face_rect 人脸框
     */
    virtual void DrawFaceRect(cv::Mat &img, cv::Rect face_rect) = 0;

    /**
     * @brief SetBboxes2Squares 设置是否将输出人脸框转化成方框
     * @param flag true转化成方框，false不进行转化
     */
    virtual void SetBboxes2Squares(bool flag = true) = 0;

    /**
     * @brief VerifyCalibrate 人脸框校准
     * @param img 输入图像
     * @param face_rect 原始人脸框
     * @param key_pts 引用返回人脸关键点
     * @return 校准后的人脸框
     */
    virtual float VerifyCalibrate(const cv::Mat &img, cv::Rect &face_rect, std::vector<cv::Point2f> &key_pts) = 0;

    /**
     * @brief VerifyCalibrate 人脸框校准
     * @param img 输入图像
     * @param face_rect 原始人脸框
     * @param key_pts 引用返回人脸关键点
     * @return 校准后的人脸框
     */
    virtual std::vector<float>
    VerifyCalibrate(const cv::Mat &img, std::vector<cv::Rect> &face_rects, std::vector<std::vector<cv::Point2f>> &key_pts) = 0;

    /**
     * @brief SetRedetectInterval 设置重新进行检测的帧间隔（仅在is_video_stream为true的时候有效）
     * @param interval 帧间隔
     */
    virtual void SetRedetectInterval(int interval = 8) = 0;

    /**
     * @brief SetMaxFaceSize 最大检测人脸大小，0表示无限制
     * @param size 最大检测人脸大小
     */
    virtual void SetMaxFaceSize(int size = 0) = 0;

    /**
     * @brief SetScaleFactor 设置图像金字塔缩放系数
     * @param factor 缩放系数
     */
    virtual void SetScaleFactor(float factor = 0.71f) = 0;

    /**
     * @brief SetDetectThd 设置多级检测器的阈值
     * @param thd1 第一级检测器的阈值
     * @param thd2 第二级检测器的阈值
     * @param thd3 第三级检测器的阈值
     */
    virtual void SetDetectThd(float thd1 = 0.60f, float thd2 = 0.75f, float thd3 = 0.85f) = 0;

    /**
     * @brief SetStep1NMSThreshold 设置第一级NMS的阈值
     * @param thd NMS阈值
     */
    virtual void SetStep1NMSThreshold(float thd = 0.50f) = 0;

    /**
     * @brief SetStep2NMSThreshold 设置第二级NMS的阈值
     * @param thd NMS阈值
     */
    virtual void SetStep2NMSThreshold(float thd = 0.70f) = 0;

    /**
     * @brief SetStep3NMSThreshold 设置第三级NMS的阈值
     * @param thd NMS阈值
     */
    virtual void SetStep3NMSThreshold(float thd = 0.70f) = 0;

    /**
     * @brief SetPreNMSTopN 设置NMS之前保留的框的个数
     * @param pre_top_n 框个数
     */
    virtual void SetPreNMSTopN(int pre_top_n = 0) = 0;

    /**
     * @brief SetPostNMSTopN 设置NMS之后保留的框个数
     * @param post_top_n 框个数
     */
    virtual void SetPostNMSTopN(int post_top_n = 0) = 0;

    /**
     * @brief ~VisionFaceDetect 默认析构函数
     */
    virtual ~VisionFaceDetect() {
    }
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
