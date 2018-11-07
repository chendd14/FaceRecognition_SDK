#include "VisionFaceTrack/VisionFaceTrack.h"

#include <vector>
#include <fstream>

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

template<typename T>
std::string ToString(const T& tmp)
{
	std::stringstream sstream;
	sstream << tmp;
	return sstream.str();
}

int main(int argc, char** argv) {
	vision::VisionFaceTrack* face_track = vision::instantiateVisionFaceTrack();

	cv::VideoCapture capture("D:\\GitLab\\VisionFaceProject\\Resources\\test_videos\\track_test_sample_video.mp4");
	cv::Mat img;

	while (true)
	{
		capture >> img;
		if (!img.empty()) {
			face_track->Track(img);

			std::vector<vision::VisionFace> track_results = face_track->GetTrackInfo();

			for (int i = 0; i < track_results.size(); i++)
			{
				cv::rectangle(img, track_results.at(i).bbox, cv::Scalar(255, 0, 0), 2);
				cv::putText(img, ToString(track_results[i].face_id), cv::Point(track_results[i].bbox.x, track_results[i].bbox.y), 1, 2.8, cv::Scalar(0, 255, 0), 2);

				//cv::imshow("best", track_results[i].GetBestFace());
			}
		}

		cv::imshow("track result", img);
		if (cv::waitKey(1) == 27)
		{
			break;
		}
	}
	destroyVisionFaceTrack(face_track);
	return 0;
}