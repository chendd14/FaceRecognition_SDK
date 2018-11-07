#include "VisionFaceDetect/VisionFaceDetect.h"
#include "VisionFaceQuality/VisionFaceQuality.h"

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif

void CameraTest()
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect();
	vision::VisionFaceQuality* face_quality = vision::instantiateVisionFaceQuality();

	cv::VideoCapture cap(0);
	cv::Mat frame;
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> frame;

			std::vector<vision::VisionFace> faces = face_detect->GetFaces(frame);

			double start = (double)cvGetTickCount(); 
			std::vector<float> faces_qulity = face_quality->GetFaceQuality(frame, faces);
			double stop = (double)cvGetTickCount();
			double time = (stop - start) / (cvGetTickFrequency() * 1000);
			std::cout << "Face quality time cost: " << time << " ms." << std::endl;

			for (size_t i = 0; i < faces.size(); i++)
			{
				face_detect->DrawFaceRect(frame, faces[i].bbox);
				//system("cls");
				std::cout << faces_qulity[i] << std::endl;
			}

			cv::imshow("test example", frame);
			if (cv::waitKey(1) == 27)
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "Failed to open camera." << std::endl;
	}

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceQuality(face_quality);
}

int main(int argc, char** argv)
{
	CameraTest();
	return 0;
}