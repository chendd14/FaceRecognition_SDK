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
    int   idx;
    float score;
    void *data;
};

class VISION_API VisionFaceCompare {
public:
    /* 默认构造函数 */
    virtual ~VisionFaceCompare() {
    }

    /* 初始化模块，构建Database，输入特征维数feature_dim和特征的个数number（可以更改） */
    virtual bool Init(int feature_dim, int number) = 0;

    /* 重新调整需要存储的特征的个数，并且选择是否保留已有的数据 */
    virtual void ReSize(int number, bool reserve_data = true) = 0;

    /* 清除Database中的数据 */
    virtual void Clear() = 0;

    /* 将特征添加到Database，返回index；返回-1时表示添加失败 */
    virtual int Add(const std::vector<float> &feature, void *data = NULL) = 0;

    /* 删除对应index的特征 */
    virtual bool Delete(int index) = 0;

	/* 更新对应index的特征 */
    virtual bool Update(int index, const std::vector<float> &feature) = 0;

    /* 获取比对相似度Top N的比对结果 */
    virtual std::vector<VisionFaceCompareResult> GetResult(const std::vector<float> &feature, int top_k = 10) = 0;

    /* 获取两张图片之间的相似度 */
    virtual float GetScore(const std::vector<float> &fea1, const std::vector<float> &fea2, bool origin_score = false) = 0;

	/* 获取存储的特征个数 */
    virtual int Size() = 0;

    /* 设置人脸相似度转换参数。当原始人脸相似度等于mean时，新的相似度输出为0.5；var越大，分数变化越缓和 */
    virtual void SetScoreParam(float mean = 0.4, float var = 0.12) = 0;
};

/* 构造对象 */
VISION_API VisionFaceCompare *instantiateVisionFaceCompare(int device_id = 0);

/* 销毁对象 */
VISION_API void destroyVisionFaceCompare(VisionFaceCompare *_ptr);
} // namespace vision
#endif // VISION_FACE_COMPARE_H
