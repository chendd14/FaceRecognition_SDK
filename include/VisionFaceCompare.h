/************************************************************************
 * @brief
 * @author chendd14@163.com
 * @date
 * @copyright 2017 Vision.EE.Thu, All rights reserved.
 ************************************************************************/

#ifndef VISION_FACE_COMPARE_H
#define VISION_FACE_COMPARE_H

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

struct VisionFaceCompareResult {
    int   idx;   // 特征存储index
    float score; // 相似度
    void *data;  // 用户信息
};

class VisionFaceCompare {
public:
    /**
     * @brief Add 将特征添加到Database
     * @param feature 人脸特征
     * @param data 用户信息指针
     * @return 当前数据的index，-1表示添加失败
     */
    VISION_API virtual int Add(const std::vector<float> &feature, void *data = NULL) = 0;

    /**
     * @brief Delete 删除对应index的特征（内存并不会被释放）
     * @param index 指定index
     * @return true删除成功，false删除失败
     */
    VISION_API virtual bool Delete(int index) = 0;

    /**
     * @brief Update 更新对应index的特征
     * @param index 指定的index
     * @param feature 人脸特征
     * @return true更新成功，false更新失败
     */
    VISION_API virtual bool Update(int index, const std::vector<float> &feature) = 0;

    /**
     * @brief GetResult 获取Top N比对结果（默认Top1）
     * @param feature 目标人脸特征
     * @param top_k 返回的结果条数（小于入库的特征条数）
     * @return 查询结果
     */
    VISION_API virtual std::vector<VisionFaceCompareResult> GetResult(const std::vector<float> &feature, int top_k = 1) = 0;

    /**
     * @brief GetScore 获取两条人脸特征之间的相似度
     * @param fea1 人脸特征1
     * @param fea2 人脸特征2
     * @param origin_score 是否返回原始人脸相似度
     * @return 人脸相似度
     */
    VISION_API virtual float GetScore(const std::vector<float> &fea1, const std::vector<float> &fea2, bool origin_score = false) = 0;

    /**
     * @brief Size 获取当前存储的特征条数
     * @return
     */
    VISION_API virtual int Size() = 0;

    /**
     * @brief SetDatabaseAutoIncreaseRatio 设置数据库空间自增长的比例（小于1024则增长1024条存储空间）
     * @param ratio
     */
    VISION_API virtual void SetDatabaseAutoIncreaseRatio(float ratio = 0.1f) = 0;

    /**
     * @brief Init 初始化模块，为数据存储分配内存空间
     * @param feature_dim 人脸特征维度
     * @param number 为N条人脸特征分配空间
     * @return ture空间分配成功，false空间分配失败
     */
    VISION_API virtual bool Init(int feature_dim, int number) = 0;

    /**
     * @brief ReSize 重新调整需要存储的特征的个数，并且选择是否保留已有的数据
     * @param number 新的数据库存储特征条数
     * @param reserve_data 是否保留数据
     */
    VISION_API virtual void ReSize(int number, bool reserve_data = true) = 0;

    /**
     * @brief Clear 清除数据控，并释放空间
     */
    VISION_API virtual void Clear() = 0;
};

/**
 * @brief instantiateVisionFaceCompare 构造对象
 * @param device_id 需要使用的GPU编号（仅在GPU模式下有效）
 * @return 对象指针
 */
VISION_API VisionFaceCompare *instantiateVisionFaceCompare(int device_id = 0);

/**
 * @brief destroyVisionFaceCompare 销毁对象
 * @param ptr 对象指针
 */
VISION_API void destroyVisionFaceCompare(VisionFaceCompare *_ptr);
} // namespace vision
#endif // VISION_FACE_COMPARE_H
