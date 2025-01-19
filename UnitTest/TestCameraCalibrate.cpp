#include "pch.h"
#include "CppUnitTest.h"
#include "CameraCalibrate.h"
#include "PointMapper.h"
#include "CalculateCorner.h"
#include "Tools.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(TestCameraCalibrate) {
	public:
		CameraCalibrate cameraCalibrate;
		CalculateCorner calculator;
		PointMapper pointMapper;
		TEST_METHOD(TestStandardCamera) {
			CalibrationOptions calibrateOption;
			calibrateOption.RadialDistortion = RadialDistortionType::TWO_COEFFICIENTS;
			calibrateOption.UseTangentialDistortion = true;

			std::string imagePath = "D:/2024Fall/cv/FinalProject/Resources/Data/OrdinaryCamera/01-20mm/chess (0).JPG";
			std::string outputPath = "D:/2024Fall/cv/FinalProject/result.txt";
			cv::Size ImageSize = Tools::GetImageSize(imagePath);

			auto worldPoints = this->TestWorldCoordinates(cv::Size(8, 6), 20.0f);
			auto corners = this->TestCalculateCorner(cv::Size(8, 6), imagePath, false, false);
			
			Assert::IsTrue(this->pointMapper.MapSingleImagePoint(corners[0], worldPoints[0]), L"��С��ƥ��");

			this->cameraCalibrate.CalibrateStandardCamera(worldPoints, corners, ImageSize, calibrateOption);
			// ����Ƿ�Ӧ���� 2 ���������ϵ��
			double k1 = this->cameraCalibrate.GetDistCoeffs().at<double>(0); // ������� K1
			double k2 = this->cameraCalibrate.GetDistCoeffs().at<double>(1); // ������� K2
			Assert::IsTrue(k1 != 0 || k2 != 0, L"�������ϵ��δ��ȷӦ��");
			// ����Ƿ�����������
			if (calibrateOption.UseTangentialDistortion) {
				double p1 = this->cameraCalibrate.GetDistCoeffs().at<double>(2);
				double p2 = this->cameraCalibrate.GetDistCoeffs().at<double>(3);
				Assert::IsTrue(p1 != 0 || p2 != 0, L"�������δ��ȷӦ��");
			}
			this->cameraCalibrate.ExportCameraParameters(outputPath, ImageSize);
		};
		TEST_METHOD(TestFisheyeCamera) {
			std::string path = "D:/2024Fall/cv/FinalProject/Resources/Data/FisheyeCamera/01-20mm/img0.bmp";
			std::string outputPath = "D:/2024Fall/cv/FinalProject/fisheye.txt";
			cv::Size boardSize(9, 6);
			cv::Size imageSize = Tools::GetImageSize(path);

			CalibrationOptions option;
			option.RadialDistortion = RadialDistortionType::THREE_COEFFICIENTS;
			option.OptimizeParameters = true;

			std::vector<std::vector<cv::Point3f>> worldPoints = this->TestWorldCoordinates(boardSize, 20.0f);
			std::vector<std::vector<cv::Point2f>> corners = this->TestCalculateCorner(boardSize, path, false, true);

			Assert::IsTrue(this->pointMapper.MapSingleImagePoint(corners[0], worldPoints[0]), L"��С��ƥ��");

			this->cameraCalibrate.CalibrateFishEyeCamera(worldPoints, corners, imageSize, option);
			// ��֤�������ͻ���ϵ���Ѿ�����
			cv::Mat cameraMatrix = this->cameraCalibrate.GetCameraMatrix();
			cv::Mat distCoeffs = this->cameraCalibrate.GetDistCoeffs();
			Assert::IsFalse(cameraMatrix.empty());
			Assert::IsFalse(distCoeffs.empty());

			// ��֤��ת��λ������������
			auto rvecs = this->cameraCalibrate.GetRotationVectors();
			auto tvecs = this->cameraCalibrate.GetTranslationVectors();
			Assert::AreEqual(rvecs.size(), corners.size());
			Assert::AreEqual(tvecs.size(), corners.size());

			this->cameraCalibrate.ExportCameraParameters(outputPath, imageSize);

		}
	private:
		std::vector<std::vector<cv::Point2f>> TestCalculateCorner(cv::Size boardSize,std::string path, bool drawImage, bool isFishEye) {
			// ʹ�õĲ���ͼƬ��7��9�еĸ��ӣ���ôBoardSizeӦΪ (9-1, 7-1)
			this->calculator.SetBoardSize(boardSize);
			// ���Ե�ʱ����CalculateCorner�����������C:/...���棬Ϊ�˷�������Ҿ�д�ɾ���·����

			this->calculator.AddImagePath(path);
			this->calculator.CalculateEveryImageCorners(drawImage, isFishEye);
			auto ImageCorner = this->calculator.GetImageCorner();
			if (ImageCorner.find(path) != ImageCorner.end()) {
				std::vector<std::vector<cv::Point2f>> result;
				result.push_back(ImageCorner.at(path));
				return result;
			}
			else
				return std::vector<std::vector<cv::Point2f>>();
		}
		std::vector<std::vector<cv::Point3f>> TestWorldCoordinates(cv::Size boardSize, float squareSize) {
			this->pointMapper.SetBoardType(BoardType::CHESSBOARD);
			this->pointMapper.SetBoardSize(boardSize);
			this->pointMapper.SetSquareSize(squareSize);
			std::vector<cv::Point3f> current = this->pointMapper.GenerateRealWorldPoints();
			std::vector<std::vector<cv::Point3f>> result;
			result.push_back(current);
			return result;
		}
	};
}