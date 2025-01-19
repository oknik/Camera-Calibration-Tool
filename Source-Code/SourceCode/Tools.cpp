#include "../pch.h"
#include "../Headers/Tools.h"

cv::Size Tools::GetImageSize(std::string imagePath) {
	cv::Mat image = cv::imread(imagePath);
	if (image.empty())
		throw std::runtime_error("unable to load image " + imagePath);
	return cv::Size(image.cols, image.rows);
}