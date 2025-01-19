#include "../pch.h"
#include "../Headers/ImageUndistort.h"

ImageUndistort::ImageUndistort(cv::Mat cameraMatrix, cv::Mat distCoeffs) :CameraMatrix(cameraMatrix), DistCoeffs(distCoeffs) {}

void ImageUndistort::Undistort(cv::Mat distortedImage, bool isFishEye) {
	if (distortedImage.empty()) {
		std::cerr << "input image is empty" << std::endl;
		return;
	}
	if (isFishEye)
		cv::fisheye::undistortImage(distortedImage, this->UndistortedImage, this->CameraMatrix, this->DistCoeffs);
	else
		cv::undistort(distortedImage, this->UndistortedImage, this->CameraMatrix, this->DistCoeffs);
}

cv::Mat ImageUndistort::GetUndistortImage() const {
	if(!this->UndistortedImage.empty())
		return this->UndistortedImage;
	else {
		std::cerr << "undistorted image is empty" << std::endl;
		return this->UndistortedImage;
	}
}

void ImageUndistort::DrawReprojectedImage(
	std::vector<cv::Point3f> worldPoints,
	std::vector<cv::Point2f> imagePoints,
	std::vector<cv::Mat> rotationVectors,
	std::vector<cv::Mat> translationVectors,
	cv::Size imageSize) {

	// blank image
	this->ReprojectedImage = cv::Mat(imageSize, CV_8UC3, cv::Scalar(255, 255, 255));

	// project the world point to the image plane
	std::vector<cv::Point2f> ReprojectedPoints;
	cv::projectPoints(
		worldPoints, rotationVectors[0], translationVectors,
		CameraMatrix, DistCoeffs, ReprojectedPoints);

	// draw origin and projection points
	for (size_t i = 0; i < imagePoints.size(); i++) {
		cv::circle(this->ReprojectedImage, imagePoints[i], 5, cv::Scalar(0, 0, 255), -1); // original point
		cv::circle(this->ReprojectedImage, ReprojectedPoints[i], 5, cv::Scalar(0, 255, 0), -1); // recasted point
		cv::line(this->ReprojectedImage, imagePoints[i], ReprojectedPoints[i], cv::Scalar(255, 0, 0), 1);
	}

	cv::imshow("Reprojected Points", this->ReprojectedImage);
}

double ImageUndistort::ComputeReprojectionError(
	std::vector<std::vector<cv::Point3f>> worldPoints,
	std::vector<std::vector<cv::Point2f>> imagePoints,
	std::vector<cv::Mat> rotationVectors,
	std::vector<cv::Mat> translationVectors
) {
	double TotalError = 0.0;
	size_t TotalPoints = 0;

	for (size_t i = 0; i < worldPoints.size(); i++) {
		std::vector<cv::Point2f> ProjectedPoints;
		cv::projectPoints(
			worldPoints[i], rotationVectors[i], translationVectors[i],
			CameraMatrix, DistCoeffs, ProjectedPoints);

		double error = cv::norm(imagePoints[i], ProjectedPoints, cv::NORM_L2);
		TotalError += error * error;
		TotalPoints += worldPoints[i].size();
	}

	return std::sqrt(TotalError / TotalPoints);
}

cv::Mat ImageUndistort::GetReprojectedImage() const {
	if (this->ReprojectedImage.empty()) {
		std::cerr << "reprojected image has not been generated, please generate first" << std::endl;
		return cv::Mat();
	}
	return this->ReprojectedImage;
}

void ImageUndistort::SaveUndistortedImage(std::string outputPath) {
	if (this->UndistortedImage.empty()) {
		std::cerr << "empty image, can not save" << std::endl;
		return;
	}

	if (cv::imwrite(outputPath, this->UndistortedImage))
		std::cout << "undistorted image saved to : " << outputPath << std::endl;
	else
		std::cerr << "failed to save undistorted image to : " << outputPath << std::endl;
}

