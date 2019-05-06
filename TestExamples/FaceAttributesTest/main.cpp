#ifdef _WIN32
#include "VisionFaceDetect.h"
#include "VisionFaceAlignment.h"
#include "VisionFaceAttributes.h"

#include <vector>
#include <iostream>
#include <windows.h>

using namespace vision;
using namespace std;
using namespace cv;

#ifdef _DEBUG
#pragma comment(lib, "VisionFaceCPUd.lib")
#else
#pragma comment(lib, "VisionFaceCPU.lib")
#endif // _DEBUG

void GetStringSize(HDC hDC, const char* str, int* w, int* h)
{
	SIZE size;
	GetTextExtentPoint32A(hDC, str, strlen(str), &size);
	if (w != 0) *w = size.cx;
	if (h != 0) *h = size.cy;
}

void putTextZH(Mat &dst, const char* str, Point org, Scalar color, int fontSize, const char* fn, bool italic = false, bool underline = false)
{
	CV_Assert(dst.data != 0 && (dst.channels() == 1 || dst.channels() == 3));

	int x, y, r, b;
	if (org.x > dst.cols || org.y > dst.rows) return;
	x = org.x < 0 ? -org.x : 0;
	y = org.y < 0 ? -org.y : 0;

	LOGFONTA lf;
	lf.lfHeight = -fontSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 5;
	lf.lfItalic = italic;   //斜体
	lf.lfUnderline = underline; //下划线
	lf.lfStrikeOut = 0;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy_s(lf.lfFaceName, fn);

	HFONT hf = CreateFontIndirectA(&lf);
	HDC hDC = CreateCompatibleDC(0);
	HFONT hOldFont = (HFONT)SelectObject(hDC, hf);

	int strBaseW = 0, strBaseH = 0;
	int singleRow = 0;
	char buf[1 << 12];
	strcpy_s(buf, str);
	char *bufT[1 << 12];  // 这个用于分隔字符串后剩余的字符，可能会超出。
						  //处理多行
	{
		int nnh = 0;
		int cw, ch;

		const char* ln = strtok_s(buf, "\n", bufT);
		while (ln != 0)
		{
			GetStringSize(hDC, ln, &cw, &ch);
			strBaseW = max(strBaseW, cw);
			strBaseH = max(strBaseH, ch);

			ln = strtok_s(0, "\n", bufT);
			nnh++;
		}
		singleRow = strBaseH;
		strBaseH *= nnh;
	}

	if (org.x + strBaseW < 0 || org.y + strBaseH < 0)
	{
		SelectObject(hDC, hOldFont);
		DeleteObject(hf);
		DeleteObject(hDC);
		return;
	}

	r = org.x + strBaseW > dst.cols ? dst.cols - org.x - 1 : strBaseW - 1;
	b = org.y + strBaseH > dst.rows ? dst.rows - org.y - 1 : strBaseH - 1;
	org.x = org.x < 0 ? 0 : org.x;
	org.y = org.y < 0 ? 0 : org.y;

	BITMAPINFO bmp = { 0 };
	BITMAPINFOHEADER& bih = bmp.bmiHeader;
	int strDrawLineStep = strBaseW * 3 % 4 == 0 ? strBaseW * 3 : (strBaseW * 3 + 4 - ((strBaseW * 3) % 4));

	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = strBaseW;
	bih.biHeight = strBaseH;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = strBaseH * strDrawLineStep;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	void* pDibData = 0;
	HBITMAP hBmp = CreateDIBSection(hDC, &bmp, DIB_RGB_COLORS, &pDibData, 0, 0);

	CV_Assert(pDibData != 0);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hDC, hBmp);

	//color.val[2], color.val[1], color.val[0]
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0);
	//SetStretchBltMode(hDC, COLORONCOLOR);

	strcpy_s(buf, str);
	const char* ln = strtok_s(buf, "\n", bufT);
	int outTextY = 0;
	while (ln != 0)
	{
		TextOutA(hDC, 0, outTextY, ln, strlen(ln));
		outTextY += singleRow;
		ln = strtok_s(0, "\n", bufT);
	}
	uchar* dstData = (uchar*)dst.data;
	int dstStep = dst.step / sizeof(dstData[0]);
	unsigned char* pImg = (unsigned char*)dst.data + org.x * dst.channels() + org.y * dstStep;
	unsigned char* pStr = (unsigned char*)pDibData + x * 3;
	for (int tty = y; tty <= b; ++tty)
	{
		unsigned char* subImg = pImg + (tty - y) * dstStep;
		unsigned char* subStr = pStr + (strBaseH - tty - 1) * strDrawLineStep;
		for (int ttx = x; ttx <= r; ++ttx)
		{
			for (int n = 0; n < dst.channels(); ++n) {
				double vtxt = subStr[n] / 255.0;
				int cvv = vtxt * color.val[n] + (1 - vtxt) * subImg[n];
				subImg[n] = cvv > 255 ? 255 : (cvv < 0 ? 0 : cvv);
			}

			subStr += 3;
			subImg += dst.channels();
		}
	}

	SelectObject(hDC, hOldBmp);
	SelectObject(hDC, hOldFont);
	DeleteObject(hf);
	DeleteObject(hBmp);
	DeleteDC(hDC);
}

void PutAttributesText(cv::Mat& img, string id, cv::Rect face_rect, cv::Point point, int face_width,
	string gender, string age_group, string glasses, string race, string attractive, string expression,
	string smile, string mask, string age, std::vector<cv::Point2f> tracking_path)
{
	int att_num = 0;

	if (gender != "未知")
	{
		att_num++;
	}

	if (age_group != "未知")
	{
		att_num++;
	}

	if (glasses != "未知")
	{
		att_num++;
	}

	if (race != "未知")
	{
		att_num++;
	}

	if (attractive != "未知")
	{
		att_num++;
	}

	if (expression != "未知")
	{
		att_num++;
	}

	if (smile != "未知")
	{
		att_num++;
	}

	if (mask != "未知")
	{
		att_num++;
	}

	if (age != "未知")
	{
		att_num++;
	}

	cv::Point ori_point = point;
	/*if (gender == "未知")
	{
	return;
	}*/

	int font_size = 14;

	putTextZH(img, id.c_str(), point - cv::Point(0, font_size + 6), cv::Scalar(0, 0, 255), font_size + 2, "微软雅黑");

	point += Point(face_width + 1, 0);

	int width = 80;
	int height = (font_size + 2) * att_num + 10;
	string font = "simhei";
	cv::Scalar font_color(255, 0, 0);

	if (att_num > 0)
	{
		cv::Mat info_background(width, height, CV_8UC3, Scalar(255, 255, 255));
		if ((point.x + width) >= img.cols)
		{
			width = img.cols - point.x;
		}
		if ((point.y + height) >= img.rows)
		{
			height = img.rows - point.y;
		}
		if (width > 5 && height > 5)
		{
			cv::resize(info_background, info_background, cv::Size(width, height));
			cv::Mat imageROI = img(cv::Rect(point.x, point.y, info_background.cols, info_background.rows));
			cv::addWeighted(imageROI, 1.0, info_background, 0.4, 0, imageROI);

			cv::rectangle(img, cv::Rect(point.x, point.y, info_background.cols, info_background.rows), Scalar(200, 200, 200));
		}

		point += Point(3, 0);

		int tmp_idx = 0;
		point += Point(0, 4);

		if (gender != "未知")
		{
			putTextZH(img, gender.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (age_group != "未知")
		{
			putTextZH(img, age_group.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (glasses != "未知")
		{
			putTextZH(img, glasses.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (race != "未知")
		{
			putTextZH(img, race.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (attractive != "未知")
		{
			putTextZH(img, attractive.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (expression != "未知")
		{
			putTextZH(img, expression.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (smile != "未知")
		{
			putTextZH(img, smile.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (mask != "未知")
		{
			putTextZH(img, mask.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}

		if (age != "未知")
		{
			putTextZH(img, age.c_str(), point + cv::Point(0, (font_size + 2) * (tmp_idx++)), font_color, font_size, font.c_str());
		}
	}

	int show_num = 50;
	int tracking_path_size = tracking_path.size();
	if (tracking_path.size() < show_num)
	{
		show_num = tracking_path.size();
	}

	float scale = width / 6.0f / show_num;

	for (int i = 1; i < show_num; i++)
	{
		int x = tracking_path.at(tracking_path_size - i).x;
		int y = tracking_path.at(tracking_path_size - i).y;

		if ((x > face_rect.x && x < (face_rect.x + face_rect.width)) && (y > face_rect.y && y < (face_rect.y + face_rect.height)))
		{
			continue;
		}
		cv::line(img, tracking_path.at(tracking_path_size - i), tracking_path.at(tracking_path_size - 1 - i), cv::Scalar(0, 255, 0), (show_num - i) * scale);
	}
}

std::string int2str(const int &int_temp)
{
	std::string string_temp;
	std::stringstream stream;
	stream << int_temp;
	string_temp = stream.str();
	return string_temp;
}

void DrawAttributesResult(cv::Mat& img, int id, vision::VisionFace& vision_face)
{
	string gender;
	if (vision_face.gender == 0)
		gender = "女";
	else if (vision_face.gender == 1)
		gender = "男";
	else if (vision_face.gender == 2)
		gender = "中性";
	else
		gender = "未知";

	string age_group;
	if (vision_face.age_group == 0)
		age_group = "儿童";
	else if (vision_face.age_group == 1)
		age_group = "少年";
	else if (vision_face.age_group == 2)
		age_group = "青年";
	else if (vision_face.age_group == 3)
		age_group = "中青年";
	else if (vision_face.age_group == 4)
		age_group = "中年";
	else if (vision_face.age_group == 5)
		age_group = "中老年";
	else if (vision_face.age_group == 6)
		age_group = "老年";
	else
		age_group = "未知";

	string glasses;
	if (vision_face.glasses == 0)
		glasses = "无眼镜";
	else if (vision_face.glasses == 1)
		glasses = "戴眼镜";
	else if (vision_face.glasses == 2)
		glasses = "戴墨镜";
	else
		glasses = "未知";

	string race;
	if (vision_face.race == 0)
		race = "黄种人";
	else if (vision_face.race == 1)
		race = "白人";
	else if (vision_face.race == 2)
		race = "黑人";
	else
		race = "未知";

	string attractive;
	if (vision_face.attractive == -1)
		attractive = "未知";
	else
		attractive = "颜值" + int2str(vision_face.attractive);

	string expression;
	if (vision_face.expression == 0)
		expression = "惊讶";
	else if (vision_face.expression == 1)
		expression = "害怕";
	else if (vision_face.expression == 2)
		expression = "烦躁";
	else if (vision_face.expression == 3)
		expression = "开心";
	else if (vision_face.expression == 4)
		expression = "伤心";
	else if (vision_face.expression == 5)
		expression = "生气";
	else if (vision_face.expression == 6)
		expression = "淡定";
	else
		expression = "未知";

	string smile;
	if (vision_face.smile == 0)
		smile = "没有微笑";
	else if (vision_face.smile == 1)
		smile = "微笑";
	else
		smile = "未知";

	string mask;
	if (vision_face.mask == 0)
		mask = "无口罩";
	else if (vision_face.mask == 1)
		mask = "戴口罩";
	else
		mask = "未知";

	string age;
	if (vision_face.age == -1)
		age = "未知";
	else
		age = "年龄" + int2str(vision_face.age);

	PutAttributesText(img, int2str(id), vision_face.bbox, cv::Point(vision_face.bbox.x, vision_face.bbox.y), vision_face.bbox.width,
		gender, age_group, glasses, race, attractive, expression, smile, mask, age, vision_face.tracking_path);
}

void CameraTest(int camera_id, std::string model_name)
{
	vision::VisionFaceDetect* face_detect = vision::instantiateVisionFaceDetect(); // 包含默认初始化参数，无需再次初始化
	vision::VisionFaceAlignment* face_align = vision::instantiateVisionFaceAlignment(); // 包含默认初始化参数，无需再次初始化
	VisionFaceAttributes *face_attributes = instantiateVisionFaceAttributes();
	
	face_attributes->InitV3(model_name);

	cv::VideoCapture cap(camera_id);
	cv::Mat img;
	double start, stop, time;
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> img;
			if (!img.empty())
			{
				// Perform Face Detect First
				std::vector<vision::VisionFace> faces = face_detect->GetFaces(img); // 注意，此时VisionFace对象中除了检测框和人脸关键点，其他量都是无效的

				for (int i=0; i<faces.size(); i++)
				{
					cv::Mat norm_face = face_align->GetAlignedFace(img, faces[i].bbox);
					std::vector<int> results = face_attributes->GetAttributes(norm_face);
					faces[i].gender = results[0];
					faces[i].age_group = results[1];
					faces[i].glasses = results[2];
					faces[i].race = results[3];
					faces[i].attractive = results[4];
					faces[i].expression = results[5];
					faces[i].smile = results[6];
					faces[i].mask = results[7];
					faces[i].age = results[8];

					DrawAttributesResult(img, i+1, faces[i]);
					face_detect->DrawFaceRect(img, faces[i].bbox);
				}

				cv::imshow("Test", img);
				if (cv::waitKey(1) == 27)
				{
					break;
				}
			}
		}
	}

	vision::destroyVisionFaceDetect(face_detect);
	vision::destroyVisionFaceAlignment(face_align);
	vision::destroyVisionFaceAttributes(face_attributes);
}

int main(int argc, char** argv)
{
	if (argc == 4)
	{
        std::string license_file = std::string(argv[1]);
		int camra_id = 0;
        sscanf_s(argv[2], "%d", &camra_id);
        std::string model_name = std::string(argv[3]);

        vision::SetLicenseFile(license_file);
        CameraTest(camra_id, model_name);
    } else {
        std::cout << "Usage:  FaceAttributesTest license_file camera_id model_name" << std::endl;
    }
	return 0;
}
#endif