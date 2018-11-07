#ifndef _VISION_COMMON_H
#define _VISION_COMMON_H

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#ifdef VISIONFACE_EXPORTS
#define VISION_API __declspec(dllexport)
#else
#define VISION_API __declspec(dllimport)
#endif // VISION_API
#else  // defined (windows)
#define VISION_API
#endif

#define VISION_VERSION 2
#include "opencv2/opencv.hpp"

namespace vision {
struct VisionFace {
    int                                    face_id;            // track id
    cv::Rect                               bbox;               // face bounding box
    std::vector<cv::Point2f>               key_pts;            // face key points
    float                                  head_pose[3];       // pitch、yaw、roll
    cv::Mat                                norm_face;          // a normalized face image
    float                                  face_quality_score; // current face quality
    std::vector<std::pair<cv::Mat, float>> norm_faces;         // normalized face images
    int                                    gender;             // 0 female；1 male
    int                                    age_group;          // 0 baby; 1 preteen; 2 teenager; 3 adult; 4 middle-age; 5 middle old-age; old age
    int                                    glasses;            // 0 no; 1 yes; 2 sunglasses
    int                                    race;               // 0 yellow; 1 black; 2 white
    int                                    attractive;         // score for 0-100
    int                                    expression;         // 0 suprise; 1 fear; 2 disgust; 3 happiness; 4 sadness; 5 anger; 6 netural
    int                                    smile;              // 0 no; 1 yes
    int                                    mask;               // 0 no; 1 yes
    int                                    age;                // a number
    int                                    eye;                // 0 open; 1 close
    int                                    beard;              // 0 no; 1 yes
    int                                    track_frames;       // frames count tracked
    bool                                   live_state;         // 0 fake, 1 live
    std::vector<cv::Point2f>               tracking_path;      // histroy face position
    VISION_API cv::Mat GetBestFace();                          // get a face image with best face quality score
    VISION_API cv::Rect GetBbox();                             // get face bounding box
    VISION_API std::vector<cv::Point2f> GetKeyPts();           // get face key points
};

VISION_API void SetBlasCoreNum(int num = 2);              // set the number of scores the blas library can use
VISION_API void SetLicenseFile(std::string license_file); // set license file path
VISION_API std::string GetLicenseFile();                  // get license file path
} // namespace vision
#endif
