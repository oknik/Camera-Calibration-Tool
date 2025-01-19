#include "../pch.h"
#include "../Headers/PointMapper.h"

PointMapper::PointMapper() {}

void PointMapper::SetSquareSize(float squareSize) {
	this->SquareSize = squareSize;
}

void PointMapper::SetBoardSize(cv::Size boardSize) {
	this->BoardSize = boardSize;
}

void PointMapper::SetBoardType(BoardType boardType) {
	this->board_type = boardType;
}

void PointMapper::SetDistortionLevel(DistortionLevel level) {
	this->distortionLevel = level;
}

float PointMapper::GetSquareSize() const {
	return this->SquareSize;
}

cv::Size PointMapper::GetBoardSize() const {
	return this->BoardSize;
}

BoardType PointMapper::GetBoardType() const {
	return this->board_type;
}

DistortionLevel PointMapper::GetDistortionLevel() const {
	return this->distortionLevel;
}

std::vector<cv::Point3f> PointMapper::GenerateRealWorldPoints() {
	std::vector<cv::Point3f> realWorldPoints;

	switch (this->board_type) {
	case BoardType::CHESSBOARD:
		for (int i = 0; i < this->BoardSize.height; i++)
			for (int j = 0; j < this->BoardSize.width; j++)
				realWorldPoints.emplace_back(j * this->SquareSize, i * this->SquareSize, 0);
		break;
	case BoardType::SYMMETRIC_CIRCLE_GRID:
		for (int i = 0; i < this->BoardSize.height; i++)
			for (int j = 0; j < this->BoardSize.width; j++)
				realWorldPoints.emplace_back(j * this->SquareSize, i * this->SquareSize, 0);
		break;
	case BoardType::ASYMMETRIC_CIRCLE_GRID:
		for (int i = 0; i < this->BoardSize.height; i++) {
			for (int j = 0; j < this->BoardSize.width; j++) {
				float xOffset = (i % 2 == 0) ? 0.0f : this->SquareSize / 2.0f; 
				realWorldPoints.emplace_back(j * this->SquareSize + xOffset, i * this->SquareSize, 0);
			}
		}
		break;
	}
	this->RealWorldPoints = realWorldPoints;
	return realWorldPoints;
}

std::vector<cv::Point3f> PointMapper::GetRealWorldPoints() const {
	return this->RealWorldPoints;
}

bool PointMapper::MapSingleImagePoint(std::vector<cv::Point2f>& imagePoints, std::vector<cv::Point3f>& worldPoints) {
	if (imagePoints.size() != worldPoints.size()) {
		throw std::runtime_error("Mismatch between image points and world points count.");
		return false;
	}
	return true;
}
