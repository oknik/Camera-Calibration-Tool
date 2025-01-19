#ifndef IMAGEUNDISTORT_H
#define IMAGEUNDISTORT_H

#pragma once
#pragma warning(disable: 4251)
#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <opencv2/opencv.hpp>
#include <string>

class DLL_EXPORT ImageUndistort{
	// step four, according to camera parameters to undistort the picture
public:
	ImageUndistort(cv::Mat cameraMatrix, cv::Mat distCoeffs);

	// undistort image
	void Undistort(cv::Mat distortedImage, bool isFishEye);
	cv::Mat GetUndistortImage() const;

	// recast points
	void DrawReprojectedImage(
		std::vector<cv::Point3f> worldPoints,
		std::vector<cv::Point2f> imagePoints,
		std::vector<cv::Mat> rotationVectors,
		std::vector<cv::Mat> translationVectors,
		cv::Size imageSize
	);
	cv::Mat GetReprojectedImage() const;

	// compute the reprojection error
	double ComputeReprojectionError(
		std::vector<std::vector<cv::Point3f>> worldPoints,
		std::vector<std::vector<cv::Point2f>> imagePoints,
		std::vector<cv::Mat> rotationVectors,
		std::vector<cv::Mat> translationVectors
	);

	// save undistorted image
	void SaveUndistortedImage(std::string outputPath);

private:
	cv::Mat CameraMatrix;
	cv::Mat DistCoeffs;
	cv::Mat UndistortedImage;
	cv::Mat ReprojectedImage;
};

#endif // !IMAGEUNDISTORT

