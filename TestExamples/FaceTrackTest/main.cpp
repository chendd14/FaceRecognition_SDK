#include "VisionFaceTrack.h"

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
	vision::SetLicenseFile("d:/dip/license.lic");
	vision::VisionFaceTrack* face_track = vision::instantiateVisionFaceTrack();
	face_track->SetUseRefind(true);

	cv::VideoCapture capture(0);
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
			}


			cv::imshow("track result", img);
			if (cv::waitKey(1) == 27)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	destroyVisionFaceTrack(face_track);
	return 0;
}