#ifndef CALCULATECORNER_H
#define CALCULATECORNER_H

#pragma once
#pragma warning(disable: 4251)

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include "../pch.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

class DLL_EXPORT CalculateCorner{
	// step one, code related to calculate corners
public:
	CalculateCorner();

	// set image path and board size
	void AddImagePath(std::string imagePath);
	void RemoveImagePath(std::string imagePath);
	void ResetImagePathVector();
	void SetBoardSize(cv::Size boardSize);

	// capture image from camera and save it to certain position
	// cameraIndex = 0 indicates using default camera that computer possesses, front end determine where the picture is stored
	bool CaptureImageFromCamera(std::string savePath, int cameraIndex);

	// draw x and y axis
	void DrawAxisOnChessBoard(cv::Mat image, std::vector<cv::Point2f> corners);

	// draw corners of certain pictures
	// calculate corner and distinguish whether it is fisheye camera
	// if found corners, then display image on the screen, execute remaining code after image is closed
	void CalculateSingleImageCorners(std::string imagePath, bool drawCorner, bool isFishEye); 
	// draw corners for every picture stored in ImagePathVector
	void CalculateEveryImageCorners(bool drawCorner, bool isFishEye);

	// get every image corners
	std::vector<std::vector<cv::Point2f>> GetAllImageCorners();
	// return dictionary, enable front end to fetch corners based on the file path
	std::map<std::string, std::vector<cv::Point2f>> GetImageCorner() const;
	// return dictionary, allow front end to fetch drawn corner pictures by path
	std::map<std::string, cv::Mat> GetProcessedImageByPath() const;
	// get file path
	std::vector<std::string> GetAllFilePaths() const;

private:
	std::vector<std::string> ImagePathVector;
	cv::Size BoardSize;
	std::map<std::string, std::vector<cv::Point2f>> FindCornersByPath;
	std::map<std::string, cv::Mat> FindProcessedImageByPath;
};

#endif // !CALCULATECORNER_H
