#include "pch.h"
#include "CppUnitTest.h"
#include "PointMapper.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(TestPointMapper) {
	public:
		PointMapper pointMapper;
		TestPointMapper() {
			this->pointMapper.SetSquareSize(5.0f);
		}
		TEST_METHOD(TestSetBoardSizeAndGenerateRealWorldPoints) {

			Assert::AreEqual(5.0f, this->pointMapper.GetSquareSize());
			this->pointMapper.SetSquareSize(10.0f);
			this->pointMapper.SetBoardSize(cv::Size(8, 6));
			this->pointMapper.SetBoardType(BoardType::CHESSBOARD);

			Assert::AreEqual(10.0f, this->pointMapper.GetSquareSize());
			//Assert::AreEqual(cv::Size(8, 6), this->pointMapper.GetBoardSize());
			//Assert::AreEqual(BoardType::CHESSBOARD, this->pointMapper.GetBoardType());

			auto points = this->pointMapper.GenerateRealWorldPoints();
			// 检查生成的点数对不对
			Assert::AreEqual(size_t(48), points.size(), L"Generated point count mismatch.");
			// 检查点的位置对不对
			Assert::AreEqual(0.0f, points[0].x, L"first point x mismatch");
			Assert::AreEqual(0.0f, points[0].y, L"first point y mismatch");

			Assert::AreEqual(10.0f, points[1].x, L"Second point X mismatch.");
			Assert::AreEqual(0.0f, points[1].y, L"Second point Y mismatch.");

			Assert::AreEqual(0.0f, points[8].x);
			Assert::AreEqual(10.0f, points[8].y);
		}
		TEST_METHOD(TestAsymmetricCircleGridPoints) {
			this->pointMapper.SetBoardSize(cv::Size(4, 3));
			this->pointMapper.SetBoardType(BoardType::ASYMMETRIC_CIRCLE_GRID);

			auto points = this->pointMapper.GenerateRealWorldPoints();

			Assert::AreEqual(size_t(12), points.size());
			Assert::AreEqual(0.0f, points[0].x);
			Assert::AreEqual(2.5f, points[4].x);
			Assert::AreEqual(5.0f, points[4].y);
		}
		TEST_METHOD(TestMapSingleImagePoint) {
			this->pointMapper.SetBoardSize(cv::Size(3, 2));
			this->pointMapper.SetSquareSize(10.0f);
			this->pointMapper.SetBoardType(BoardType::CHESSBOARD);

			auto RealWorldPoints = this->pointMapper.GenerateRealWorldPoints();
			// 创建一个与真实点匹配的虚拟图像点
			std::vector<cv::Point2f> ValidImagePoints = {
				cv::Point2f(0.0f, 0.0f),
				cv::Point2f(10.0f, 0.0f),
				cv::Point2f(20.0f, 0.0f),
				cv::Point2f(0.0f, 10.0f),
				cv::Point2f(10.0f, 10.0f),
				cv::Point2f(20.0f, 10.0f),
			};
			// 不匹配的图像点
			std::vector<cv::Point2f> InvalidImagePoints = {
				cv::Point2f(0.0f, 0.0f),
				cv::Point2f(10.0f, 0.0f),
				cv::Point2f(20.0f, 0.0f),
				cv::Point2f(0.0f, 10.0f),
			};

			Assert::IsTrue(this->pointMapper.MapSingleImagePoint(ValidImagePoints, RealWorldPoints));
			try {
				this->pointMapper.MapSingleImagePoint(InvalidImagePoints, RealWorldPoints);
				Assert::Fail(L"Expected an exception due to mismatch in point count.");
			}
			catch (const std::runtime_error& e) {
				Assert::AreEqual("Mismatch between image points and world points count.", std::string(e.what()).c_str(), L"Exception message mismatch.");
			}

		}
	};
}