#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#pragma once
#pragma warning(disable: 4251)
#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#include "CalculateCorner.h"
#include "PointMapper.h"
#include "../Enums/CalibrationOptions.cpp"


#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class DLL_EXPORT CameraCalibrate{
	// step three, calculte world coordinate system and pixel coordinate system, calibrate the camera
public:
	CameraCalibrate();

	void CalibrateStandardCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// calibrate fisheye camera
	// only accept Skew and optimize parameters
	void CalibrateFishEyeCamera(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		cv::Size imageSize,
		CalibrationOptions option
	);

	// export camera parameters
	void ExportCameraParameters(std::string outputFile, cv::Size imageSize) const;

	cv::Mat GetCameraMatrix() const;
	cv::Mat GetDistCoeffs() const;
	std::vector<cv::Mat> GetRotationVectors() const;
	std::vector<cv::Mat> GetTranslationVectors() const;

private:
	cv::Mat CameraMatrix; // intrinsic matrix
	cv::Mat DistCoeffs; // distortion parameters
	std::vector<cv::Mat> RotationVectors; // rotation variables
	std::vector<cv::Mat> TranslationVectors; // translation variables
};

#endif // !CAMERACALIBRATION_H

