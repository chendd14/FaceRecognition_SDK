# FaceRecognition SDK Windows edition, build by **visual studio 2015**

### 1.&nbsp;&nbsp;人脸检测
```c++
namespace vision {
	class VISION_API VisionFaceDetect
	{
	public:
		/* 默认析构函数 */
		virtual ~VisionFaceDetect() {}

		/* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
		virtual bool Init(std::string param_path = "models/face_detect_cascade.json", int device_id = 0) = 0;

		/* 输入一张图像img，获取人脸信息（包括人脸框和关键点）；在输入视频的情况下，将is_video_stream设置为true，能够极大提升检测速度 */
		virtual std::vector<VisionFace> GetFaces(const cv::Mat& img, bool is_video_stream = false) = 0;

		/* 输入一张图像img，返回所有的人脸框以及对应的人脸关键点（引用形式返回） */
		virtual std::vector<cv::Rect> GetFaces(const cv::Mat& img, std::vector<std::vector<cv::Point2f>>& key_pts, bool is_video_stream = false) = 0;

		/* 获取图像img中最大人脸的人脸框 */
		virtual std::vector<VisionFace> GetMaximumFace(const cv::Mat& img, bool is_video_stream = false) = 0;

		/* 输入一张图像img，图像img中最大人脸的人脸框以及对应的人脸关键点（引用形式返回） */
		virtual std::vector<cv::Rect> GetMaximumFace(const cv::Mat& img, std::vector<std::vector<cv::Point2f>>& key_pts, bool is_video_stream = false) = 0;
	};

	/* 构造对象，默认使用GPU 0 */
	VISION_API VisionFaceDetect* instantiateVisionFaceDetect(int device_id = 0);

	/* 销毁对象 */
	VISION_API void destroyVisionFaceDetect(VisionFaceDetect* ptr);
}
```
示例代码
```c++
// 摄像头测试
void CameraTest()
{
    VisionFaceDetect *face_detect = instantiateVisionFaceDetect();
    cv::VideoCapture capture(0);
    Mat img;

    while (true)
    {
        capture >> img;

        if (!img.empty()) {
            Mat img_show = img.clone();
            double start = cv::getTickCount();
            std::vector<cv::Rect> face_rects;

            std::vector<std::vector<cv::Point2f>> key_pts;
            face_rects = face_detect->GetFaces(img, key_pts); // 获取人脸框位置同时获取每张脸的5个关键点

            for (int i = 0; i < face_rects.size(); i++)
            {
                face_detect->DrawFaceRect(img_show, face_rects.at(i));
            }
            cv::imshow("img_show", img_show);
            if (cv::waitKey(5) == 27)
            {
                break;
            }
        }
    }
    vision::destroyVisionFaceDetect(face_detect);
}
```

----
### 2.&nbsp;&nbsp;关键点定位和归一化
```c++
namespace vision {
    class VISION_API VisionFaceAlignment
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionFaceAlignment() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/face_align.json", int device_id = 0) = 0;

        /* 输入一张图像img和人脸框face_rect，输出人脸的关键点位置 */
        virtual std::vector<cv::Point2f> GetKeyPoints(const cv::Mat& img, const cv::Rect& face_rect) = 0;

        /* 批量获取人脸的关键点位置 */
        virtual std::vector<std::vector<cv::Point2f>> GetKeyPoints(const cv::Mat& img, const std::vector<cv::Rect>& face_rects) = 0;

        /* 输入一张图像img和人脸框face_rect，输出一张归一化人脸（180x220）。若padding是true，则在图像变换的时候采用周边的像素进行补充 */
        virtual cv::Mat GetAlignedFace(const cv::Mat& img, const cv::Rect& face_rect, bool padding = false) = 0;

        /* 批量获取归一化人脸 */
        virtual std::vector<cv::Mat> GetAlignedFace(const cv::Mat& img, const std::vector<cv::Rect>& face_rect, bool padding = false) = 0;

        /* 输入一张图像img和人脸关键点key_pts，输出一张归一化人脸 */
        virtual cv::Mat GetAlignedFace(const cv::Mat& img, const std::vector<cv::Point2f>& key_pts, bool padding = false) = 0;

        /* 批量获取归一化人脸 */
        virtual std::vector<cv::Mat> GetAlignedFace(const cv::Mat& img, const std::vector<std::vector<cv::Point2f>>& key_pts, bool padding = false) = 0;
    };

    /* 构造对象，新增变量device_id，默认使用GPU 0 */
    VISION_API VisionFaceAlignment* instantiateVisionFaceAlignment(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionFaceAlignment(VisionFaceAlignment* ptr);
}
```
示例代码
```c++
void CameraTest()
{
    VisionFaceDetect* face_detect = instantiateVisionFaceDetect();
    VisionFaceAlignment* face_align = instantiateVisionFaceAlignment();

    cv::VideoCapture capture(0);
    Mat img;

    while (true)
    {
        capture >> img;
        std::vector<std::vector<cv::Point2f>> key_pts;
        std::vector<cv::Rect> face_rects = face_detect->GetFaces(img, key_pts);
        // warning: CPU模式下，人脸检测不会返回关键点，需要通过VisionFaceAlignment获取关键点
        if (key_pts.size() == 0)
        {
            key_pts = face_align->GetKeyPoints(img, face_rects);
        }

        for (int i = 0; i < key_pts.size(); i++)
        {
            for (int j = 0; j < key_pts.at(i).size(); j++)
            {
                cv::circle(img, key_pts.at(i).at(j), 2, cv::Scalar(255, 0, 0), 2);
            }
        }

        std::vector<cv::Mat> norm_faces = face_align->GetAlignedFace(img, key_pts);
        if (norm_faces.size() > 0)
        {
            cv::imshow("norm_face", norm_faces[0]);
        }

        cv::imshow("alignment result", img);

        if (cv::waitKey(5) == 27)
        {
            break;
        }
    }

    vision::destroyVisionFaceDetect(face_detect);
    vision::destroyVisionFaceAlignment(face_align);
}
```

----
### 3.&nbsp;&nbsp;人脸特征提取
```c++
namespace vision {
    class VISION_API VisionFaceFeature
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionFaceFeature() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/face_feature.json", int device_id = 0) = 0;

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id，针对V3的模型【推荐使用】 */
        virtual bool InitV3(std::string param_path = "face_feature_v3.param", int device_id = 0) = 0;

        /* 获取输入归一化人脸图像img的特征 */
        virtual std::vector<float> GetFeature(const cv::Mat& img) = 0;

        /* 批量获取归一化人脸图像特征 */
        virtual std::vector<std::vector<float>> GetFeature(const std::vector<cv::Mat>& imgs) = 0;

        /* 获取两张人脸特征的相似度 */
        virtual float GetScore(const std::vector<float>& fea1, const std::vector<float>& fea2, bool origin_score = false) = 0;
    };

    /* 构造对象， 默认使用GPU 0 */
    VISION_API VisionFaceFeature* instantiateVisionFaceFeature(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionFaceFeature(VisionFaceFeature* ptr);
}
```
示例代码
```c++
void CameraTest()
{
    VisionFaceDetect* face_detect = instantiateVisionFaceDetect();
    VisionFaceAlignment* face_align = instantiateVisionFaceAlignment();
    VisionFaceFeature* face_feature = instantiateVisionFaceFeature();
    // warning, VisionFaceFeature必须进行初始化。默认的模型无法满足需求
    // 其中ResNet224_1202_iter_260000_V3.param可以用其他的.param模型替代
    if (!face_feature->InitV3("ResNet224_1202_iter_260000_V3.param", 0))
    {
        return;
    }

    cv::VideoCapture capture(0);
    Mat img;

    while (true)
    {
        capture >> img;
        std::vector<std::vector<cv::Point2f>> key_pts;
        std::vector<cv::Rect> face_rects = face_detect->GetFaces(img, key_pts);

        for (int i = 0; i < key_pts.size(); i++)
        {
            for (int j = 0; j < key_pts.at(i).size(); j++)
            {
                cv::circle(img, key_pts.at(i).at(j), 2, cv::Scalar(255, 0, 0), 2);
            }
        }

        std::vector<cv::Mat> norm_faces = face_align->GetAlignedFace(img, key_pts);
        if (norm_faces.size() > 0)
        {
            cv::imshow("norm_face", norm_faces[0]);
            // warning， 人脸特征提取的输入必须是归一化人脸图像
            std::vector<float> feature = face_feature->GetFeature(norm_faces[0]);
            for (int i = 0; i < feature.size(); i++)
            {
                std::cout << feature.at(i) << std::endl;
            }
        }
    }

    vision::destroyVisionFaceDetect(face_detect);
    vision::destroyVisionFaceAlignment(face_align);
    vision::destroyVisionFaceFeature(face_feature);
}
```

----
### 4.&nbsp;&nbsp;人脸比对
```c++
namespace vision {

    struct VisionFaceCompareResult
    {
        int idx;
        float score;
		void* data;
    };

    class VISION_API VisionFaceCompare
    {
    public:
        /* 默认构造函数 */
        virtual ~VisionFaceCompare() {}

        /* 初始化模块，构建Database，输入特征维数feature_dim和特征的个数number（可以更改） */
        virtual bool Init(int feature_dim, int number) = 0;

        /* 重新调整需要存储的特征的个数，并且选择是否保留已有的数据 */
        virtual void ReSize(int number, bool reserve_data = true) = 0;

        /* 清除Database中的数据 */
        virtual void Clear() = 0;

        /* 将特征添加到Database，返回index；返回-1时表示添加失败 */
        virtual bool Add(const std::vector<float>& feature, void* data=NULL) = 0;

		/* 删除对应index的特征 */
		virtual bool Delete(int index) = 0;

        /* 获取比对相似度Top N的比对结果 */
        virtual std::vector<VisionFaceCompareResult> GetResult(const std::vector<float>& feature, int top_k = 10) = 0;

        /* 获取两张图片之间的相似度 */
        virtual float GetScore(const std::vector<float>& fea1, const std::vector<float>& fea2, bool origin_score = false) = 0;
    };

    /* 构造对象 */
    VISION_API VisionFaceCompare* instantiateVisionFaceCompare();

    /* 销毁对象 */
    VISION_API void destroyVisionFaceCompare(VisionFaceCompare* _ptr);
}
```
示例代码
```c++
int main()
{
    vision::VisionFaceCompare* face_compare = vision::instantiateVisionFaceCompare();

    int NUMBER = 1000000;
    int FEATURE_DIM = 256;
    // 为Database分配空间
    face_compare->Init(FEATURE_DIM, NUMBER);

    for (int i = 0; i < NUMBER; i++)
    {
        std::vector<float> tmp;
        for (int j = 0; j < FEATURE_DIM; j++)
        {
            tmp.push_back(Rand());
        }
        // 将人脸特征写入Database
        face_compare->Add(tmp);
    }

    // 构建用于查询的feature
    std::vector<float> tmp;
    for (int i = 0; i < FEATURE_DIM; i++)
    {
        tmp.push_back(Rand());
    }

    std::vector<vision::VisionFaceCompareResult> result;

    for (int i = 0; i < 10; i++)
    {
        double start_time = clock();
        // 获取相似度最高的10个结果
        result = face_compare->GetResult(tmp, 10);

        // Sleep(1000);
        double end_time = clock();
        std::cout << "The run time is:" << (end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms!" << std::endl;
    }

    for (int i = 0; i < result.size(); i++)
    {
        std::cout << result.at(i).idx << " - " << result.at(i).score << std::endl;
    }

    return 0;
}
```

----
### 5.&nbsp;&nbsp;人脸质量判断
```c++
namespace vision {
    class VISION_API VisionFaceQuality
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionFaceQuality() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/face_quality.json", int device_id = 0) = 0;

        /* 输入一张归一化人脸图像，输出人脸质量得分[0,1]，分数越高说明质量越好 */
        virtual float GetFaceQuality(const cv::Mat& img) = 0;

        /* 批量获取人脸图像质量 */
        virtual std::vector<float> GetFaceQuality(const std::vector<cv::Mat>& imgs) = 0;
    };

    /* 构造对象，默认使用GPU 0 */
    VISION_API VisionFaceQuality* instantiateVisionFaceQuality(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionFaceQuality(VisionFaceQuality* ptr);
}
```
示例代码
```c++
参照VisionFaceFeature，输入必须是归一化图像。输出结果[0,1]，越靠近1说明人脸质量越高；否则越低。
```

----
### 6.&nbsp;&nbsp;人脸属性（测试中）
```c++
namespace vision {
    class VISION_API VisionFaceAttributes
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionFaceAttributes() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/face_attributes.json", int device_id = 0) = 0;

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id，针对V3的模型【推荐使用】 */
        virtual bool InitV3(std::string param_path = "face_attributes_v3.param", int device_id = 0) = 0;

        /* 输入一张归一化人脸图像，输出人脸属性 */
		/*0 性别: 0.女, 1.男, -1. 中性;
          1 年龄段: 0.幼, 1.少, 2.青, 3.壮, 4.中, 5.中老, 6.老;
          2 眼镜: 0.无, 1.眼镜, 2.墨镜, -1.无法判断;
          3 种族: 0.黄, 1.白, 2. 黑, -1.无法判断;
          4 颜值: 百分制
          5 表情: 0.Suprise, 1.Fear, 2.Disgust, 3.Happiness, 4.Sadness, 5:Anger 6:Neural;
          6 笑容: 0.无, 1.有, -1.无法判断;
          7 口罩: 0.无, 1.有, -1.无法判断;
          8 年龄：具体岁数;
		  9 是否黑色人种：0.否, 1.是, -1.无法判断*/
          //是否黑色人种:在技术文档中未做说明
        virtual std::vector<int> GetAttribtues(const cv::Mat& img) = 0;
		virtual std::vector<float> GetAttributesRaw(const cv::Mat& img) = 0;
        virtual std::vector<int> GetAttribtues(const cv::Mat& img, std::vector<float>& feature) = 0;

        /* 批量获取人脸属性 */
        virtual std::vector<std::vector<int> > GetAttribtues(const std::vector<cv::Mat>& imgs) = 0;
		virtual std::vector<std::vector<float> > GetAttributesRaw(const std::vector<cv::Mat>& imgs) = 0;
        virtual std::vector<std::vector<int> > GetAttribtues(const std::vector<cv::Mat>& imgs, std::vector<std::vector<float>>& features) = 0;
    };

    /* 构造此类的一个对象， 默认使用GPU 0 */
    VISION_API VisionFaceAttributes* instantiateVisionFaceAttributes(int device_id = 0);

    /* 销毁此类的一个对象 */
    VISION_API void destroyVisionFaceAttributes(VisionFaceAttributes* ptr);
}
```
示例代码
```c++
#include <VisionFaceAttributes.h>
#include <vector>
#include <iostream>

int main()
{
	/*
	输入需要是归一化人脸图像，如果不是归一化人脸图像，需要使用detect->alignment对人脸进行归一化
	*/
	cv::Mat img = cv::imread("demo.jpg");
	vision::VisionFaceAttributes * face_attributes = vision::instantiateVisionFaceAttributes();
	std::vector<int> attributes = face_attributes->GetAttributes(img);
	if (attributes[8] == 1) 
	{ 
	std::cout << "戴口罩" << std::endl; 
	} 
	else if (attributes[8] == 0) 
	{ 
		std::cout << "未戴口罩" << std::endl; 
	} 
	else
	{
		std::cout << "不明" << std::endl;
	}
	vision::destroyVisionFaceAttributes(face_attributes);
}
```

----
### 7.&nbsp;&nbsp;人脸跟踪（测试中）
```c++
namespace vision {
    class VISION_API VisionFaceTrack
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionFaceTrack() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/face_track.json", int device_id = 0) = 0;

        /* 输入视频序列img，返回此时丢失的跟踪对象 */
        virtual std::vector<VisionFace> Track(const cv::Mat& img) = 0;

        /* 输入视频序列img，跟踪最大脸，并返回跟踪信息 */
        virtual std::vector<VisionFace> TrackMaximumFace(const cv::Mat& img) = 0;

        /* 获取当前的所有跟踪信息 */
        virtual std::vector<VisionFace> GetTrackInfo() = 0;

        /* 高级接口 */
        virtual void SetRedetectInterval(int interval = 10) = 0;
        virtual void SetDetectionNonoverlapIou(float iou = 0.5f) = 0;
        virtual void SetFaceVerifyCalScoreThd(float thd = 0.5f) = 0;
        virtual void SetFaceQualityThd(float thd = 0.75f) = 0;
        virtual void SetRefindThd(float thd = 0.55f) = 0;
    };

    /* 构造对象 */
    VISION_API VisionFaceTrack* instantiateVisionFaceTrack(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionFaceTrack(VisionFaceTrack* ptr);
}
```
示例代码
```c++
开发测试中
```

----
### 8.&nbsp;&nbsp;特征提取
```c++
namespace vision {
    class VISION_API VisionExtractFeature
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionExtractFeature() {}

        /* 初始化，输入配置文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path, int device_id = 0) = 0;

        /* 输入数据，提取特征 */
        virtual std::vector<float> GetFeature(const std::vector<float> &data, int C, int H, int W, bool norm_flag = false) = 0;

        /* 批量提取数据特征 */
        virtual std::vector<std::vector<float> > GetFeatures(const std::vector<float> &data, int N, int C, int H, int W, bool norm_flag = false) = 0;
    };

    /* 构造对象，如果传入的model_str不为空，则直接对深度模型进行初始化；否则需要再次调用Init函数 */
    VISION_API VisionExtractFeature* instantiateVisionExtractFeature(std::string model_str = "", int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionExtractFeature(VisionExtractFeature* ptr);
}
```
示例代码
```c++
开发测试中
```

----
### 9.&nbsp;&nbsp;眼部状态（测试中）
```c++
namespace vision {
    class VISION_API VisionEyeState
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionEyeState() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/eye_state.json", int device_id = 0) = 0;

        /* 输入一张图像img和人脸关键点key_pts，输出左眼和右眼闭合的概率 */
        virtual std::vector<float> GetEyeState(const cv::Mat& img, const std::vector<cv::Point2f>& key_pts) = 0;
    };

    /* 构造对象，新增变量device_id，默认使用GPU 0 */
    VISION_API VisionEyeState* instantiateVisionEyeState(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionEyeState(VisionEyeState* ptr);
}
```
示例代码
```c++
开发测试中
```

----
### 10.&nbsp;&nbsp;活体相机（测试中）
```c++
namespace vision {
	enum LivingStatus
	{
		// 非活体
		FACE_FAKE = 0,
		// 人脸活体
		FACE_LIVING = 1,
		// 脸偏离屏幕中间（此时会影响活体判断）
		FACE_OFF_CENTER = 2,
		// 人脸太小
		FACE_TOO_SMALL = 3
	};

	// 摄像头分辨率
	enum CameraResolution
	{
		W640_H480 = 0,
		W800_H600 = 1,
		W1024_H768 = 2,
		W1920_H1080 = 3
	};

	class VISION_API VisionFaceLiveCapture
	{
	public:
		/* 默认析构函数 */
		virtual ~VisionFaceLiveCapture() {}

		/* 打开摄像头，打开成功返回true，否则返回false */
		virtual bool OpenCamera() = 0;

		/* 关闭摄像头 */
		virtual void CloseCamera() = 0;

		/* 检查摄像头是否连接并被识别 */
		virtual bool CheckCameraConnectStatus() = 0;
		
		/* 预览拍照 */
		virtual void TaskSnapshot(std::string img_name) = 0;

		/* 获取图像的宽高和通道数 */
		virtual bool GetImgInfo(int& img_width, int& img_height, int& img_channels) = 0;

		/* 获取摄像头数据，img_data需要提前分配好内存空间，其中x1、y1、x2、y2是人脸框的位置 */
		virtual bool Processing(uchar* img_data, int& living_status, int& x1, int& y1, int& x2, int& y2) = 0;

		/* 设置图像采集分辨率 */
		virtual bool SetResolution(int type = W640_H480) = 0;

		/* 设置最小的人脸大小 */
		virtual void SetMinFaceSize(int value = 60) = 0;

		/* 设置活体置信度 */
		virtual void SetLiveThd(float live_thd = 0.90f) = 0;

		/* 提高图像亮度，负数为减小 */
		virtual void IncreaseCameraBrightness(int value = 1) = 0;
		/* 提高图像对比度，负数为减小 */
		virtual void IncreaseCameraContrast(int value = 1) = 0;
		/* 提高图像饱和度，负数为减小 */
		virtual void IncreaseCameraSaturation(int value = 1) = 0;
		/* 提高图像色调，负数为减小 */
		virtual void IncreaseCameraHue(int value = 1) = 0;
		/* 提高图像增益，负数为减小 */
		virtual void IncreaseCameraGain(int value = 1) = 0;
		/* 提高图像曝光，负数为减小 */
		virtual void IncreaseCameraExposure(int value = 1) = 0;
	};

	/* 构造对象，默认使用GPU 0 */
	VISION_API VisionFaceLiveCapture* instantiateVisionFaceLiveCapture(int device_id = 0);

	/* 销毁对象 */
	VISION_API void destroyVisionFaceLiveCapture(VisionFaceLiveCapture* ptr);
}
```
示例代码
```c++
#include "VisionFaceLiveCapture.h"
#include <iostream>
#include "opencv2/opencv.hpp"

#pragma comment(lib, "VisionFaceLiveCapture.lib")
#pragma comment(lib, "VisionFaceCPU.lib")
#pragma comment(lib, "opencv_world320.lib")

int main(int argc, char** argv)
{
	// 新建对象
	vision::VisionFaceLiveCapture* face_live_capture = vision::instantiateVisionFaceLiveCapture();
	// 设置活体置信度，默认为0.90
	//face_live_capture->SetLiveThd(0.90);
	// 检查摄像头是否连接正常
	if (!face_live_capture->CheckCameraConnectStatus())
	{
		std::cout << "Please check camera connection." << std::endl;
		return -1;
	}

	// 打开摄像头
	if (!face_live_capture->OpenCamera())
	{
		std::cout << "Failed to open camera." << std::endl;
		return -1;
	}

	face_live_capture->SetResolution(vision::W800_H600);

	int img_width, img_height, img_channels, x1, y1, x2, y2;
	uchar* img_data;
	int live_status;

	// 获取摄像头获取图像的宽、高和通道数
	if (!face_live_capture->GetImgInfo(img_width, img_height, img_channels))
	{
		std::cout << "Failed to get image info." << std::endl;
		return -1;
	}
	else
	{
		// 初始化数组，存储图像数据（必须提前分配空间）
		img_data = new uchar[img_width * img_height * img_channels];
	}

	while (true)
	{
		// 获取摄像头实时采集的图像，并返回人脸状态和人脸框的位置
		if (face_live_capture->Processing(img_data, live_status, x1, y1, x2, y2))
		{
			// 将输出转化为图像
			cv::Mat img(img_height, img_width, CV_8UC3);
			for (int i = 0; i < img_height; i++)
			{
				for (int j = 0; j < img_width; j++)
				{
					for (int k = 0; k < img_channels; k++)
					{
						img.at<cv::Vec3b>(i, j)[k] = img_data[i*img_width * 3 + j * 3 + k];
					}
				}
			}

			// 显示结果
			if (live_status == vision::FACE_LIVING)
			{
				cv::rectangle(img, cv::Rect(x1, y1, x2 - x1, y2 - y1), cv::Scalar(0, 255, 0));
				cv::putText(img, "Living", cv::Point(x1, y1), 0.5, 0.8, cv::Scalar(0, 255, 0), 2);
			}
			else if(live_status == vision::FACE_FAKE)
			{
				cv::rectangle(img, cv::Rect(x1, y1, x2 - x1, y2 - y1), cv::Scalar(0, 0, 255));
				cv::putText(img, "Fake", cv::Point(x1, y1), 0.5, 0.8, cv::Scalar(0, 0, 255), 2);
			}
			else
			{
				cv::rectangle(img, cv::Rect(x1, y1, x2 - x1, y2 - y1), cv::Scalar(255, 0, 0));
				cv::putText(img, "Please put your face at the center of the screen.", cv::Point(0.5 * (x1 + x2), 0.5 *(y1 + y2)), 0.5, 0.8, cv::Scalar(255, 0, 0), 2);
			}
			

			cv::imshow("test", img);
		}
		if (cv::waitKey(1) == 27)
		{
			// 关闭摄像头
			face_live_capture->CloseCamera();
			// 销毁对象
			vision::destroyVisionFaceLiveCapture(face_live_capture);
			return 0;
		}
	}
}
```

----
### 11.&nbsp;&nbsp;嘴部状态（测试中）
```c++
namespace vision {
    class VISION_API VisionMouthState
    {
    public:
        /* 默认析构函数 */
        virtual ~VisionMouthState() {}

        /* 初始化，输入参数文件param_path和所要使用的GPU编号device_id */
        virtual bool Init(std::string param_path = "models/mouth_state.json", int device_id = 0) = 0;

        /* 输入一张图像img和人脸关键点key_pts，输出闭嘴的概率 */
        virtual float GetMouthState(const cv::Mat& img, const std::vector<cv::Point2f>& key_pts) = 0;
    };

    /* 构造对象，新增变量device_id，默认使用GPU 0 */
    VISION_API VisionMouthState* instantiateVisionMouthState(int device_id = 0);

    /* 销毁对象 */
    VISION_API void destroyVisionMouthState(VisionMouthState* ptr);
}
```
示例代码
```c++
开发测试中
```

----
## General dependencies
#### GPU Edition
- [Cuda 8.0](https://pan.baidu.com/s/1nuI1vy5)
- [Cudnn 5.1](https://pan.baidu.com/s/1V-okUGtn-vFoCbnfvDrthg)

----
## Contact
chendd14 <chendd14@163.com>
<div class="footer">
Copyright(c) chendd14
</div>
