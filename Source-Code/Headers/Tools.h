#ifndef TOOLS_H
#define TOOLS_H

#pragma once
#pragma warning(disable: 4251)

#include "../pch.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <string>

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif



class DLL_EXPORT Tools {
public:
	cv::Size static GetImageSize(std::string imagePath);
};

#endif