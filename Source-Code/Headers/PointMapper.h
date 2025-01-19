#ifndef POINTMAPPER_H
#define POINTMAPPER_H

#pragma once
#pragma warning(disable: 4251)

#include "CalculateCorner.h"
#include "../Enums/BoardType.cpp"
#include "../Enums/DistortionLevel.cpp"
#include "../pch.h"

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class DLL_EXPORT PointMapper{
	// step two, establish the projection relationship between real world coordinate system and pixel coordinate system
public:
	PointMapper();

	void SetSquareSize(float squareSize);
	void SetBoardSize(cv::Size boardSize);
	void SetBoardType(BoardType boardType);
	void SetDistortionLevel(DistortionLevel level);

	float GetSquareSize() const;
	cv::Size GetBoardSize() const;
	BoardType GetBoardType() const;
	DistortionLevel GetDistortionLevel() const;

	// generate real world point for single picture
	std::vector<cv::Point3f> GenerateRealWorldPoints(); 
	std::vector<cv::Point3f> GetRealWorldPoints() const;
	
	// check if real world points' number equals to pixel points' number
	bool MapSingleImagePoint(std::vector<cv::Point2f>& imagePoints, std::vector<cv::Point3f>& worldPoints);

private:
	CalculateCorner CalculateCorner;
	BoardType board_type;
	float SquareSize;
	cv::Size BoardSize;
	DistortionLevel distortionLevel;
	std::vector<cv::Point3f> RealWorldPoints;
};

#endif // !POINTMAPPER_H